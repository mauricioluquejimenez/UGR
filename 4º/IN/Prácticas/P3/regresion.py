# p3_regresion_base.py
# Base sólida para Práctica 3 (regresión) con RMSE y submission para Zindi
# Modelos: CatBoost, LightGBM, XGBoost
# Extras útiles:
# - Importancias a CSV
# - Log de experimentos a resultados/resultados.csv
# - Tuning opcional (RandomizedSearchCV) para LGBM y XGB (RMSE)
#
# Requisitos:
#   pip install pandas numpy scikit-learn catboost lightgbm xgboost scipy

from __future__ import annotations

import os
import json
import argparse
import warnings
from dataclasses import dataclass
from datetime import datetime
from typing import Tuple, List, Optional, Dict, Any

import numpy as np
import pandas as pd

from sklearn.model_selection import KFold, GroupKFold, RandomizedSearchCV
from sklearn.metrics import mean_squared_error, make_scorer
from sklearn.compose import ColumnTransformer
from sklearn.pipeline import Pipeline
from sklearn.preprocessing import OneHotEncoder
from sklearn.impute import SimpleImputer

warnings.filterwarnings("ignore")


# -----------------------------
# Config
# -----------------------------
@dataclass
class Paths:
    train_csv: str = "Train.csv"
    test_csv: str = "Test.csv"
    sample_csv: str = "SampleSubmission.csv"
    out_dir: str = "resultados"


@dataclass
class CVConfig:
    n_splits: int = 5
    seed: int = 42
    use_group_kfold: bool = False  # True -> agrupa por Place_ID


# -----------------------------
# Métrica
# -----------------------------
def rmse(y_true: np.ndarray, y_pred: np.ndarray) -> float:
    return float(np.sqrt(mean_squared_error(y_true, y_pred)))


def rmse_scorer():
    # sklearn maximiza el score, así que devolvemos negativo
    return make_scorer(lambda yt, yp: -rmse(yt, yp), greater_is_better=True)


# -----------------------------
# IO / utilidades
# -----------------------------
def ensure_out_dir(out_dir: str) -> None:
    os.makedirs(out_dir, exist_ok=True)


def now_str() -> str:
    return datetime.now().strftime("%Y-%m-%d %H:%M:%S")


def append_experiment_log(out_dir: str, row: Dict[str, Any]) -> None:
    """
    Log simple a CSV para que luego puedas montar la tabla de experimentos del guion sin dolor.
    No es "documentación", es trazabilidad mínima del estudio.
    """
    ensure_out_dir(out_dir)
    path = os.path.join(out_dir, "experiments.csv")
    df = pd.DataFrame([row])

    if os.path.exists(path):
        df.to_csv(path, mode="a", index=False, header=False)
    else:
        df.to_csv(path, index=False)


def add_date_features(df: pd.DataFrame, date_col: str = "Date") -> pd.DataFrame:
    out = df.copy()
    dt = pd.to_datetime(out[date_col], errors="coerce")

    out["year"] = dt.dt.year.astype("float32")
    out["month"] = dt.dt.month.astype("float32")
    out["day"] = dt.dt.day.astype("float32")
    out["dayofweek"] = dt.dt.dayofweek.astype("float32")
    out["dayofyear"] = dt.dt.dayofyear.astype("float32")
    out["weekofyear"] = dt.dt.isocalendar().week.astype("float32")
    out["is_weekend"] = (dt.dt.dayofweek >= 5).astype("float32")

    # Estacionalidad suave (barata y suele ayudar algo)
    # Si dayofyear es NaN -> sin/cos -> NaN, y los modelos lo gestionan o se imputará
    rad = 2.0 * np.pi * (out["dayofyear"] / 365.0)
    out["doy_sin"] = np.sin(rad).astype("float32")
    out["doy_cos"] = np.cos(rad).astype("float32")

    return out


