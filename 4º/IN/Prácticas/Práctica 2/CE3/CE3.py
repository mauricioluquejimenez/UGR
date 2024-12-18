import os
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
from sklearn.cluster import KMeans
from sklearn.preprocessing import StandardScaler
from datetime import datetime
from pandas.plotting import scatter_matrix

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

def definir_temporada(fecha):
    if fecha.month in [6, 7, 8, 12]:  # Verano y Navidad
        return 'alta'
    else:
        return 'baja'

# Crear directorio de resultados
os.makedirs("CE3/resultados", exist_ok=True)

# Cargar datos
alojamientos = pd.read_csv("booking_Granada_2024.csv", sep=';', encoding="iso-8859-1")

# Preprocesamiento de fechas
alojamientos['fecha_reserva'] = pd.to_datetime(alojamientos['Query date'], errors='coerce')
alojamientos['fecha_entrada'] = pd.to_datetime(alojamientos['Check in'], errors='coerce')
alojamientos['fecha_salida'] = pd.to_datetime(alojamientos['Check out'], errors='coerce')

# Eliminar filas con fechas no válidas
alojamientos = alojamientos.dropna(subset=['fecha_reserva', 'fecha_entrada', 'fecha_salida'])

alojamientos['temporada'] = alojamientos['fecha_reserva'].apply(definir_temporada)

# Variables a analizar
usadas = ['Price', 'Rating', 'Distance', 'Surface Area (m2)', 'Kitchens', 'Bedrooms']
alojamientos[usadas] = alojamientos[usadas].fillna(alojamientos[usadas].median())

# Separar datos en temporadas alta y baja
temporadas = alojamientos['temporada'].unique()

for temporada in temporadas:
    print(f"--- Análisis para temporada {temporada} ---")
    subset = alojamientos[alojamientos['temporada'] == temporada]
    
    if len(subset) < 10:
        print(f"Temporada {temporada} tiene pocos datos, se omite.")
        continue

    # Normalización de variables
    X_normalizado = norm_to_zero_one(subset[usadas])

    # Clustering (K-Means)
    print("Ejecutando K-Means...")
    kmeans = KMeans(n_clusters=3, random_state=123)
    cluster_predict = kmeans.fit_predict(X_normalizado)
    subset['cluster'] = cluster_predict

    # Visualización de resultados
    sns.scatterplot(data=subset, x='Distance', y='Price', hue='cluster', palette='Set2')
    plt.title(f"Clustering en temporada {temporada}")
    plt.xlabel("Distancia al centro")
    plt.ylabel("Precio")
    plt.legend(title="Cluster")
    plt.savefig(f"CE3/resultados/clustering_temporada_{temporada}.pdf")
    plt.close()

    # Mostrar distribución de clusters
    print("Distribución de clusters:")
    print(subset['cluster'].value_counts())

    # Análisis descriptivo por cluster
    print("Características promedio por cluster:")
    print(subset.groupby('cluster')[['Price', 'Rating', 'Distance', 'Surface Area (m2)', 'Kitchens', 'Bedrooms']].mean())

    # Gráfico de centroides
    centers = pd.DataFrame(kmeans.cluster_centers_, columns=usadas)
    plt.figure(figsize=(10, 6))
    sns.heatmap(centers, cmap="YlGnBu", annot=True, fmt='.2f')
    plt.title(f"Centroides - Temporada {temporada}")
    plt.savefig(f"CE3/resultados/centroides_temporada_{temporada}.pdf")
    plt.close()

print("\nAnálisis completado.")