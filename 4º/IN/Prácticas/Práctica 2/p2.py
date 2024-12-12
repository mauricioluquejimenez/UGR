import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import seaborn as sns
import time

from math import floor
from sklearn import metrics
from sklearn.cluster import KMeans
from sklearn.impute import KNNImputer

def normalizacion(df):
    return (df - df.min()) * 1.0 / (df.max() - df.min())

data = pd.read_csv('booking_Granada_2024.csv', sep = ';', encoding = "iso-8859-1")

subset = data[data.Price > 1000]
subset = subset.rename(columns = {"Review" : "reseña", "Surface Area (m2)" : "superficie", "Guests" : "huespedes", "Average selected price" : "precio"})
usadas = ['reseña', 'superficie', 'huespedes', 'precio']

x = subset[usadas]
x_normalizado = x.apply(normalizacion)
print(x_normalizado)

print('----- Ejecutando K-Means', end = '')
k_means = KMeans(init = 'k-means++', n_clusters = 4, n_init = 5, random_state = 123456)

t = time.time()
cluster = k_means.fit_predict(x_normalizado)
tiempo = time.time() - t
print(": {:.2f} segundos, ".format(tiempo), end='')

metric_cluster = metrics.calinski_harabasz_score(x_normalizado, cluster)
print("Calinski-Harabasz Index: {:.3f}, ".format(metric_cluster), end = '')

clusters = pd.DataFrame(cluster, index = x.index, columns = ['cluster'])

print("Tamaño de cada cluster:")
size = clusters['cluster'].value_counts()
size = size.sort_index()

for i,c in enumerate(size):
   print('%s: %5d (%5.2f%%)' % (i,c,100*c/len(clusters)))
   
k = len(size)
colors = sns.color_palette(palette = 'Paired', n_colors = k, desat = None)

print("---------- Heatmap de centroides...")
centers = pd.DataFrame(k_means.cluster_centers_, columns = list(x))
centers_desnormal = centers.copy()
for var in list(centers):
    centers_desnormal[var] = x[var].min() + centers[var] * (x[var].max() - x[var].min())
    
centers.index += 1
plt.figure()
hm = sns.heatmap(centers, cmap = "YlGnBu", annot = centers_desnormal, annot_kws = {"fontsize":18}, fmt = '.3f')
hm.set_ylim(len(centers),0)
hm.figure.set_size_inches(15,15)
hm.figure.savefig("prueba1.pdf")
centers.index -= 1

X_kmeans = pd.concat([x, clusters], axis = 1)
print(X_kmeans)

print("---------- Scatter matrix...")
sns.set()
variables = list(X_kmeans)
variables.remove('cluster')
X_kmeans['cluster'] += 1
sns_plot = sns.pairplot(X_kmeans, vars = variables, hue = "cluster", palette = colors, plot_kws = {"s": 25}, diag_kind = "hist") #en hue indicamos que la columna 'cluster' define los colores
X_kmeans['cluster'] -= 1
print(X_kmeans)
sns_plot.fig.subplots_adjust(wspace = .03, hspace = .03)
sns_plot.fig.set_size_inches(15,15)
sns_plot.savefig("prueba2.pdf")
plt.show()