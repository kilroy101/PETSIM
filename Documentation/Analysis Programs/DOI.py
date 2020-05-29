#Kyle Klein
#Description: Inputs optical photon, gamma, and electron data taken from GEANT4 and shows Depth of Interaction data correlated with photons detection data
#157.910
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

Top_Far = Top_Far.EventID.value_counts()
Top_Far = Top_Far.reset_index()
Top_Far.columns=['EventID', 'Far_Counts']
Top_Far["EventID2"] = Top_Far["EventID"]
Top_Near = Top_Near.EventID.value_counts()
Top_Near = Top_Near.reset_index()
Top_Near.columns=['EventID', 'Near_Counts']
Top_Near["EventID2"] = Top_Near["EventID"]

Bottom_Far = Bottom_Far.EventID.value_counts()
Bottom_Far = Bottom_Far.reset_index()
Bottom_Far.columns=['EventID', 'Far_Counts']
Bottom_Far["EventID1"] = Bottom_Far["EventID"]
Bottom_Near = Bottom_Near.EventID.value_counts()
Bottom_Near = Bottom_Near.reset_index()
Bottom_Near.columns=['EventID', 'Near_Counts']
Bottom_Near["EventID1"] = Bottom_Near["EventID"]

df1=pd.read_csv("Gamma_BaSO4.txt", sep=",", header=None, low_memory=False)
df1.columns=['EventID', 'Time', 'gammaxpos', 'gammaypos', 'gammazpos']
df1["EventID1"] = df1["EventID"]

df1.gammaxpos = df1.gammaxpos.str.replace('(', '')
df1.gammazpos = df1.gammazpos.str.replace(')', '')

df1.gammaxpos = df1.gammaxpos.astype(np.float64)
df1.gammazpos = df1.gammazpos.astype(np.float64)
Gamma_Data = df1[df1['gammaxpos'] >= int(0)]
Gamma_Data.gammaxpos = Gamma_Data.gammaxpos.subtract(164.5)
Gamma_Data.gammaxpos = Gamma_Data.gammaxpos.abs()
Gamma_DATA = Gamma_Data.groupby('EventID').last()

df2=pd.read_csv("Edata_BaSO4.txt", sep=",", header=None, low_memory=False)
df2.columns=['CP','EventID', 'Energy', 'Sec_x']
E_data = df2[df2['Sec_x'] >= 0]
E_dataCopy = E_data.copy()

oof = E_data.EventID.value_counts()
oof = oof.reset_index()
oof.columns=['EventID', '# of processes']

P_types = E_data.groupby(['EventID', 'CP']).size().unstack(fill_value=0)
eid_c = [] #eventID of only compton
eid_p= [] #eventID of only phot
eid_cc = [] #eventID of two comptons
eid_cp = [] #eventID of both compton and phot
eid_other = []

for i in P_types.iterrows():
    if i[1]["compt"] ==1 and i[1]["phot"] ==1:
        # has both a compton and a phot
        eid_cp.append(i[0])
    elif i[1]["compt"] == 1:
        # has 1 compton
        eid_c.append(i[0])
    elif i[1]["phot"] == 1:
        # has 1 phot
        eid_p.append(i[0])
    elif i[1]["compt"] == 2:
        # has two comptons
        eid_cc.append(i[0])
    else:
        eid_other.append(i[0])

CP = pd.DataFrame(eid_cp)
CP.columns = ['EventID']
C = pd.DataFrame(eid_c)
C.columns = ['EventID']
P = pd.DataFrame(eid_p)
P.columns = ['EventID']
CC = pd.DataFrame(eid_cc)
CC.columns = ['EventID']
O = pd.DataFrame(eid_other)
O.columns = ['EventID']

DATA = pd.merge(Bottom_Far,Gamma_DATA, on='EventID1')

CP_D = pd.merge(CP,DATA, on='EventID')
C_D = pd.merge(C,DATA, on='EventID')
P_D = pd.merge(P,DATA, on='EventID')
CC_D = pd.merge(CC,DATA, on='EventID')
O_D = pd.merge(O,DATA, on='EventID')

ax1 = CP_D.plot(kind='scatter', x='gammaxpos', y='Far_Counts', label = 'compt-phot',color='r')
ax2 = C_D.plot(kind='scatter', x='gammaxpos', y='Far_Counts', label = 'compt',color='g', ax=ax1)
ax2 = P_D.plot(kind='scatter', x='gammaxpos', y='Far_Counts', label = 'phot',color='b', ax=ax1)
ax2 = CC_D.plot(kind='scatter', x='gammaxpos', y='Far_Counts', label = 'compt-compt',color='c', ax=ax1)
ax2 = O_D.plot(kind='scatter', x='gammaxpos', y='Far_Counts', label = 'other',color='m', ax=ax1)
plt.xlim(0,16)
plt.ylim(0,8000)
plt.xlabel("Depth of Gamma Interaction in mm")
plt.ylabel("Far Detector Photon Count")
plt.title("DOI Plastic")
plt.text(8,7750,"mean: 7.82")
plt.text(8,7500,"std: 6.30")
plt.text(8,7250,"entries: 3402")
plt.grid()
plt.show()
print(DATA.gammaxpos.mean())
print(DATA.gammaxpos.std())
print(DATA.gammaxpos.count())
