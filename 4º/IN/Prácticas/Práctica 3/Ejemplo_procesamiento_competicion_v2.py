import pandas as pd
import numpy as np
import seaborn as sns
from matplotlib import pyplot as plt
from sklearn.preprocessing import LabelEncoder, OrdinalEncoder
from sklearn.impute import SimpleImputer
from sklearn.model_selection import cross_val_score


train = pd.read_csv("train.csv", na_values="NaN", sep = ",") # Definimos na_values para identificar bien los valores perdidos
train.info()

train.columns()

marcas = train[train['brand'].str.contains('MINI|BMW|Nissan', regex = True) ]
sns.catplot(data=marcas, y="price", x='brand', kind="box")

if 'id' in train:
    train.drop('id', axis=1, inplace=True)

test = pd.read_csv("test.csv", na_values="NaN")
test_ids = test.id
test = test.drop('id', axis=1)

input_all = pd.concat([train.drop('price', axis=1), test])
input_all.columns

col_cat = list(input_all.select_dtypes(exclude=np.number).columns)

input_all[col_cat].head()

test.isnull().sum()
train.isnull().sum()

imputer_cat = SimpleImputer(strategy="most_frequent")
imputer_cat.fit(input_all[col_cat])
train[col_cat] = imputer_cat.transform(train[col_cat])
test[col_cat] = imputer_cat.transform(test[col_cat])

train.price.isnull().sum()

col_num = list(train.select_dtypes(include=np.number).columns)
col_num.remove('price')
imputer_num = SimpleImputer(strategy="median")
imputer_num.fit(input_all[col_num])
train[col_num] = imputer_num.transform(train[col_num])
test[col_num] = imputer_num.transform(test[col_num])

labelers = {}
test_l = test.copy()
train_l = train.copy()

for col in col_cat:
    labelers[col] = LabelEncoder().fit(input_all[col])
    test_l[col] = labelers[col].transform(test[col])
    train_l[col] = labelers[col].transform(train[col])

train_l.head()

assert((train_l.columns == train.columns).all())

y_train = train_l.price
X_train = train_l.drop('price', axis=1)

if 'id' in test_l:
    test_l.drop('id', axis=1, inplace=True)

X_test = test_l

from sklearn.tree import DecisionTreeRegressor
model = DecisionTreeRegressor()

sklearn.metrics.get_scorer_names()

values = cross_val_score(model, X_train, y_train, scoring='neg_root_mean_squared_error', cv=5)
print(values)
print(values.mean())

model.fit(X_train, y_train)

pred = model.predict(X_test)

salida = pd.DataFrame({'id': test_ids, 'price': pred})
salida.to_csv("Prueba2.csv", index=False)