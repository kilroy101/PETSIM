import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

df=pd.read_csv("3x315.txt", sep=",", header=None, low_memory=False)
df.columns=['EventID', 'CopyNumber', 'XPos', 'YPos', 'ZPos', 'Time']

Det_limit = int(0)#change back to 64 for macro case
Time_limit = int(5)
Pos_limit = int(0)

#df = df[df['Time'] <= Time_limit]
Bottom  = df[df['XPos'] >= Pos_limit]
Top = df[df['XPos'] <= Pos_limit]

Bottom_Near = Bottom[Bottom['CopyNumber'] <= Det_limit]
Bottom_Far = Bottom[Bottom['CopyNumber'] >= Det_limit]
Top_Near = Top[Top['CopyNumber'] <= Det_limit]
Top_Far = Top[Top['CopyNumber'] >= Det_limit]

#DATA_P = Bottom_Far.EventID.value_counts()
#fig = DATA_P.hist(bins=30)

DATA_10 = Bottom_Far.groupby('EventID').nth(10).reset_index()
DATA_20 = Bottom_Far.groupby('EventID').nth(20).reset_index()
DATA_30 = Bottom_Far.groupby('EventID').nth(30).reset_index()
DATA_50 = Bottom_Far.groupby('EventID').nth(50).reset_index()
DATA_100 = Bottom_Far.groupby('EventID').nth(100).reset_index()

D1 = DATA_10.Time
D2 = DATA_20.Time
D3 = DATA_30.Time
D4 = DATA_50.Time
D5 = DATA_100.Time

labels = ['10th photon', '20th photon', '30th photon', '50th photon', '100th photon']
colors = ['r','g', 'b', 'c', 'm']
fig = plt.hist([D1,D2,D3,D4,D5], 40, density=False, histtype='bar', color=colors,label=labels,stacked=True)
plt.xlabel("Time in ns")
plt.ylabel("Counts")
plt.title("Time of nth Detected Photon BaSo4")
#plt.xlim(0,0.2)
plt.ylim(0,3500)
plt.legend()
plt.text(150,3300, "count: "+str(D1.count()))
plt.text(150,3100, "count: "+str(D2.count()))
plt.text(150,2900, "count: "+str(D3.count()))
plt.text(150,2700, "count: "+str(D4.count()))
plt.text(150,2500, "count: "+str(D5.count()))
plt.show()
