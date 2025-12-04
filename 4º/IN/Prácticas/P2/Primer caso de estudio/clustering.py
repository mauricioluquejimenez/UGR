# Mauricio Luque Jiménez
# Primer caso de estudio: ingresos recientes y expectativa económica

import time
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import seaborn as sns
from sklearn import metrics
from sklearn.cluster import KMeans, AgglomerativeClustering, Birch, DBSCAN, MeanShift, estimate_bandwidth
from sklearn.impute import KNNImputer

def normalizacion(df): return (df - df.min()) * 1.0 / (df.max() - df.min())

def evaluar_clustering(nombre, etiquetas, df_eval, tiempo, info_extra = None):
    etiquetas = np.array(etiquetas)

    # máscara para ignorar ruido (-1)
    mask = etiquetas != -1
    if mask.sum() == 0:
        n_clusters = 0
        ch = np.nan
        sil = np.nan
    else:
        etiquetas_eff = etiquetas[mask]
        df_eff = df_eval[mask]

        n_clusters = len(np.unique(etiquetas_eff))

        if n_clusters <= 1:
            ch = np.nan
            sil = np.nan
        else:
            ch = metrics.calinski_harabasz_score(df_eff, etiquetas_eff)

            if df_eff.shape[0] > 10000:
                sample_size = min(5000, df_eff.shape[0])
            else:
                sample_size = None

            sil = metrics.silhouette_score(
                df_eff,
                etiquetas_eff,
                metric = "euclidean",
                sample_size = sample_size,
                random_state = 42
            )

    resultado = {
        "algoritmo": nombre,
        "n_clusters": n_clusters,
        "CH": ch,
        "Silhouette": sil,
        "tiempo": tiempo
    }
    if info_extra is not None:
        resultado.update(info_extra)
    return resultado

# ---------------------------------------------------------
# 1) Lectura del fichero CSV
# ---------------------------------------------------------
datos = pd.read_csv("ECV.csv", low_memory = False)
print("Filas, columnas (datos completos):", datos.shape)

# ---------------------------------------------------------
# 2) Selección de segmento: HI010 informada (flag = 1)
# ---------------------------------------------------------
subset = datos[datos["HI010_F"] == 1].copy()
print("Filas, columnas:", subset.shape)

# ---------------------------------------------------------
# 3) Renombrar variables de interés para trabajar más cómodo
# ---------------------------------------------------------
subset = subset.rename(columns = {
    "HY020":   "renta_disp",
    "HY022":   "renta_antes_trans_no_pensiones",
    "HY023":   "renta_antes_todas_trans",
    "vhRentaa": "renta_equiv",
    "HS040":   "vacaciones",
    "HS050":   "comida_calidad",
    "HS060":   "gasto_imprevisto",
    "HS120":   "fin_de_mes",
    "HI040":   "exp_ingresos"
})

# ---------------------------------------------------------
# 4) Lista de variables que entran en el clustering
# ---------------------------------------------------------
usadas = [
    "renta_disp",
    "renta_antes_trans_no_pensiones",
    "renta_antes_todas_trans",
    "renta_equiv",
    "vacaciones",
    "comida_calidad",
    "gasto_imprevisto",
    "fin_de_mes",
    "exp_ingresos"
]

# ---------------------------------------------------------
# 5) Construir matriz de trabajo y convertir a numérico
# ---------------------------------------------------------
df = subset[usadas].copy()
print("\nVista inicial de df:")
print(df.head())

for c in usadas:
    df[c] = pd.to_numeric(df[c], errors = "coerce")

# ---------------------------------------------------------
# 6) Normalizar a [0,1]
# ---------------------------------------------------------
df = df.apply(normalizacion)

print("\nMatriz df normalizada:", df.shape)
print(df.head())

print("\nNaN por columna antes de imputar:")
print(df.isna().sum())

# ---------------------------------------------------------
# 7) Imputación KNN sobre la matriz normalizada
# ---------------------------------------------------------
imputer = KNNImputer(n_neighbors = 5)
imputacion = imputer.fit_transform(df)

# ---------------------------------------------------------
# 8) Volver a DataFrame con mismos índices y columnas
# ---------------------------------------------------------

df = pd.DataFrame(imputacion, index = df.index, columns = df.columns)

print("\nMatriz df tras imputación:", df.shape)
print(df.head())

print("\nNaN por columna después de imputar:")
print(df.isna().sum())

# Matriz final de trabajo (numpy) y subset alineado
matriz = df.values
subset_clust = subset.loc[df.index].copy()

# Contenedor para todos los resultados de modelos
resultados = []

