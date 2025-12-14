# Mauricio Luque Jiménez
# Segundo caso de estudio: estructura familiar y bieniestar económico

import os
import time
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import seaborn as sns
from sklearn import metrics
from sklearn.cluster import KMeans, AgglomerativeClustering, Birch, DBSCAN, MeanShift, estimate_bandwidth
from sklearn.impute import KNNImputer
from sklearn.metrics import silhouette_samples
from sklearn.manifold import MDS
from scipy.cluster.hierarchy import linkage, dendrogram

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
# 2) Selección de segmento: Hogares según tipo y número de miembros
# ---------------------------------------------------------
datos["HX060"] = pd.to_numeric(datos["HX060"], errors = "coerce")

# Elegimos todos los posibles valores de tipo de hogar excluyendo los "otros" hogares (9, 14)
codigos = [1,2,3,4,5,6,7,8,10,11,12,13]
subset = datos[datos["HX060"].isin(codigos)].copy()
print("Filas, columnas:", subset.shape)

# ---------------------------------------------------------
# 3) Renombrar variables de interés para trabajar más cómodo
# ---------------------------------------------------------
subset = subset.rename(columns = {
    "HX040":   "n_miembros",
    "HX060":   "tipo_hogar",
    "vhRentaa": "renta_equiv",
    "HS120":  "fin_de_mes",
    "HS040":   "vacaciones",
    "HS050":   "comida_calidad",
    "HS060":   "gasto_imprevisto",
    "vhPobreza":  "pobreza",
    "vhMATDEP": "carencia_material"
})

subset["hay_menores"] = np.where(subset["tipo_hogar"].isin([10, 11, 12, 13, 14]), 1, 0)
subset["monoparental"] = np.where(subset["tipo_hogar"] == 10, 1, 0)
subset["pareja_hijos"] = np.where(subset["tipo_hogar"].isin([11, 12, 13]), 1, 0)
subset["unipersonal"] = np.where(subset["tipo_hogar"].isin(range(1, 7)), 1, 0)