def make_datasets(
    train: pd.DataFrame,
    test: pd.DataFrame,
    target_col: str = "target",
    id_col: str = "Place_ID X Date",
    date_col: str = "Date",
    place_col: str = "Place_ID",
) -> Tuple[pd.DataFrame, np.ndarray, pd.DataFrame, pd.Series]:
    train = train.copy()
    test = test.copy()

    test_ids = test[id_col].copy()

    train = add_date_features(train, date_col=date_col)
    test = add_date_features(test, date_col=date_col)

    drop_cols = [id_col, date_col]

    # En Train hay columnas con stats del target que no existen en Test.
    # Usarlas como features rompe el escenario real.
    extra_target_stats = ["target_min", "target_max", "target_variance", "target_count"]
    for c in extra_target_stats:
        if c in train.columns:
            drop_cols.append(c)

    y = train[target_col].astype("float32").to_numpy()

    X_train = train.drop(columns=[target_col] + drop_cols, errors="ignore")
    X_test = test.drop(columns=drop_cols, errors="ignore")

    # Marca Place_ID como categórica y alinea categorías
    if place_col in X_train.columns:
        X_train[place_col] = X_train[place_col].astype("category")
        X_test[place_col] = X_test[place_col].astype("category")

        all_cats = pd.Categorical(pd.concat(
            [X_train[place_col].astype(str), X_test[place_col].astype(str)],
            axis=0
        ))
        X_train[place_col] = pd.Categorical(X_train[place_col].astype(str), categories=all_cats.categories)
        X_test[place_col] = pd.Categorical(X_test[place_col].astype(str), categories=all_cats.categories)

    # Asegura mismas columnas (por si en algún sitio se te coló algo raro)
    X_test = X_test.reindex(columns=X_train.columns, fill_value=np.nan)

    return X_train, y, X_test, test_ids


def get_splits(X: pd.DataFrame, y: np.ndarray, cv: CVConfig, place_col: str = "Place_ID"):
    if cv.use_group_kfold and place_col in X.columns:
        splitter = GroupKFold(n_splits=cv.n_splits)
        groups = X[place_col].astype(str).values
        return splitter.split(X, y, groups=groups)
    splitter = KFold(n_splits=cv.n_splits, shuffle=True, random_state=cv.seed)
    return splitter.split(X, y)


