#Kyle Klein
#Description: Inputs optical photon, gamma, and electron data taken from GEANT4 and shows Depth of Interaction data correlated with photons detection data
#142.495
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from scipy.optimize import curve_fit

dir = "~/path/Geant4/Analysis Data/Electron_Test/"
df=pd.read_csv(dir+"3x315.txt", sep=",", header=None, usecols=[0,1,2,3,4,5],low_memory=False)
df.columns=['EventID', 'CopyNumber', 'XPos', 'YPos', 'ZPos', 'Time']

Det_limit = int(128)
Time_limit = int(5)
Pos_limit = int(0)

#df = df[df['Time'] <= Time_limit]
Bottom  = df[df['XPos'] >= Pos_limit]
Top = df[df['XPos'] <= Pos_limit]

Bottom_Near = Bottom[Bottom['CopyNumber'] < Det_limit]
Bottom_Far = Bottom[Bottom['CopyNumber'] < Det_limit]
Top_Near = Top[Top['CopyNumber'] <= Det_limit]
Top_Far = Top[Top['CopyNumber'] >= Det_limit]

# Top_Far = Top_Far.EventID.value_counts()
# Top_Far = Top_Far.reset_index()
# Top_Far.columns=['EventID', 'Far_Counts']
# Top_Far["EventID2"] = Top_Far["EventID"]
# Top_Near = Top_Near.EventID.value_counts()
# Top_Near = Top_Near.reset_index()
# Top_Near.columns=['EventID', 'Near_Counts']
# Top_Near["EventID2"] = Top_Near["EventID"]

Bottom_Far = Bottom_Far.EventID.value_counts()
Bottom_Far = Bottom_Far.reset_index()
Bottom_Far.columns=['EventID', 'Far_Counts']
Bottom_Far["EventID1"] = Bottom_Far["EventID"]
Bottom_Near = Bottom_Near.EventID.value_counts()
Bottom_Near = Bottom_Near.reset_index()
Bottom_Near.columns=['EventID', 'Near_Counts']
Bottom_Near["EventID1"] = Bottom_Near["EventID"]


# Bottom_Far = pd.merge(Bottom_Far, Bottom_Near, on='EventID')
#~/path/Geant4/Analysis Data/Proposal_YAP_Sept_2020/
df1=pd.read_csv(dir+"Gamma3x315.txt", sep=",", header=None,usecols=[0,1,2,3,4],low_memory=False)
df1.columns=['EventID', 'Time', 'gammazpos', 'gammaypos', 'gammaxpos']
df1.gammazpos = df1.gammazpos.str.replace('(', '')
df1.gammaxpos = df1.gammaxpos.str.replace(')', '')

df1.gammazpos = df1.gammazpos.astype(np.float64)
df1.gammaxpos = df1.gammaxpos.astype(np.float64)

# entry_p = df1.gammaxpos[0]
Gamma_Data = df1[df1['gammazpos'] >= int(0)]
Gamma_Data.gammazpos = Gamma_Data.gammazpos.subtract(20)
Gamma_Data = Gamma_Data[Gamma_Data["gammaxpos"] == 0].reset_index()
Gamma_Data = Gamma_Data[Gamma_Data["gammaypos"] == 0].reset_index()
Gamma_DATA = Gamma_Data.groupby('EventID').last().reset_index()
Gamma_DATA = Gamma_DATA.drop(columns=["level_0", "index"])
Gamma_DATA.columns = ['EventID', 'Time', 'gammazpos', 'gammaypos', 'gammaxpos']

df2=pd.read_csv(dir+"Edata15.txt", sep=",", header=None, low_memory=False)
df2.columns=['CP','EventID', 'Energy', 'Sec_x', 'TrackID']
df2 = df2[df2['Sec_x'] >= 0]
df2 = df2.groupby(["EventID", "TrackID"])
E_DATA = pd.DataFrame(columns=["CP","EventID","Energy","Sec_x","TrackID"])

#itrates through all eletrons and takes their first step in their track rather than assuming a trackID as before. More accurate but less efficient.
row = 1
for group in df2.groups:
    E_DATA.loc[row] = df2.get_group(group).iloc[0]
    row+=1

