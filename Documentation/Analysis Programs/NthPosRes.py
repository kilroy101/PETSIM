#Kyle Klein
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit
from scipy.stats import norm

df=pd.read_csv("3x315.txt", sep=",", header=None, low_memory=False)
df.columns=['EventID', 'CopyNumber', 'XPos', 'YPos', 'ZPos', 'Time']

Det_limit = int(0)
Time_limit = int(5)
Pos_limit = int(0)

#df = df[df['Time'] <= Time_limit]
Bottom  = df[df['XPos'] >= Pos_limit]
Top = df[df['XPos'] <= Pos_limit]

Bottom_Near = Bottom[Bottom['CopyNumber'] <= Det_limit]
Bottom_Far = Bottom[Bottom['CopyNumber'] >= Det_limit]
Top_Near = Top[Top['CopyNumber'] <= Det_limit]
Top_Far = Top[Top['CopyNumber'] >= Det_limit]

Bottom_Far.YPos = Bottom_Far.YPos/2.235
Bottom_Far.ZPos = Bottom_Far.ZPos/2.235

DATAY_10 = Bottom_Far.groupby('EventID').head(10).groupby('EventID').YPos.std().reset_index()
DATAY_20 = Bottom_Far.groupby('EventID').head(20).groupby('EventID').YPos.std().reset_index()
DATAY_30 = Bottom_Far.groupby('EventID').head(30).groupby('EventID').YPos.std().reset_index()
DATAY_50 = Bottom_Far.groupby('EventID').head(50).groupby('EventID').YPos.std().reset_index()
DATAY_100 = Bottom_Far.groupby('EventID').head(100).groupby('EventID').YPos.std().reset_index()

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

#fitting data to Gaussians

bins = np.linspace(0,D1.max(),78)
x = np.linspace(0,D1.max(),77)
y1 = D1.value_counts(bins=bins,sort=False).tolist()
y2 = D2.value_counts(bins=bins,sort=False).tolist()
y3 = D3.value_counts(bins=bins,sort=False).tolist()
y4 = D4.value_counts(bins=bins,sort=False).tolist()
y5 = D5.value_counts(bins=bins,sort=False).tolist()

def gauss(x,a,mu,sigma,c):
    y = a*np.exp(-(x - mu)**2 / (2 * sigma**2))+c
    return y

x1_p, x1_co = curve_fit(gauss,x,y1, p0=[80,0.4,0.1,0])
x2_p, x2_co = curve_fit(gauss,x,y2, p0=[80,0.4,0.1,0])
x3_p, x3_co = curve_fit(gauss,x,y3, p0=[80,0.4,0.1,0])
x4_p, x4_co = curve_fit(gauss,x,y4, p0=[80,0.4,0.1,0])
x5_p, x5_co = curve_fit(gauss,x,y5, p0=[80,0.4,0.1,0])

#2d stacked histogram
#labels = ['10th photon', '20th photon', '30th photon', '50th photon', '100th photon']
#colors = ['r','g', 'b', 'c', 'm']
#fig = plt.hist([D1,D2,D3,D4,D5], bins = 78, density=False, histtype='bar', color=colors,label=labels,stacked=True)#bins = 73 for BaSO4

#indivdual histograms and fits
fig1 = plt.figure()
fig2 = plt.figure()
fig3 = plt.figure()
fig4 = plt.figure()
fig5 = plt.figure()
ax1 = fig1.add_subplot(111,xlim=(0,0.6), ylim=(0,600),xlabel="Position Resolution in mm",ylabel="Counts",title="Position Resolution in Z of 10th Photon")
ax2 = fig2.add_subplot(111,xlim=(0,0.6), ylim=(0,600),xlabel="Position Resolution in mm",ylabel="Counts",title="Position Resolution in Z of 20th Photon")
ax3 = fig3.add_subplot(111,xlim=(0,0.6), ylim=(0,600),xlabel="Position Resolution in mm",ylabel="Counts",title="Position Resolution in Z of 30th Photon")
ax4 = fig4.add_subplot(111,xlim=(0,0.6), ylim=(0,600),xlabel="Position Resolution in mm",ylabel="Counts",title="Position Resolution in Z of 50th Photon")
ax5 = fig5.add_subplot(111,xlim=(0,0.6), ylim=(0,600),xlabel="Position Resolution in mm",ylabel="Counts",title="Position Resolution in Z of 100th Photon")
ax1.hist(D1, bins=78, color='r')
ax1.plot(x,gauss(x,*x1_p), color='r')
ax1.text(0.1,575,"mean: "+format(D1.mean(),"5.2f"))
ax1.text(0.1,550,"std: "+format(D1.std(),"5.2f"))
ax1.text(0.1,525,"count: "+str(D1.count()))
ax2.hist(D2, bins=78, color='g')
ax2.plot(x,gauss(x,*x2_p), color='g')
ax2.text(0.1,575,"mean: "+format(D2.mean(),"5.2f"))
ax2.text(0.1,550,"std: "+format(D2.std(),"5.2f"))
ax2.text(0.1,525,"count: "+str(D2.count()))
ax3.hist(D3, bins=78,color='b')
ax3.plot(x,gauss(x,*x3_p), color='b')
ax3.text(0.1,575,"mean: "+format(D3.mean(),"5.2f"))
ax3.text(0.1,550,"std: "+format(D3.std(),"5.2f"))
ax3.text(0.1,525,"count: "+str(D3.count()))
ax4.hist(D4, bins=78,color='c')
ax4.plot(x,gauss(x,*x4_p), color='c')
ax4.text(0.1,575,"mean: "+format(D4.mean(),"5.2f"))
ax4.text(0.1,550,"std: "+format(D4.std(),"5.2f"))
ax4.text(0.1,525,"count: "+str(D4.count()))
ax5.hist(D5, bins=78,color='m')
ax5.plot(x,gauss(x,*x5_p), color='m')
ax5.text(0.1,575,"mean: "+format(D5.mean(),"5.2f"))
ax5.text(0.1,550,"std: "+format(D5.std(),"5.2f"))
ax5.text(0.1,525,"count: "+str(D5.count()))

#2d stacked histogram constraints
#plt.xlabel("Position Resolution in mm")
#plt.ylabel("Counts")
#plt.title("Position Resolution in Y by nth detected photon Plastic")
#plt.xlim(0,0.6)
#plt.ylim(0,1800)
#plt.legend()
#plt.text(0.2,1685, "count: "+str(D1.count()))
#plt.text(0.2,1585, "count: "+str(D2.count()))
#plt.text(0.2,1485, "count: "+str(D3.count()))
#plt.text(0.2,1385, "count: "+str(D4.count()))
#plt.text(0.2,1285, "count: "+str(D5.count()))
plt.show()
