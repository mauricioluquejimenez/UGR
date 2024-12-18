import time
import os
import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import seaborn as sns

from scipy.cluster.hierarchy import dendrogram, linkage
from sklearn.cluster import KMeans, DBSCAN, Birch, MeanShift, AgglomerativeClustering
from sklearn import metrics
from sklearn.preprocessing import StandardScaler

pd.set_option('future.no_silent_downcasting', True)

def norm_to_zero_one(df):
    result = pd.DataFrame()
    for col in df.columns:
        if pd.api.types.is_numeric_dtype(df[col]):
            min_val = df[col].min()
            max_val = df[col].max()
            if min_val != max_val:
                result[col] = (df[col] - min_val) / (max_val - min_val)
            else:
                result[col] = 0
        else:
            result[col] = df[col]  # Mantener los datos no numéricos sin cambios
    return result

# Crear directorio de resultados si no existe
os.makedirs("CE2/resultados", exist_ok=True)

# Cargar datos
busquedas = pd.read_csv('booking_Granada_2024.csv', sep=';', encoding="iso-8859-1")

# Selección y limpieza de datos
busquedas = busquedas.rename(columns={"Price": "precio", "Distance": "distancia",
                                            "Rating": "valoracion", "Special": "mencion", "Review": "reseña", "Location": "localizacion",
                                            "Deal": "oferta"})

# Variables de interés
usadas = ['precio', 'distancia', 'valoracion', 'mencion', 'reseña','oferta']

busquedas_con_oferta = busquedas[busquedas['oferta'].notnull()]

busquedas_con_oferta = busquedas_con_oferta.replace({ "Oferta de finales de año": 1, "Oferta por tiempo limitado": 2, "Oferta de navidad": 3})
busquedas_con_oferta = busquedas_con_oferta.replace({ "normal": 1, "preferente": 2, "destacado": 3, "plus": 4}).fillna(0)
busquedas_con_oferta = busquedas_con_oferta[busquedas_con_oferta['oferta'].isin([1, 2, 3])]
print(busquedas_con_oferta) 

subset = busquedas_con_oferta[usadas]
print(subset)

busquedas_normalizadas = norm_to_zero_one(subset)
print(busquedas_normalizadas)

# Normalize and scale data
X_normalizado = norm_to_zero_one(subset)
scaler = StandardScaler()
X_escalado = scaler.fit_transform(subset)

# Define clustering algorithms
algoritmos = {
    'K-Means': KMeans(n_clusters=3, random_state=123),
    'Birch': Birch(n_clusters=3),
    'DBSCAN': DBSCAN(eps=0.5, min_samples=5),
    'Mean Shift': MeanShift(),
    'Jerárquico': AgglomerativeClustering(n_clusters=3)
}

resultados = {}
silhouette_scores = []

# Execute clustering algorithms
for nombre, modelo in algoritmos.items():
    print(f"\nEjecutando {nombre}...")
    t_ini = time.time()

    if nombre in ['DBSCAN', 'Mean Shift']:
        cluster_predict = modelo.fit_predict(X_escalado)
    else:
        cluster_predict = modelo.fit_predict(X_normalizado)

    tiempo = time.time() - t_ini
    print(f"Tiempo: {tiempo:.2f} segundos")

    if len(np.unique(cluster_predict)) > 1:
        metric_SC = metrics.silhouette_score(X_normalizado, cluster_predict)
        print(f"Silhouette Coefficient: {metric_SC:.5f}")
        metric_CH = metrics.calinski_harabasz_score(X_normalizado, cluster_predict)
        print(f"Calinski-Harabasz Index: {metric_CH:.2f}")
        silhouette_scores.append((nombre, metric_SC))
    else:
        print("Silhouette y Calinski-Harabasz no aplicables (1 cluster)")
        silhouette_scores.append((nombre, 0))

    resultados[nombre] = cluster_predict

# Generate visualizations
for nombre, clusters in resultados.items():
    if nombre == 'K-Means':
        subset['cluster'] = clusters
        sns.set()
        variables = usadas.copy()
        colors = sns.color_palette("husl", len(subset['cluster'].unique()))
        sns_plot = sns.pairplot(subset, vars=variables, hue="cluster", palette=colors, plot_kws={"s": 25}, diag_kind="hist")
        sns_plot.fig.subplots_adjust(wspace=.03, hspace=.03)
        sns_plot.fig.set_size_inches(15, 15)
        plt.suptitle(f"{nombre} - Scatter Matrix")
        plt.savefig(f"CE2/resultados/{nombre}_scatter_matrix.pdf")
        plt.close()
        subset.drop(columns=['cluster'], inplace=True)

    if nombre in ['K-Means', 'Birch']:
        centers = pd.DataFrame(KMeans(n_clusters=3).fit(X_normalizado).cluster_centers_, columns=usadas)
        plt.figure(figsize=(10, 6))
        sns.heatmap(centers, cmap="YlGnBu", annot=True, fmt='.3f')
        plt.title(f"{nombre} - Centroides")
        plt.savefig(f"CE2/resultados/{nombre}_centroides.pdf")
        plt.close()

    if nombre == 'Jerárquico':
        linked = linkage(X_normalizado, method='ward')
        plt.figure(figsize=(10, 7))
        dendrogram(linked, orientation='top', distance_sort='descending', show_leaf_counts=True)
        plt.title(f"{nombre} - Dendrograma")
        plt.xlabel("Índices de puntos")
        plt.ylabel("Distancia")
        plt.savefig(f"CE2/resultados/{nombre}_dendrograma.pdf")
        plt.close()