# Mauricio Luque Jiménez
# Primer caso de estudio: ingresos recientes y expectativa económica

import time
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import seaborn as sns
from sklearn import metrics
from sklearn.cluster import KMeans
from sklearn.impute import KNNImputer

def normalizacion(df): return (df - df.min()) * 1.0 / (df.max() - df.min())

# 1) Lectura del fichero CSV
datos = pd.read_csv('ECV.csv', low_memory = False)

# 2) Selección de todas las instancias que tengan la columna HI010 rellenada
#    Uso de la variable HI010_F para filtrar si la columna Hi010 está rellena correctamente
subset = datos[datos["HI010_F"] == 1].copy()
print("Filas, columnas:", datos.shape)

# 3) Renombrar variables de interés para trabajar más cómodo
subset = subset.rename(columns = {
    "HY020": "renta_disp",
    "HY022": "renta_antes_trans_no_pensiones",
    "HY023": "renta_antes_todas_trans",
    "vhRentaa":"renta_equiv", "HS040": "vacaciones",
    "HS050": "comida_calidad",
    "HS060": "gasto_imprevisto",
    "HS120": "fin_de_mes",
    "HI040": "exp_ingresos"
})

# 4) Lista de variables que SÍ entran en el clustering
usadas = [
    "renta_disp",
    "renta_antes_trans_no_pensiones",
    "renta_antes_todas_trans",
    "renta_equiv", "vacaciones",
    "comida_calidad",
    "gasto_imprevisto",
    "fin_de_mes",
    "exp_ingresos"
]

# 5) Construir matriz de trabajo
df = subset[usadas].copy()
print(df.head())

# Convertir a numérico por si vienen como 'object'
for c in usadas:
    df[c] = pd.to_numeric(df[c], errors = "coerce")
    
# 6) Normalizar a [0,1] (como en el ejemplo)
df = df.apply(normalizacion)

print("Matriz df_normal:", df.shape)
print(df.head())

print("NaN por columna antes de limpiar:")
print(df.isna().sum())

# 7) Imputación KNN sobre la matriz normalizada
imputer = KNNImputer(n_neighbors = 5)
imputacion = imputer.fit_transform(df)

# 8) Volver a DataFrame con mismos índices y columnas
df = pd.DataFrame(imputacion, index = df.index, columns = df.columns)

print("Matriz df_completo:", df.shape)
print(df.head())

print("NaN por columna despues de limpiar:")
print(df.isna().sum())

# ============================
# 9) Clustering con K-Means
# ============================

# Matriz final para clustering: df (ya normalizado e imputado)
print("Matriz de clustering:", df.shape)

# Por si quieres usar después HI010, HI020, HI030, etc. para interpretar
subset_clust = subset.loc[df.index].copy()

# Probar varios valores de k
rango_k = range(2, 9)   # por ejemplo de 2 a 8 clusters
resultados_kmeans = []

for k in rango_k:
    print(f"Ejecutando K-Means con k = {k}...")
    t0 = time.time()
    modelo = KMeans(n_clusters = k, n_init = 10, random_state = 42)
    etiquetas = modelo.fit_predict(df)
    t1 = time.time()

    ch = metrics.calinski_harabasz_score(df, etiquetas)
    sil = metrics.silhouette_score(df, etiquetas)

    resultados_kmeans.append({
        "algoritmo": "kmeans",
        "k": k,
        "CH": ch,
        "Silhouette": sil,
        "tiempo": t1 - t0
    })

# Pasamos a DataFrame para verlo cómodo
res_kmeans = pd.DataFrame(resultados_kmeans)
print("\nResultados K-Means:")
print(res_kmeans)

k_elegido = res_kmeans.loc[res_kmeans["Silhouette"].idxmax(), "k"]

modelo_final = KMeans(n_clusters = int(k_elegido), n_init = 10, random_state = 42)
etiquetas_final = modelo_final.fit_predict(df)

subset_clust["cluster_kmeans"] = etiquetas_final
print("\nNúmero de hogares por cluster (K-Means):")
print(subset_clust["cluster_kmeans"].value_counts())

# Distribución de HI010 (cambio de ingresos) por cluster
print("\nDistribución HI010 por cluster (proporciones):")

for cl, df_cl in subset_clust.groupby("cluster_kmeans"):
    print(f"Cluster {cl}")
    print(
        df_cl["HI010"]
        .value_counts(normalize=True)
        .rename("proportion")
    )
    print("-" * 40)

# 10.2 Medias de las variables de clustering por grupo
#     (usamos df, que es numérico y normalizado)
vars_cluster = [
    "renta_disp",
    "renta_equiv",
    "vacaciones",
    "comida_calidad",
    "gasto_imprevisto",
    "fin_de_mes",
    "exp_ingresos"
]

df_clusters = df.copy()
df_clusters["cluster_kmeans"] = etiquetas_final

print("\nMedias de variables económicas por cluster (escala normalizada):")
print(
    df_clusters
      .groupby("cluster_kmeans")[vars_cluster]
      .mean()
)