# -----------------------------
# Modelos: CatBoost
# -----------------------------
def train_predict_catboost(
    X_train: pd.DataFrame,
    y_train: np.ndarray,
    X_test: pd.DataFrame,
    cv: CVConfig,
    out_dir: str,
    place_col: str = "Place_ID",
) -> Tuple[np.ndarray, np.ndarray, float]:
    from catboost import CatBoostRegressor, Pool

    ensure_out_dir(out_dir)

    cat_features = []
    if place_col in X_train.columns:
        cat_features = [X_train.columns.get_loc(place_col)]

    oof = np.zeros(len(X_train), dtype="float32")
    test_pred = np.zeros(len(X_test), dtype="float32")
    fold_scores: List[float] = []

    for fold, (tr_idx, va_idx) in enumerate(get_splits(X_train, y_train, cv, place_col=place_col), start=1):
        X_tr, X_va = X_train.iloc[tr_idx], X_train.iloc[va_idx]
        y_tr, y_va = y_train[tr_idx], y_train[va_idx]

        train_pool = Pool(X_tr, y_tr, cat_features=cat_features)
        valid_pool = Pool(X_va, y_va, cat_features=cat_features)
        test_pool = Pool(X_test, cat_features=cat_features)

        model = CatBoostRegressor(
            loss_function="RMSE",
            eval_metric="RMSE",
            iterations=2000,
            learning_rate=0.03,
            depth=8,
            l2_leaf_reg=3.0,
            random_seed=cv.seed,
            od_type="Iter",
            od_wait=250,
            verbose=250,
        )

        model.fit(train_pool, eval_set=valid_pool, use_best_model=True)

        va_pred = model.predict(X_va).astype("float32")
        oof[va_idx] = va_pred

        fold_rmse = rmse(y_va, va_pred)
        fold_scores.append(fold_rmse)
        print(f"[CatBoost] Fold {fold}/{cv.n_splits} RMSE: {fold_rmse:.6f}")

        test_pred += model.predict(test_pool).astype("float32") / cv.n_splits

        model_path = os.path.join(out_dir, f"catboost_fold{fold}.cbm")
        model.save_model(model_path)

        # Importancias por fold (útil para ver estabilidad)
        imp = pd.DataFrame({
            "feature": X_train.columns,
            "importance": model.get_feature_importance(type="FeatureImportance")
        }).sort_values("importance", ascending=False)
        imp.to_csv(os.path.join(out_dir, f"catboost_importance_fold{fold}.csv"), index=False)

    overall = rmse(y_train, oof)
    print(f"[CatBoost] OOF RMSE: {overall:.6f} | folds: {np.mean(fold_scores):.6f} ± {np.std(fold_scores):.6f}")

    np.save(os.path.join(out_dir, "oof_catboost.npy"), oof)
    np.save(os.path.join(out_dir, "testpred_catboost.npy"), test_pred)

    # Importancia media (promedio de importancias de folds)
    try:
        imp_files = [os.path.join(out_dir, f"catboost_importance_fold{i}.csv") for i in range(1, cv.n_splits + 1)]
        imps = [pd.read_csv(p).set_index("feature")["importance"] for p in imp_files]
        mean_imp = pd.concat(imps, axis=1).mean(axis=1).sort_values(ascending=False).reset_index()
        mean_imp.columns = ["feature", "importance_mean"]
        mean_imp.to_csv(os.path.join(out_dir, "catboost_importance_mean.csv"), index=False)
    except Exception:
        pass

    return oof, test_pred, overall


# -----------------------------
# Modelos: LightGBM
# -----------------------------
def _lgbm_base_model(seed: int) -> Any:
    import lightgbm as lgb
    return lgb.LGBMRegressor(
        objective="regression",
        n_estimators=30000,
        learning_rate=0.02,
        num_leaves=255,
        subsample=0.8,
        colsample_bytree=0.8,
        reg_alpha=0.0,
        reg_lambda=1.0,
        random_state=seed,
    )


