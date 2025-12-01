# Mauricio Luque Jiménez
# Primer caso de estudio: ingresos recientes y expectativa económica

import time
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import seaborn as sns

from sklearn import metrics
from sklearn.cluster import KMeans

def normalizacion(df):
    return (df - df.min()) * 1.0 / (df.max() - df.min())

# Lectura del fichero CSV
datos = pd.read_csv('ECV.csv', low_memory = False)

# Selección de todas las instancias que tengan la columna HI010 rellenada
# Uso de la variable HI010_F para filtrar si la columna Hi010 está rellena correctamente
subset = datos[datos["HI010_F"] == 1].copy()

print("Filas, columnas:", datos.shape)

# 3) Renombrar variables de interés para trabajar más cómodo
subset = subset.rename(columns = {
    "HY020":   "renta_disp",
    "HY022":   "renta_antes_trans_no_pensiones",
    "HY023":   "renta_antes_todas_trans",
    "vhRentaa":"renta_equiv",
    "HS040":   "vacaciones",
    "HS050":   "comida_calidad",
    "HS060":   "gasto_imprevisto",
    "HS120":   "fin_de_mes",
    "HI040":   "exp_ingresos"
})

# 4) Lista de variables que SÍ entran en el clustering
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




# 5) Construir matriz de trabajo
matriz = subset[usadas].copy()

# Convertir a numérico por si vienen como 'object'
# for c in usadas:
#     matriz[c] = pd.to_numeric(matriz[c], errors="coerce")

# Puedes decidir aquí cómo tratar los NaN.
# De momento, opción sencilla: imputación KNN o borrado de filas incompletas.

# df = pd.DataFrame(columns=usadas, index=matriz.index)

# 6) Normalizar a [0,1] (como en el ejemplo)
# df_normal = df.apply(normalizacion)

# print("Matriz df_normal:", df_normal.shape)
# print(df_normal.head())