# Rango de k para algoritmos que lo utilizan
rango_k = range(2, 9)

# ---------------------------------------------------------
# 9) KMeans clásico
# ---------------------------------------------------------
print("\n=== KMeans clásico ===")

for k in rango_k:
    print(f"Probando KMeans con k = {k}")
    t0 = time.time()
    modelo = KMeans(
        n_clusters = k,
        init = "k-means++",
        n_init = 10,
        random_state = 42
    )
    etiquetas = modelo.fit_predict(matriz)
    t1 = time.time()

    res = evaluar_clustering(
        nombre = "kmeans",
        etiquetas = etiquetas,
        df_eval = matriz,
        tiempo = t1 - t0,
        info_extra = {"k": k}
    )
    resultados.append(res)

# ---------------------------------------------------------
# 10) AgglomerativeClustering (Ward) sobre muestra
# ---------------------------------------------------------
print("\n=== AgglomerativeClustering (Ward) sobre muestra ===")

tam_muestra = 3000 if matriz.shape[0] > 3000 else matriz.shape[0]
idx_muestra = np.random.choice(matriz.shape[0], size = tam_muestra, replace = False)
muestra = matriz[idx_muestra]

for k in rango_k:
    print(f"Probando Agglomerative (Ward) con k = {k}")
    t0 = time.time()
    modelo = AgglomerativeClustering(
        n_clusters = k,
        linkage = "ward"
    )
    etiquetas_sub = modelo.fit_predict(muestra)
    t1 = time.time()

    res = evaluar_clustering(
        nombre = "agg_ward",
        etiquetas = etiquetas_sub,
        df_eval = muestra,
        tiempo = t1 - t0,
        info_extra = {"k": k, "muestra": tam_muestra}
    )
    resultados.append(res)

# ---------------------------------------------------------
# 11) Birch
# ---------------------------------------------------------
print("\n=== Birch ===")

for k in rango_k:
    print(f"Probando Birch con n_clusters = {k}")
    t0 = time.time()
    modelo = Birch(
        n_clusters = k,
        threshold = 0.5
    )
    etiquetas = modelo.fit_predict(matriz)
    t1 = time.time()

    res = evaluar_clustering(
        nombre = "birch",
        etiquetas = etiquetas,
        df_eval = matriz,
        tiempo = t1 - t0,
        info_extra = {"k": k, "threshold": 0.5}
    )
    resultados.append(res)

# ---------------------------------------------------------
# 12) DBSCAN
# ---------------------------------------------------------
print("\n=== DBSCAN ===")

param_eps = [0.2, 0.3, 0.4, 0.5]
param_min_samples = [20, 50]

for eps in param_eps:
    for ms in param_min_samples:
        print(f"Probando DBSCAN con eps = {eps}, min_samples = {ms}")
        t0 = time.time()
        modelo = DBSCAN(
            eps = eps,
            min_samples = ms,
            metric = "euclidean"
        )
        etiquetas = modelo.fit_predict(matriz)
        t1 = time.time()

        res = evaluar_clustering(
            nombre = "dbscan",
            etiquetas = etiquetas,
            df_eval = matriz,
            tiempo = t1 - t0,
            info_extra = {"eps": eps, "min_samples": ms}
        )
        resultados.append(res)

# ---------------------------------------------------------
# 13) MeanShift sobre muestra
# ---------------------------------------------------------
print("\n=== MeanShift sobre muestra ===")

muestra_ms = muestra
tam_muestra_ms = muestra_ms.shape[0]
cuantiles = [0.1, 0.2, 0.3]

for q in cuantiles:
    print(f"Probando MeanShift con quantile = {q}")
    t0 = time.time()
    bw = estimate_bandwidth(muestra_ms, quantile = q, n_samples = tam_muestra_ms)
    modelo = MeanShift(
        bandwidth = bw,
        bin_seeding = True
    )
    etiquetas_ms = modelo.fit_predict(muestra_ms)
    t1 = time.time()

    res = evaluar_clustering(
        nombre = "meanshift",
        etiquetas = etiquetas_ms,
        df_eval = muestra_ms,
        tiempo = t1 - t0,
        info_extra = {"quantile": q, "bandwidth": bw, "muestra": tam_muestra_ms}
    )
    resultados.append(res)

# ---------------------------------------------------------
# 14) Resumen global de resultados
# ---------------------------------------------------------
df_resultados = pd.DataFrame(resultados)
print("\nResumen global de modelos:")
print(df_resultados)

# Guardar resultados a CSV para usar en la memoria
df_resultados.to_csv("resultados_caso6_algoritmos.csv", index = False)