def train_predict_lightgbm(
    X_train: pd.DataFrame,
    y_train: np.ndarray,
    X_test: pd.DataFrame,
    cv: CVConfig,
    out_dir: str,
    tune: bool = False,
    tune_iter: int = 30,
    place_col: str = "Place_ID",
) -> Tuple[np.ndarray, np.ndarray, float, Dict[str, Any]]:
    import lightgbm as lgb

    ensure_out_dir(out_dir)

    categorical_features = [place_col] if place_col in X_train.columns else []

    # Tuning global rápido (no por fold) para no disparar tiempo.
    # Después hacemos CV normal con esos params.
    best_params: Dict[str, Any] = {}
    if tune:
        base = _lgbm_base_model(cv.seed)
        # Espacio de búsqueda razonable (adaptación del ejemplo a regresión)
        # El ejemplo usa Grid/RandomSearch en clasificación :contentReference[oaicite:7]{index=7}
        param_dist = {
            "num_leaves": [31, 63, 127, 255],
            "min_child_samples": [10, 20, 40, 80],
            "subsample": [0.6, 0.7, 0.8, 0.9, 1.0],
            "colsample_bytree": [0.6, 0.7, 0.8, 0.9, 1.0],
            "reg_lambda": [0.0, 0.5, 1.0, 2.0, 5.0],
            "reg_alpha": [0.0, 0.1, 0.5, 1.0],
            "learning_rate": [0.01, 0.02, 0.03, 0.05],
        }

        # Para que RandomizedSearchCV funcione bien con categóricas en pandas,
        # evitamos transformadores y dejamos dtype category.
        rs = RandomizedSearchCV(
            estimator=base,
            param_distributions=param_dist,
            n_iter=tune_iter,
            scoring=rmse_scorer(),
            cv=3,
            random_state=cv.seed,
            verbose=1,
            n_jobs=-1,
        )
        rs.fit(X_train, y_train, categorical_feature=categorical_features if categorical_features else "auto")
        best_params = rs.best_params_
        print(f"[LightGBM] Best params (RandomizedSearchCV): {best_params}")

    # Splitter
    splits = get_splits(X_train, y_train, cv, place_col=place_col)

    oof = np.zeros(len(X_train), dtype="float32")
    test_pred = np.zeros(len(X_test), dtype="float32")
    fold_scores: List[float] = []

    for fold, (tr_idx, va_idx) in enumerate(splits, start=1):
        X_tr, X_va = X_train.iloc[tr_idx], X_train.iloc[va_idx]
        y_tr, y_va = y_train[tr_idx], y_train[va_idx]

        model = _lgbm_base_model(cv.seed)
        if best_params:
            model.set_params(**best_params)

        model.fit(
            X_tr,
            y_tr,
            eval_set=[(X_va, y_va)],
            eval_metric="rmse",
            callbacks=[lgb.early_stopping(stopping_rounds=400, verbose=True)],
            categorical_feature=categorical_features if categorical_features else "auto",
        )

        va_pred = model.predict(X_va, num_iteration=model.best_iteration_).astype("float32")
        oof[va_idx] = va_pred

        fold_rmse = rmse(y_va, va_pred)
        fold_scores.append(fold_rmse)
        print(f"[LightGBM] Fold {fold}/{cv.n_splits} RMSE: {fold_rmse:.6f}")

        test_pred += model.predict(X_test, num_iteration=model.best_iteration_).astype("float32") / cv.n_splits

        model_path = os.path.join(out_dir, f"lgbm_fold{fold}.txt")
        model.booster_.save_model(model_path)

        # Importancias por fold
        imp = pd.DataFrame({
            "feature": X_train.columns,
            "importance_gain": model.booster_.feature_importance(importance_type="gain"),
            "importance_split": model.booster_.feature_importance(importance_type="split"),
        }).sort_values("importance_gain", ascending=False)
        imp.to_csv(os.path.join(out_dir, f"lgbm_importance_fold{fold}.csv"), index=False)

    overall = rmse(y_train, oof)
    print(f"[LightGBM] OOF RMSE: {overall:.6f} | folds: {np.mean(fold_scores):.6f} ± {np.std(fold_scores):.6f}")

    np.save(os.path.join(out_dir, "oof_lgbm.npy"), oof)
    np.save(os.path.join(out_dir, "testpred_lgbm.npy"), test_pred)

    # Importancia media
    try:
        imp_files = [os.path.join(out_dir, f"lgbm_importance_fold{i}.csv") for i in range(1, cv.n_splits + 1)]
        imps = [pd.read_csv(p).set_index("feature")["importance_gain"] for p in imp_files]
        mean_imp = pd.concat(imps, axis=1).mean(axis=1).sort_values(ascending=False).reset_index()
        mean_imp.columns = ["feature", "importance_gain_mean"]
        mean_imp.to_csv(os.path.join(out_dir, "lgbm_importance_mean.csv"), index=False)
    except Exception:
        pass

    return oof, test_pred, overall, best_params


# -----------------------------
# Modelos: XGBoost (con prepro correcto sin fuga)
# -----------------------------
def _xgb_base_model(seed: int) -> Any:
    from xgboost import XGBRegressor
    return XGBRegressor(
        n_estimators=30000,
        learning_rate=0.02,
        max_depth=8,
        min_child_weight=1.0,
        subsample=0.8,
        colsample_bytree=0.8,
        reg_alpha=0.0,
        reg_lambda=1.0,
        objective="reg:squarederror",
        random_state=seed,
        tree_method="hist",
    )


