#Kyle Klein
#Description: Inputs optical photon data taken from GEANT4 and shows timing resolution
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

df=pd.read_csv("3x315.txt", sep=",", header=None, low_memory=False)
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

DATA_T = Top_Far.groupby('EventID').nth(50).reset_index()
DATA_B = Bottom_Far.groupby('EventID').nth(50).reset_index()
Stuff = pd.merge(DATA_T,DATA_B, on='EventID')
DATA = pd.DataFrame()
DATA['EventID'] = Stuff["EventID"]
DATA["Time"] = Stuff["Time_x"]-Stuff["Time_y"]


df2=pd.read_csv("Edata15.txt", sep=",", header=None, low_memory=False)
df2.columns=['CP','EventID', 'Energy', 'Sec_x']
E_dataB = df2[df2['Sec_x'] >= 0]
E_dataCopyB = E_dataB.copy()

oofB = E_dataB.EventID.value_counts()
oofB = oofB.reset_index()
oofB.columns=['EventID', '# of processes']

P_typesB = E_dataB.groupby(['EventID', 'CP']).size().unstack(fill_value=0)
eid_cB = [] #eventID of only compton
eid_pB= [] #eventID of only phot
eid_ccB = [] #eventID of two comptons
eid_cpB = [] #eventID of both compton and phot
eid_otherB = []

for i in P_typesB.iterrows():
    if i[1]["compt"] ==1 and i[1]["phot"] ==1:
        # has both a compton and a phot
        eid_cpB.append(i[0])
    elif i[1]["compt"] == 1:
        # has 1 compton
        eid_cB.append(i[0])
    elif i[1]["phot"] == 1:
        # has 1 phot
        eid_pB.append(i[0])
    elif i[1]["compt"] == 2:
        # has two comptons
        eid_ccB.append(i[0])
    else:
        eid_otherB.append(i[0])

CPB = pd.DataFrame(eid_cpB)
CPB.columns = ['EventID']
CB = pd.DataFrame(eid_cB)
CB.columns = ['EventID']
PB = pd.DataFrame(eid_pB)
PB.columns = ['EventID']
CCB = pd.DataFrame(eid_ccB)
CCB.columns = ['EventID']
OB = pd.DataFrame(eid_otherB)
OB.columns = ['EventID']

E_dataT = df2[df2['Sec_x'] <= 0]
E_dataCopyT = E_dataT.copy()

oofT = E_dataT.EventID.value_counts()
oofT = oofT.reset_index()
oofT.columns=['EventID', '# of processes']

P_typesT = E_dataT.groupby(['EventID', 'CP']).size().unstack(fill_value=0)
eid_cT = [] #eventID of only compton
eid_pT= [] #eventID of only phot
eid_ccT = [] #eventID of two comptons
eid_cpT = [] #eventID of both compton and phot
eid_otherT = []

for i in P_typesT.iterrows():
    if i[1]["compt"] ==1 and i[1]["phot"] ==1:
        # has both a compton and a phot
        eid_cpT.append(i[0])
    elif i[1]["compt"] == 1:
        # has 1 compton
        eid_cT.append(i[0])
    elif i[1]["phot"] == 1:
        # has 1 phot
        eid_pT.append(i[0])
    elif i[1]["compt"] == 2:
        # has two comptons
        eid_ccT.append(i[0])
    else:
        eid_otherT.append(i[0])

CPT = pd.DataFrame(eid_cpT)
CPT.columns = ['EventID']
CT = pd.DataFrame(eid_cT)
CT.columns = ['EventID']
PT = pd.DataFrame(eid_pT)
PT.columns = ['EventID']
CCT = pd.DataFrame(eid_ccT)
CCT.columns = ['EventID']
OT = pd.DataFrame(eid_otherT)
OT.columns = ['EventID']

CP_D = pd.merge(CPT,DATA, on='EventID')
C_D = pd.merge(CT,DATA, on='EventID')
P_D = pd.merge(PT,DATA, on='EventID')
CC_D = pd.merge(CCT,DATA, on='EventID')
O_D = pd.merge(OT,DATA, on='EventID')

P_D = pd.merge(PB,P_D, on='EventID')
C_D = pd.merge(PB,C_D, on='EventID')
CP_D = pd.merge(PB,CP_D, on='EventID')
CC_D = pd.merge(PB,CC_D, on='EventID')
O_D = pd.merge(PB,O_D, on='EventID')

D1 = P_D.Time
D2 = CP_D.Time
D3 = C_D.Time
D4 = CC_D.Time
D5 = O_D.Time

labels = ['Phot', 'Compt-Phot','Compt','Compt-Compt', 'Other']
colors = ['r','g', 'b', 'c', 'm']
fig = plt.hist([D1,D2,D3,D4,D5], 50, density=False, histtype='bar', color=colors,label=labels,stacked=True)
#fig = plt.hist(D5, 50)
plt.legend()
plt.text(-4, 86, "Mean: 0.011")
plt.text( -4, 82,"Std: 1.78")
plt.text( -4, 78,"Count: 1734")
plt.title("Stacked histogram of Time Resolution with Bottom Phot and Top varying in Processes")
plt.xlabel("Time in ns")
plt.ylabel("Counts")
plt.xlim(-4.5,4.5)
plt.ylim(0,90)
plt.show()
print(DATA.Time.mean())
print(DATA.Time.std())
print(DATA.Time.count())
