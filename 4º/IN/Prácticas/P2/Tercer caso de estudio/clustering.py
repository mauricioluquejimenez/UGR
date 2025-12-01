# Mauricio Luque Jiménez
# Tercer caso de estudio: hogares con menores y carencias familiares

import time
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import seaborn as sns

from sklearn import metrics
from sklearn.cluster import KMeans

datos = pd.read_csv('ECV.csv',sep = ';', encoding = "iso-8859-1")