def train_predict_xgboost(
    X_train: pd.DataFrame,
    y_train: np.ndarray,
    X_test: pd.DataFrame,
    cv: CVConfig,
    out_dir: str,
    tune: bool = False,
    tune_iter: int = 30,
    place_col: str = "Place_ID",
) -> Tuple[np.ndarray, np.ndarray, float, Dict[str, Any]]:
    from xgboost import XGBRegressor

    ensure_out_dir(out_dir)

    cat_cols = [place_col] if place_col in X_train.columns else []
    num_cols = [c for c in X_train.columns if c not in cat_cols]

    preprocessor = ColumnTransformer(
        transformers=[
            ("num", Pipeline(steps=[
                ("imputer", SimpleImputer(strategy="median")),
            ]), num_cols),
            ("cat", Pipeline(steps=[
                ("imputer", SimpleImputer(strategy="most_frequent")),
                ("ohe", OneHotEncoder(handle_unknown="ignore")),
            ]), cat_cols),
        ],
        remainder="drop",
        sparse_threshold=0.3,
    )

    best_params: Dict[str, Any] = {}
    if tune:
        # Tuning global sobre pipeline (sin early stopping aquí para simplificar)
        pipe = Pipeline(steps=[
            ("prep", preprocessor),
            ("model", _xgb_base_model(cv.seed)),
        ])

        param_dist = {
            "model__max_depth": [4, 6, 8, 10],
            "model__min_child_weight": [1.0, 2.0, 5.0, 10.0],
            "model__subsample": [0.6, 0.7, 0.8, 0.9, 1.0],
            "model__colsample_bytree": [0.6, 0.7, 0.8, 0.9, 1.0],
            "model__reg_lambda": [0.0, 0.5, 1.0, 2.0, 5.0],
            "model__reg_alpha": [0.0, 0.1, 0.5, 1.0],
            "model__learning_rate": [0.01, 0.02, 0.03, 0.05],
        }

        rs = RandomizedSearchCV(
            estimator=pipe,
            param_distributions=param_dist,
            n_iter=tune_iter,
            scoring=rmse_scorer(),
            cv=3,
            random_state=cv.seed,
            verbose=1,
            n_jobs=-1,
        )
        rs.fit(X_train, y_train)
        best_params = rs.best_params_
        print(f"[XGBoost] Best params (RandomizedSearchCV): {best_params}")

    splits = get_splits(X_train, y_train, cv, place_col=place_col)

    oof = np.zeros(len(X_train), dtype="float32")
    test_pred = np.zeros(len(X_test), dtype="float32")
    fold_scores: List[float] = []

    for fold, (tr_idx, va_idx) in enumerate(splits, start=1):
        X_tr, X_va = X_train.iloc[tr_idx], X_train.iloc[va_idx]
        y_tr, y_va = y_train[tr_idx], y_train[va_idx]

        # Fit del prepro SOLO en train del fold
        X_tr_t = preprocessor.fit_transform(X_tr)
        X_va_t = preprocessor.transform(X_va)
        X_te_t = preprocessor.transform(X_test)

        model: XGBRegressor = _xgb_base_model(cv.seed)
        if best_params:
            # Filtra params del modelo (vienen con prefijo model__)
            mp = {k.replace("model__", ""): v for k, v in best_params.items() if k.startswith("model__")}
            model.set_params(**mp)

        model.fit(
            X_tr_t,
            y_tr,
            eval_set=[(X_va_t, y_va)],
            verbose=200,
            early_stopping_rounds=400,
        )

        va_pred = model.predict(X_va_t).astype("float32")
        oof[va_idx] = va_pred

        fold_rmse = rmse(y_va, va_pred)
        fold_scores.append(fold_rmse)
        print(f"[XGBoost] Fold {fold}/{cv.n_splits} RMSE: {fold_rmse:.6f}")

        test_pred += model.predict(X_te_t).astype("float32") / cv.n_splits

        model_path = os.path.join(out_dir, f"xgb_fold{fold}.json")
        model.save_model(model_path)

    overall = rmse(y_train, oof)
    print(f"[XGBoost] OOF RMSE: {overall:.6f} | folds: {np.mean(fold_scores):.6f} ± {np.std(fold_scores):.6f}")

    np.save(os.path.join(out_dir, "oof_xgb.npy"), oof)
    np.save(os.path.join(out_dir, "testpred_xgb.npy"), test_pred)

    return oof, test_pred, overall, best_params


