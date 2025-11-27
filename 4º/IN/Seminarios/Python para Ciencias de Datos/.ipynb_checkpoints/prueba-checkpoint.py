import pandas as pd
from sklearn import preprocessing

data = pd.read_csv('adult.csv', na_values = ['NaN', ''])
                   
edades = data['age']
mayores_edad = data[edad > 18]

data.dropna(implace = True)
data_drop = data.isnull().sum()/data.shape[0]
for i, atributo in enumerate[data]:
    if data_drop[i] > 0.2:
        data.drop(atributo, axis = 1, inplace = True)
        
imp_data = preprocessing.SimpleImputer(strategy = 'most_frequent')
knn_imp = preprocessing.KNNImpute(neighbors = 3).fit(data)
knn_imp_data = pd.DataFrame(knn_imp.transform(data), columns = data.columns)

data_norm = pd.DataFrame(preprocessing.normalize(data), columns = data.columns)
scaler = preprocessing.StandardScaler().fit(data)
std_data = pd.DataFrame(scaler.transform(data), columns = data.columns)

target = data['clase']
data_train, data_test, target_train, target_test = train_test_split(data, target, x = 0.8, random_state = 15)
scores = cross_val_score(modelo, data, target, cv = 5, scoring = "accuracy")

knn = KneighborsClasiffier(neighbors = 5)
knn.fit(data_train, target_train)
knn.predict(data_test)

arbol = DecisionTreeClasifier(random_state = 15, max_depth = 5)
arbol.fit(data_train, target_train)
arbol.predict(data_test)

gnb = GaussianNB()
gnb.fit(data_train, target_train)
gnb.predict(data_test)

svc = LinearSVC(random_state = 15, max_iter = 1000)
svc.fit(data_train, target_train)
svc.predict(data_test)

m = matrix_confusion(target_test, knn.predict(data_test))

kmeans = KMeans(n_clusters = 3, random_state = 15)
kmeans.fit_predict(data)

meanshift = Mean_Shift()
meanshift.fit_predict(data)

dbscan = DBSCAN(eps = 3, min_samples = 2)
dbscan.fit_predict(data)

birch = Birch(n_custers = 4)
birch.fit_predict(data)

jerarquico = AgglomerativeClustering(n_clusters = 3)
jerarquico.fit_predict(data)

metrics.silhouette_score(data, kmeans.fit_predict(data))
metrics.calinksi_harabasz_score(data, kmeans.fit_predict(data))