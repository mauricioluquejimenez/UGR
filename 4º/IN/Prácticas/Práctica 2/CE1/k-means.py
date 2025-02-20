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

# Función para normalizar los datos entre 0 y 1 evitando divisiones por cero
def norm_to_zero_one(df):
    result = pd.DataFrame()
    for col in df.columns:
        min_val = df[col].min()
        max_val = df[col].max()
        if min_val != max_val:
            result[col] = (df[col] - min_val) / (max_val - min_val)
        else:
            result[col] = 0
    return result

# Crear directorio de resultados si no existe
os.makedirs("CE1/resultados", exist_ok=True)

# Cargar datos
alojamientos = pd.read_csv('alojamientos_booking_Granada_2024.csv', sep=';', encoding="iso-8859-1")
busquedas = pd.read_csv('booking_Granada_2024.csv', sep=';', encoding="iso-8859-1")

# Selección y limpieza de datos
alojamientos = alojamientos.rename(columns={"Price difference": "precio", "Distance": "distancia",
                                             "Rating": "valoracion", "Location": "localizacion",
                                             "Kitchens": "cocinas", "Bedrooms": "habitaciones",
                                             "Living Rooms": "salones", "Bathrooms": "baños",
                                             "Surface Area (m2)": "superficie", "Individual Beds": "camas_individuales",
                                             "Double Beds": "camas_dobles", "Double Large Beds": "camas_grandes_dobles",
                                             "Bunk Beds": "literas"})

# Variables de interés para servicios de alojamiento
usadas = ['precio', 'valoracion', 'distancia', 'cocinas', 'habitaciones', 'salones', 'baños', 'superficie',
          'camas_individuales', 'camas_dobles', 'camas_grandes_dobles', 'literas']

# Preprocesamiento: rellenar valores perdidos con la mediana de cada columna
alojamientos[usadas] = alojamientos[usadas].fillna(alojamientos[usadas].median())

# Dividir los datos por localización 
localizaciones = alojamientos['localizacion'].unique()

for loc in localizaciones:
<<<<<<< Updated upstream:4º/IN/Prácticas/Práctica 2/CE1/CE1.py
    print(f"\n--- Análisis en la localización: {loc} ---")
    subset = alojamientos[alojamientos['localizacion'] == loc][usadas]

    if len(subset) < 10:
        print(f"Localización {loc} con pocos datos, se omite.")
        continue

    # Normalización de las variables
    X_normalizado = norm_to_zero_one(subset)

    # Escalado adicional para DBSCAN y Mean Shift
    scaler = StandardScaler()
    X_escalado = scaler.fit_transform(subset)

    # Algoritmos de clustering
    algoritmos = {
        'K-Means': KMeans(n_clusters=3, random_state=123),
        'Birch': Birch(n_clusters=3),
        'DBSCAN': DBSCAN(eps=0.5, min_samples=5),
        'Mean Shift': MeanShift(),
        'Jerárquico': AgglomerativeClustering(n_clusters=3)
    }

    resultados = {}
    silhouette_scores = []

    for nombre, modelo in algoritmos.items():
        print(f"\nEjecutando {nombre}...")
        t_ini = time.time()

        # DBSCAN y MeanShift requieren datos escalados
        if nombre in ['DBSCAN', 'Mean Shift']:
            cluster_predict = modelo.fit_predict(X_escalado)
        else:
            cluster_predict = modelo.fit_predict(X_normalizado)

        tiempo = time.time() - t_ini
        print(f"Tiempo: {tiempo:.2f} segundos")

        # Evaluación de los resultados
        if len(np.unique(cluster_predict)) > 1:
            metric_SC = metrics.silhouette_score(X_normalizado, cluster_predict)
            print(f"Silhouette Coefficient: {metric_SC:.5f}")
            metric_CH = metrics.calinski_harabasz_score(X_normalizado, cluster_predict)
            print(f"Calinski-Harabasz Index: {metric_CH:.2f}")
            silhouette_scores.append((nombre, metric_SC))
        else:
            print("Silhouette y Calinski-Harabasz no aplicables (1 cluster)")
            silhouette_scores.append((nombre, 0))

        # Almacenar resultados
        resultados[nombre] = cluster_predict

    # Visualización de resultados: Scatterplots y gráficos adicionales
    for nombre, clusters in resultados.items():
        # Scatter Matrix con diferenciación de colores pero sin incluir 'cluster' explícitamente
        if nombre == 'K-Means':
            subset['cluster'] = clusters
            sns.set()
            variables = usadas.copy()
            colors = sns.color_palette("husl", len(subset['cluster'].unique()))
            sns_plot = sns.pairplot(subset, vars=variables, hue="cluster", palette=colors, plot_kws={"s": 25}, diag_kind="hist")
            sns_plot.fig.subplots_adjust(wspace=.03, hspace=.03)
            sns_plot.fig.set_size_inches(15, 15)
            plt.suptitle(f"{nombre} - Scatter Matrix en {loc}")
            plt.savefig(f"CE1/resultados/{loc}_{nombre}_scatter_matrix.pdf")
            plt.close()
            subset.drop(columns=['cluster'], inplace=True)
        
        # Gráfico de centroides (K-Means y Birch)
        if nombre in ['K-Means', 'Birch']:
            centers = pd.DataFrame(KMeans(n_clusters=3).fit(X_normalizado).cluster_centers_, columns=usadas)
            plt.figure(figsize=(10, 6))
            sns.heatmap(centers, cmap="YlGnBu", annot=True, fmt='.3f')
            plt.title(f"{nombre} - Centroides en {loc}")
            plt.savefig(f"CE1/resultados/{loc}_{nombre}_centroides.pdf")
            plt.close()

        # Dendrograma para clustering jerárquico
        if nombre == 'Jerárquico':
            linked = linkage(X_normalizado, method='ward')
            plt.figure(figsize=(10, 7))
            dendrogram(linked, orientation='top', distance_sort='descending', show_leaf_counts=True)
            plt.title(f"{nombre} - Dendrograma en {loc}")
            plt.xlabel("Índices de puntos")
            plt.ylabel("Distancia")
            plt.savefig(f"CE1/resultados/{loc}_{nombre}_dendrograma.pdf")
            plt.close()

    print("-------------------------------------------------------------------------------")

print("\nAnálisis completado.")
=======
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
    plt.savefig(f"CE1/centroides/centroides_{loc}.pdf")
    # plt.show()

    # Scatterplot para visualización de clusters
    X_kmeans = pd.concat([subset, clusters], axis = 1)
    sns.set()
    X_kmeans['cluster'] += 1
    sns_plot = sns.scatterplot(data = X_kmeans, x = 'precio', y = 'reseña', hue = 'cluster', palette = 'Set2', s = 50)
    plt.title(f"Scatterplot para {loc}")
    plt.xlabel("Precio")
    plt.ylabel("Reseña")
    plt.legend(title = "Cluster")
    plt.savefig(f"CE1/scatter/scatter_{loc}.pdf")
    # plt.show()
>>>>>>> Stashed changes:4º/IN/Prácticas/Práctica 2/CE1/k-means.py