# ---------------------------------------------------------
# 4) Lista de variables que entran en el clustering
# ---------------------------------------------------------
usadas = [
    # estructura familiar
    "n_miembros",
    "hay_menores",
    "monoparental",
    "pareja_hijos",
    "unipersonal",

    # bienestar económico
    "renta_equiv",
    "fin_de_mes",
    "gasto_imprevisto",
    "vacaciones",
    "comida_calidad",
    "pobreza",
    "carencia_material"
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

# Ruta donde se van a almacenar los resultados (para poder ejecutar en Windows y Linux)
directorio = "Segundo caso de estudio"
algoritmos = os.path.join(directorio, "algoritmos")
tablas = os.path.join(directorio, "tablas")
figuras = os.path.join(directorio, "figuras")

# Crear la subcarpeta 'algoritmos' si no existe
os.makedirs(algoritmos, exist_ok = True)
os.makedirs(tablas, exist_ok = True)
os.makedirs(figuras, exist_ok = True)

# Guardar resultados a CSV
df_resultados.to_csv(
    os.path.join(algoritmos, "resultados.csv"),
    index = False
)

# ---------------------------------------------------------
# 15) K-Means definitivo con k = 5
# ---------------------------------------------------------

kmeans_final = KMeans(
    n_clusters = 4,
    init = "k-means++",
    n_init = 10,
    random_state = 42
)
labels_final = kmeans_final.fit_predict(matriz)

# Guardar el cluster de K-Means en el subset para interpretación
subset_clust["cluster_kmeans"] = labels_final

# ---------------------------------------------------------
# 16) Scatter matrix (pairplot) con K-Means k=5
# ---------------------------------------------------------
print("\nGenerando scatter matrix (K-Means k=4)...")

vars_plot = ["renta_equiv", "fin_de_mes", "n_miembros"]

datos_plot = subset_clust[vars_plot + ["cluster_kmeans"]].copy()
datos_plot["cluster_kmeans"] = datos_plot["cluster_kmeans"].astype("category")

sns.pairplot(
    datos_plot,
    vars = vars_plot,
    hue = "cluster_kmeans",
    diag_kind = "hist",
    corner = True
)
plt.tight_layout()
plt.savefig(
    os.path.join(figuras, "scatter_matrix_kmeans.png"),
    dpi = 300
)
plt.close()

# ---------------------------------------------------------
# 17) Distribución del coeficiente silhouette por cluster
# ---------------------------------------------------------
print("Calculando coeficiente silhouette por cluster (K-Means k=4)...")

sil_values = silhouette_samples(matriz, subset_clust["cluster_kmeans"])
subset_clust["silhouette"] = sil_values

plt.figure(figsize = (8, 4))
sns.boxplot(
    data = subset_clust,
    x = "cluster_kmeans",
    y = "silhouette"
)
plt.xlabel("Cluster (K-Means k=4)")
plt.ylabel("Coeficiente silhouette")
plt.title("Distribución del coeficiente silhouette por cluster")
plt.tight_layout()
plt.savefig(
    os.path.join(figuras, "silhouette_kmeans.png"),
    dpi = 300
)
plt.close()

# Tabla con silhouette medio por cluster (por si la quieres en la memoria)
tabla_silhouette = (
    subset_clust.groupby("cluster_kmeans")["silhouette"]
    .agg(["mean", "median", "min", "max"])
)
tabla_silhouette.to_csv(
    os.path.join(tablas, "silhouette_kmeans.csv")
)

# ---------------------------------------------------------
# 18) Heatmap de centroides de K-Means (k=5)
# ---------------------------------------------------------
print("Generando heatmap de centroides (K-Means k=4)")

centroides = pd.DataFrame(
    kmeans_final.cluster_centers_,
    columns = usadas
)
centroides.index = [f"{i}" for i in range(4)]

plt.figure(figsize = (10, 4))
sns.heatmap(
    centroides,
    annot = True,
    fmt = ".2f",
    cmap = "viridis"
)
plt.title("Heatmap de centroides (K-Means k=4)")
plt.tight_layout()
plt.savefig(
    os.path.join(figuras, "centroides_kmeans.png"),
    dpi = 300
)
plt.close()

centroides.to_csv(
    os.path.join(tablas, "centroides_kmeans.csv")
)

# ---------------------------------------------------------
# 19) Gráfico de burbujas con MDS de centroides
# ---------------------------------------------------------
print("Generando gráfico de burbujas con MDS de centroides (K-Means k=4)...")

# Distancias euclídeas entre centroides
dist_centroides = metrics.pairwise_distances(centroides.values, metric = "euclidean")

mds = MDS(
    n_components = 2,
    dissimilarity = "precomputed",
    random_state = 42
)
coords = mds.fit_transform(dist_centroides)

# Tamaño de cada cluster (para las burbujas)
tam_clusters = subset_clust["cluster_kmeans"].value_counts().sort_index()
sizes = tam_clusters.values
labels_clusters = tam_clusters.index.values

plt.figure(figsize = (6, 6))
plt.scatter(
    coords[:, 0],
    coords[:, 1],
    s = sizes * 5,  # factor de escala para que se vean bien
    alpha = 0.7
)

for i, label in enumerate(labels_clusters):
    plt.text(
        coords[i, 0],
        coords[i, 1],
        f"C{label}",
        ha = "center",
        va = "center",
        fontsize = 10,
        color = "black"
    )

plt.xlabel("Dimensión 1 (MDS)")
plt.ylabel("Dimensión 2 (MDS)")
plt.title("MDS de centroides (tamaño según nº de hogares ")
plt.tight_layout()
plt.savefig(
    os.path.join(figuras, "centroides_mds.png"),
    dpi = 300
)
plt.close()

# ---------------------------------------------------------
# 20) Dendrograma (Agglomerative Ward sobre muestra)
# ---------------------------------------------------------
print("Generando dendrograma jerárquico (Ward) sobre muestra...")

Z = linkage(muestra, method = "ward")

plt.figure(figsize = (10, 5))
dendrogram(
    Z,
    truncate_mode = "level",  # mostramos solo los niveles superiores
    p = 5                      # número de niveles a mostrar
)
plt.title("Dendrograma jerárquico (Ward) sobre muestra ")
plt.xlabel("Observaciones agrupadas")
plt.ylabel("Distancia")
plt.tight_layout()
plt.savefig(
    os.path.join(figuras, "dendrograma.png"),
    dpi = 300
)
plt.close()