df2 = E_DATA.sort_values(by = "EventID")
df2 = df2.copy()
oof = df2.EventID.value_counts()
oof = oof.reset_index()
oof.columns=['EventID', '# of processes']

P_types = df2.groupby(['EventID', 'CP']).size().unstack(fill_value=0)
eid_c = [] #eventID of only compton
eid_p= [] #eventID of only phot
eid_cc = [] #eventID of two comptons
eid_cp = [] #eventID of both compton and phot
eid_other = []

for i in P_types.iterrows():
    if i[1]["compt"] ==1 and i[1]["phot"] ==1:
        # has both a compton and a phot
        eid_cp.append(i[0])
    elif i[1]["compt"] == 1 and i[1]["phot"] == 0:
        # has 1 compton
        eid_c.append(i[0])
    elif i[1]["phot"] == 1 and i[1]["compt"] == 0:
        # has 1 phot
        eid_p.append(i[0])
    elif i[1]["compt"] == 2 and i[1]["phot"] == 0:
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


DATA = pd.merge(Bottom_Far,Gamma_DATA, on='EventID')

#hist for cutting at photopeak
# plt.hist(DATA.Far_Counts, bins=25)
# plt.show()

Fit_D = DATA[DATA["Far_Counts"] >= 2500]

def line(x,m,b):
    y = m*x + b
    return y
def inv12(x,a,b,c):
    y = a/(x-b)**12 + c
    return y

def ChiSquare(data,fit):
    chi = 0
    for i in range(len(data)):
        err = (data[i]-fit[i])**2 / fit[i]
        chi += err
    return chi

f_p, f_co = curve_fit(line,Fit_D.gammazpos,Fit_D.Far_Counts, p0=[6.5,3754], bounds=[[0,2000], [20,4000]])
#1000000000000000000000000000000000000000000000,50,1300


CP_D = pd.merge(CP,DATA, on='EventID')
C_D = pd.merge(C,DATA, on='EventID')
P_D = pd.merge(P,DATA, on='EventID')
CC_D = pd.merge(CC,DATA, on='EventID')
O_D = pd.merge(O,DATA, on='EventID')


#3d plots
# fig1 = plt.figure()
# fig2 = plt.figure()
# fig3 = plt.figure()
# fig4 = plt.figure()
#
# ax1 = fig1.add_subplot(111,projection='3d')
# ax2 = fig2.add_subplot(111,projection='3d')
# ax3 = fig3.add_subplot(111,projection='3d')
# ax4 = fig4.add_subplot(111,projection='3d')
#
# hist1, xedges1, yedges1 = np.histogram2d(CP_D.gammaxpos,CP_D.Far_Counts, bins = 30, range=[[0,16],[0,8000]])
# hist2, xedges2, yedges2 = np.histogram2d(C_D.gammaxpos,C_D.Far_Counts, bins = 30, range=[[0,16],[0,8000]])
# hist3, xedges3, yedges3 = np.histogram2d(P_D.gammaxpos,P_D.Far_Counts, bins = 30, range=[[0,16],[0,8000]])
# hist4, xedges4, yedges4 = np.histogram2d(CC_D.gammaxpos,CC_D.Far_Counts, bins = 30, range=[[0,16],[0,8000]])
# hist5, xedges5, yedges5 = np.histogram2d(O_D.gammaxpos,O_D.Far_Counts, bins = 30, range=[[0,16],[0,8000]])
# xpos1, ypos1 = np.meshgrid(xedges1[:-1]+0.25,yedges1[:-1]+0.25,indexing="ij")
# xpos2, ypos2 = np.meshgrid(xedges2[:-1]+0.25,yedges2[:-1]+0.25,indexing="ij")
# xpos3, ypos3 = np.meshgrid(xedges3[:-1]+0.25,yedges3[:-1]+0.25,indexing="ij")
# xpos4, ypos4 = np.meshgrid(xedges4[:-1]+0.25,yedges4[:-1]+0.25,indexing="ij")
# xpos5, ypos5 = np.meshgrid(xedges5[:-1]+0.25,yedges5[:-1]+0.25,indexing="ij")
# xpos1 = xpos1.ravel()
# ypos1 = ypos1.ravel()
# xpos2 = xpos2.ravel()
# ypos2 = ypos2.ravel()
# xpos3 = xpos3.ravel()
# ypos3 = ypos3.ravel()
# xpos4 = xpos4.ravel()
# ypos4 = ypos4.ravel()
# xpos5 = xpos5.ravel()
# ypos5 = ypos5.ravel()
# zpos = 0
# dx = 0.5* np.ones_like(zpos)
# dy = 200* np.ones_like(zpos)
# dz1 = hist1.ravel()
# dz2 = hist2.ravel()
# dz3 = hist3.ravel()
# dz4 = hist4.ravel()
# dz5 = hist5.ravel()
# ax1.bar3d(xpos1,ypos1,zpos,dx,dy,dz1)
# ax1.set_title("Compt-Phot: Plastic")
# ax1.set_xlabel("Depth of Gamma Interaction in mm")
# ax1.set_ylabel("Far Detector Photon Count")
# ax1.set_zlabel("Counts")
#
# ax2.bar3d(xpos2,ypos2,zpos,dx,dy,dz2)
# ax2.set_title("Compt: Plastic")
# ax2.set_xlabel("Depth of Gamma Interaction in mm")
# ax2.set_ylabel("Far Detector Photon Count")
# ax2.set_zlabel("Counts")
#
# ax3.bar3d(xpos3,ypos3,zpos,dx,dy,dz3)
# ax3.set_title("Phot: Plastic")
# ax3.set_xlabel("Depth of Gamma Interaction in mm")
# ax3.set_ylabel("Far Detector Photon Count")
# ax3.set_zlabel("Counts")
#
# ax4.bar3d(xpos4,ypos4,zpos,dx,dy,dz4)
# ax4.set_title("Compt-Compt: Plastic")
# ax4.set_xlabel("Depth of Gamma Interaction in mm")
# ax4.set_ylabel("Far Detector Photon Count")
# ax4.set_zlabel("Counts")

