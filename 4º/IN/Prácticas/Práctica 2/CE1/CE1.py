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
