# CE1: algoritmo K-Means
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import seaborn as sns
import time

from math import floor
from sklearn import metrics
from sklearn.cluster import KMeans
from sklearn.cluster import DBSCAN
from sklearn.cluster import MeanShift
from sklearn.cluster import Birch
from sklearn.cluster import AgglomerativeClustering
from scipy.cluster.hierarchy import dendrogram, linkage

# Función para normalizar los datos
def normalizacion(df):
    return (df - df.min()) / (df.max() - df.min())

# Carga de datos
data = pd.read_csv('booking_Granada_2024.csv', sep = ';', encoding = "iso-8859-1")

# Selección de columnas relevantes y renombrado
data = data.rename(columns = {"Review": "reseña", "Surface Area (m2)": "superficie", 
                            "Guests": "huespedes", "Price": "precio", "Location": "localizacion"})

usadas = ['precio', 'reseña', 'huespedes', 'superficie']

# Dividir el dataset por localización
localizaciones = data['localizacion'].unique()

for loc in localizaciones:
    print(f"Analizando localización: {loc}")
    
    subset = data[data['localizacion'] == loc][usadas].dropna()
    if len(subset) < 10:  # Evitar análisis en subconjuntos pequeños
        print(f"Localización {loc} tiene pocos datos para clustering, se omite.")
        continue
    
    # Normalización de las variables
    x_normalizado = subset.apply(normalizacion)
    
    print(f'----- Ejecutando K-Means para {loc}', end = '')
    k_means = KMeans(init = 'k-means++', n_clusters = 3, n_init = 5, random_state = 123456)
    
    t_ini = time.time()
    cluster = k_means.fit_predict(x_normalizado)
    tiempo = time.time() - t_ini
    print(f": {tiempo:.2f} segundos, ", end = '')
    
    muestra_silhouette = 0.2 if (len(subset) > 10000) else 1.0
    metric_SC = metrics.silhouette_score(x_normalizado, cluster, metric = 'euclidean', sample_size = floor(muestra_silhouette * len(subset)), random_state = 123456)
    print("Silhouette Coefficient: {:.5f},  ".format(metric_SC), end = '')
    
    # Evaluación del clustering
    metric_cluster = metrics.calinski_harabasz_score(x_normalizado, cluster)
    print(f"Calinski-Harabasz Index: {metric_cluster:.3f}, ", end = '')
    
    # Resultados de los clusters
    clusters = pd.DataFrame(cluster, index = x_normalizado.index, columns = ['cluster'])
    size = clusters['cluster'].value_counts().sort_index()
    
    print("Tamaño de cada cluster:")
    for i, c in enumerate(size):
        print(f'{i}: {c:5d} ({100 * c / len(clusters):5.2f}%)')
    
    # Centroides desnormalizados
    centers = pd.DataFrame(k_means.cluster_centers_, columns = usadas)
    centers_desnormal = centers.copy()
    for var in usadas:
        centers_desnormal[var] = subset[var].min() + centers[var] * (subset[var].max() - subset[var].min())
    
    # Visualización de resultados
    centers.index += 1
    plt.figure()
    hm = sns.heatmap(centers, cmap = "YlGnBu", annot = centers_desnormal, fmt = '.2f')
    hm.set_title(f"Centroides para {loc}")
    hm.figure.set_size_inches(10, 8)
    plt.savefig(f"centroides/centroides_{loc}.pdf")
    plt.show()

    # Scatterplot para visualización de clusters
    X_kmeans = pd.concat([subset, clusters], axis = 1)
    sns.set()
    X_kmeans['cluster'] += 1
    sns_plot = sns.scatterplot(data = X_kmeans, x = 'precio', y = 'reseña', hue = 'cluster', palette = 'Set2', s = 50)
    plt.title(f"Scatterplot para {loc}")
    plt.xlabel("Precio")
    plt.ylabel("Reseña")
    plt.legend(title = "Cluster")
    plt.savefig(f"scatter/scatter_{loc}.pdf")
    plt.show()