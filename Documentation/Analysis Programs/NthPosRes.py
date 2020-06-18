import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

df=pd.read_csv("PHOT_BaSO4.txt", sep=",", header=None, low_memory=False)
df.columns=['EventID', 'CopyNumber', 'XPos', 'YPos', 'ZPos', 'Time']

Det_limit = int(64)
Time_limit = int(5)
Pos_limit = int(0)

df = df[df['Time'] <= Time_limit]
Bottom  = df[df['XPos'] >= Pos_limit]
Top = df[df['XPos'] <= Pos_limit]

Bottom_Near = Bottom[Bottom['CopyNumber'] <= Det_limit]
Bottom_Far = Bottom[Bottom['CopyNumber'] >= Det_limit]
Top_Near = Top[Top['CopyNumber'] <= Det_limit]
Top_Far = Top[Top['CopyNumber'] >= Det_limit]

Bottom_Far.YPos = Bottom_Far.YPos/2.235
Bottom_Far.ZPos = Bottom_Far.ZPos/2.235

DATAY_10 = Bottom_Far.groupby('EventID').head(10).groupby('EventID').YPos.std().reset_index()
DATAY_20 = Bottom_Far.groupby('EventID').head(100).groupby('EventID').YPos.std().reset_index()
DATAY_30 = Bottom_Far.groupby('EventID').head(500).groupby('EventID').YPos.std().reset_index()
DATAY_50 = Bottom_Far.groupby('EventID').head(1000).groupby('EventID').YPos.std().reset_index()
DATAY_100 = Bottom_Far.groupby('EventID').head(2000).groupby('EventID').YPos.std().reset_index()

DATAZ_10 = Bottom_Far.groupby('EventID').head(10).groupby('EventID').ZPos.std().reset_index()
DATAZ_20 = Bottom_Far.groupby('EventID').head(20).groupby('EventID').ZPos.std().reset_index()
DATAZ_30 = Bottom_Far.groupby('EventID').head(30).groupby('EventID').ZPos.std().reset_index()
DATAZ_50 = Bottom_Far.groupby('EventID').head(50).groupby('EventID').ZPos.std().reset_index()
DATAZ_100 = Bottom_Far.groupby('EventID').head(100).groupby('EventID').ZPos.std().reset_index()

D1 = DATAZ_10.ZPos.dropna()
D2 = DATAZ_20.ZPos.dropna()
D3 = DATAZ_30.ZPos.dropna()
D4 = DATAZ_50.ZPos.dropna()
D5 = DATAZ_100.ZPos.dropna()

labels = ['10th photon', '20th photon', '30th photon', '50th photon', '100th photon']
colors = ['r','g', 'b', 'c', 'm']
fig = plt.hist([D1,D2,D3,D4,D5], bins = 400, density=False, histtype='bar', color=colors,label=labels,stacked=True)#bins = 1700 for Y,400 for Z
plt.xlabel("Position Resolution in mm")
plt.ylabel("Counts")
plt.title("Position Resolution in Z by nth photon BaSO4")
plt.xlim(0,2)
plt.ylim(0,4000)
plt.legend()
#plt.text(1,3800, "mean: 0.356")
#plt.text(1,3600, "std: 1.63")
#plt.text(1,3400, "count: 3404")
plt.show()
print(Bottom_Far.mean())
print(Bottom_Far.std())
print(D1.count())