yy = np.repeat(3600, 150)
#2d scatter plots
ax1 = CP_D.plot(kind='scatter', x='gammazpos', y='Far_Counts', label = 'compt-phot',color='r')
ax2 = C_D.plot(kind='scatter', x='gammazpos', y='Far_Counts', label = 'compt',color='g', ax=ax1)
ax3 = P_D.plot(kind='scatter', x='gammazpos', y='Far_Counts', label = 'phot',color='b',ax=ax1)
ax4 = CC_D.plot(kind='scatter', x='gammazpos', y='Far_Counts', label = 'compt-compt',color='c', ax=ax1)
ax5 = O_D.plot(kind='scatter', x='gammazpos', y='Far_Counts', label = 'other',color='m', ax=ax1)
#2d scatter plot constraints
x_p = np.linspace(0,15, 150)
plt.xlim(0,16)
plt.ylim(0,6000)#10 for ratio
plt.plot(x_p, line(x_p,*f_p), color='k')
plt.xlabel("Depth of Gamma Interaction in mm")
plt.ylabel("Detector Photon Count")
plt.title("LYSO DOI")
plt.text(0.1,5800, "Data Parameters:")
plt.text(0.1,5650,"mean: "+format(DATA.gammazpos.mean(),"4.2f"))#7750 decrease by 250
plt.text(0.1,5500,"std: "+format(DATA.gammazpos.std(),"4.2f"))#x pos of 2.5 for far and ratio, 12 for near
plt.text(0.1,5350,"entries: "+format(DATA.gammazpos.count(),"4"))
plt.text(2.1,5800, "Fit Parameters:")
plt.text(2.1,5650,"slope: "+format(f_p[0],"4.2f"))#7750 decrease by 250, format(f_p[0],"4.2f")
plt.text(2.1,5500,"intercept: "+format(f_p[1],"4.2f"))#x pos of 2.5 for far and ratio, 12 for near
# plt.text(2.1,5350,"y-shift: "+format(f_p[2],"4.2f"))
# plt.text(2.1,5350,"Chi Square: "+format(ChiSquare(Fit_D.Far_Counts, line(Fit_D.gammaxpos, *f_p)),"9.2f"))
plt.grid()
plt.show()