# -----------------------------
# Submission
# -----------------------------
def make_submission(
    sample: pd.DataFrame,
    test_ids: pd.Series,
    preds: np.ndarray,
    out_path: str,
    id_col: str = "Place_ID X Date",
    target_col: str = "target",
) -> pd.DataFrame:
    sub = sample.copy()
    sub[id_col] = test_ids.values
    sub[target_col] = preds.astype("float32")
    sub.to_csv(out_path, index=False)
    return sub


# -----------------------------
# Main
# -----------------------------
def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--model", type=str, default="catboost", choices=["catboost", "lightgbm", "xgboost"])
    parser.add_argument("--n_splits", type=int, default=5)
    parser.add_argument("--seed", type=int, default=42)
    parser.add_argument("--group_kfold", action="store_true", help="Usa GroupKFold agrupando por Place_ID")
    parser.add_argument("--tune", action="store_true", help="Activa RandomizedSearchCV (solo LGBM y XGB)")
    parser.add_argument("--tune_iter", type=int, default=30, help="Iteraciones de RandomizedSearchCV")
    parser.add_argument("--out_dir", type=str, default="resulatdos")
    parser.add_argument("--train_csv", type=str, default="Train.csv")
    parser.add_argument("--test_csv", type=str, default="Test.csv")
    parser.add_argument("--sample_csv", type=str, default="SampleSubmission.csv")
    args = parser.parse_args()

    paths = Paths(train_csv=args.train_csv, test_csv=args.test_csv, sample_csv=args.sample_csv, out_dir=args.out_dir)
    cv = CVConfig(n_splits=args.n_splits, seed=args.seed, use_group_kfold=args.group_kfold)

    ensure_out_dir(paths.out_dir)

    # Carga
    train = pd.read_csv(paths.train_csv)
    test = pd.read_csv(paths.test_csv)
    sample = pd.read_csv(paths.sample_csv)

    # Dataset final
    X_train, y_train, X_test, test_ids = make_datasets(train, test)

    best_params: Dict[str, Any] = {}

    # Entrena y predice
    if args.model == "catboost":
        oof, test_pred, score = train_predict_catboost(X_train, y_train, X_test, cv, paths.out_dir)
    elif args.model == "lightgbm":
        oof, test_pred, score, best_params = train_predict_lightgbm(
            X_train, y_train, X_test, cv, paths.out_dir, tune=args.tune, tune_iter=args.tune_iter
        )
    else:
        oof, test_pred, score, best_params = train_predict_xgboost(
            X_train, y_train, X_test, cv, paths.out_dir, tune=args.tune, tune_iter=args.tune_iter
        )

    # Submission
    out_csv = os.path.join(paths.out_dir, f"submission_{args.model}_rmse_{score:.6f}.csv")
    sub = make_submission(sample, test_ids, test_pred, out_csv)

    print(f"Submission guardada en: {out_csv}")
    print(sub.head())

    # Log de experimento (para trazabilidad del estudio)
    exp_row = {
        "timestamp_local": now_str(),
        "model": args.model,
        "cv": f"{'GroupKFold' if args.group_kfold else 'KFold'}({args.n_splits})",
        "seed": args.seed,
        "tune": bool(args.tune),
        "tune_iter": int(args.tune_iter) if args.tune else 0,
        "oof_rmse": float(score),
        "out_csv": os.path.basename(out_csv),
        "best_params_json": json.dumps(best_params, ensure_ascii=False) if best_params else "",
        "notes": "",
    }
    append_experiment_log(paths.out_dir, exp_row)


if __name__ == "__main__":
    main()