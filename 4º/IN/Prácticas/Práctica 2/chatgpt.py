import matplotlib.pyplot as plt
import pandas as pd
import seaborn as sns
import time

from sklearn.cluster import KMeans
from sklearn import metrics

# Función para normalizar los datos
def normalizacion(df):
    return (df - df.min()) / (df.max() - df.min())

# Carga de datos
data = pd.read_csv('booking_Granada_2024.csv', sep=';', encoding="iso-8859-1")

# Selección de columnas relevantes y renombrado
data = data.rename(columns={"Distance": "distancia", "Location": "localizacion", "Price": "precio"})

# Filtrar columnas necesarias para el análisis
usadas = ['distancia', 'precio']
subset = data[usadas].dropna()

# Normalización de las variables
x_normalizado = subset.apply(normalizacion)

print('----- Ejecutando K-Means basado en distancia a puntos de interés', end='')
k_means = KMeans(init='k-means++', n_clusters=4, n_init=5, random_state=123456)

t = time.time()
cluster = k_means.fit_predict(x_normalizado)
tiempo = time.time() - t
print(f": {tiempo:.2f} segundos, ", end='')

# Evaluación del clustering
metric_cluster = metrics.calinski_harabasz_score(x_normalizado, cluster)
print(f"Calinski-Harabasz Index: {metric_cluster:.3f}, ", end='')

# Resultados de los clusters
clusters = pd.DataFrame(cluster, index=x_normalizado.index, columns=['cluster'])
size = clusters['cluster'].value_counts().sort_index()

print("Tamaño de cada cluster:")
for i, c in enumerate(size):
    print(f'{i}: {c:5d} ({100 * c / len(clusters):5.2f}%)')

# Centroides desnormalizados
centers = pd.DataFrame(k_means.cluster_centers_, columns=usadas)
centers_desnormal = centers.copy()
for var in usadas:
    centers_desnormal[var] = subset[var].min() + centers[var] * (subset[var].max() - subset[var].min())

# Visualización de centroides
centers.index += 1
plt.figure()
hm = sns.heatmap(centers, cmap="YlGnBu", annot=centers_desnormal, fmt='.2f')
hm.set_title("Centroides basados en distancia y precio")
hm.figure.set_size_inches(10, 8)
plt.savefig("centroides_distancia_precio.pdf")
plt.show()

# Scatterplot para visualización de clusters
X_kmeans = pd.concat([subset, clusters], axis=1)
sns.set()
X_kmeans['cluster'] += 1
sns_plot = sns.scatterplot(data=X_kmeans, x='distancia', y='precio', hue='cluster', palette='Set2', s=50)
plt.title("Clustering por distancia y precio")
plt.xlabel("Distancia (normalizada)")
plt.ylabel("Precio (normalizado)")
plt.legend(title="Cluster")
plt.savefig("scatter_distancia_precio.pdf")
plt.show()