#Kyle Klein
#Description: Inputs optical photon data taken from GEANT4 and displays the position resolution of each event

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit

df=pd.read_csv("3x315.txt", sep=",", header=None, low_memory=False)
df.columns=['EventID', 'CopyNumber', 'ZPos', 'YPos', 'XPos', 'Time']

DATA_100 = df.groupby('EventID').head(100)
DATA_200 = df.groupby('EventID').head(200)
DATA_500 = df.groupby('EventID').head(500)
DATA_1000 = df.groupby('EventID').head(1000)
DATA_2000 = df.groupby('EventID').head(2000)
DATA_ALL = df

C100 = DATA_100.XPos.count()
C200 = DATA_200.XPos.count()
C500 = DATA_500.XPos.count()
C1000 = DATA_1000.XPos.count()
C2000 = DATA_2000.XPos.count()
CALL = DATA_ALL.XPos.count()

D1 = DATA_100.XPos.dropna()
D2 = DATA_200.XPos.dropna()
D3 = DATA_500.XPos.dropna()
D4 = DATA_1000.XPos.dropna()
D5 = DATA_2000.XPos.dropna()
D6 = DATA_ALL.XPos.dropna()

#fitting data to Gaussians

bins = np.linspace(D1.min(),D1.max(),8)
x = np.linspace(D1.min(),D1.max(),7)
x_p = np.linspace(D1.min(),D1.max(),100)
y1 = D1.value_counts(bins=bins,sort=False).tolist()
y2 = D2.value_counts(bins=bins,sort=False).tolist()
y3 = D3.value_counts(bins=bins,sort=False).tolist()
y4 = D4.value_counts(bins=bins,sort=False).tolist()
y5 = D5.value_counts(bins=bins,sort=False).tolist()
y6 = D6.value_counts(bins=bins,sort=False).tolist()

def gauss(x,a,mu,sigma,c):
    y = a*np.exp(-(x - mu)**2 / (2 * sigma**2))+c
    return y

x1_p, x1_co = curve_fit(gauss,x,y1, p0=[1000000,0,3,0])
x2_p, x2_co = curve_fit(gauss,x,y2, p0=[1000000,0,3,0])
x3_p, x3_co = curve_fit(gauss,x,y3, p0=[1000000,0,3,0])
x4_p, x4_co = curve_fit(gauss,x,y4, p0=[1000000,0,3,0])
x5_p, x5_co = curve_fit(gauss,x,y5, p0=[1000000,0,3,0])
x6_p, x6_co = curve_fit(gauss,x,y6, p0=[1000000,0,3,0])

fig1 = plt.figure()
fig2 = plt.figure()
fig3 = plt.figure()
fig4 = plt.figure()
fig5 = plt.figure()
fig6 = plt.figure()
ax1 = fig1.add_subplot(111,xlim=(-12.9,12.9), ylim=(0,2300000),xlabel="Position in mm",ylabel="Counts",title="Position Resolution in X of 100th Photon")
ax2 = fig2.add_subplot(111,xlim=(-12.9,12.9), ylim=(0,2300000),xlabel="Position in mm",ylabel="Counts",title="Position Resolution in X of 200th Photon")
ax3 = fig3.add_subplot(111,xlim=(-12.9,12.9), ylim=(0,2300000),xlabel="Position in mm",ylabel="Counts",title="Position Resolution in X of 500th Photon")
ax4 = fig4.add_subplot(111,xlim=(-12.9,12.9), ylim=(0,2300000),xlabel="Position in mm",ylabel="Counts",title="Position Resolution in X of 1000th Photon")
ax5 = fig5.add_subplot(111,xlim=(-12.9,12.9), ylim=(0,2300000),xlabel="Position in mm",ylabel="Counts",title="Position Resolution in X of 2000th Photon")
ax6 = fig6.add_subplot(111,xlim=(-12.9,12.9), ylim=(0,2300000),xlabel="Position in mm",ylabel="Counts",title="Position Resolution in X of All Photons")

T_x = -12
T_y = [2200000, 2100000, 2000000,1900000,1800000,1700000,1600000,1500000]
ax1.hist(D1, bins=8, color='r')
ax1.plot(x_p,gauss(x_p,*x1_p), color='k')
ax1.text(-12,T_y[0],"Data Stats:")
ax1.text(-12,2100000,"mean: "+format(D1.mean(),"5.2f"))
ax1.text(-12,2000000,"std: "+format(D1.std(),"5.2f"))
ax1.text(-12,1900000,"count: "+str(C100))
ax1.text(-12,1800000,"Fit Paramters:")
ax1.text(-12,1700000,"mu: "+format(x1_p[1],"5.2f"))
ax1.text(-12,1600000,"sigma: "+format(x1_p[2],"5.2f"))
ax1.text(-12,1500000,"A: "+format(x1_p[0],"5.2f"))

ax2.hist(D2, bins=8, color='g')
ax2.plot(x_p,gauss(x_p,*x2_p), color='k')
ax2.text(-12,T_y[0],"Data Stats:")
ax2.text(-12,T_y[1],"mean: "+format(D2.mean(),"5.2f"))
ax2.text(-12,T_y[2],"std: "+format(D2.std(),"5.2f"))
ax2.text(-12,T_y[3],"count: "+str(C200))
ax2.text(-12,T_y[4],"Fit Paramters:")
ax2.text(-12,T_y[5],"mu: "+format(x2_p[1],"5.2f"))
ax2.text(-12,T_y[6],"sigma: "+format(x2_p[2],"5.2f"))
ax2.text(-12,T_y[7],"A: "+format(x2_p[0],"5.2f"))

ax3.hist(D3, bins=8,color='b')
ax3.plot(x_p,gauss(x_p,*x3_p), color='k')
ax3.text(-12,T_y[0],"Data Stats:")
ax3.text(-12,T_y[1],"mean: "+format(D3.mean(),"5.2f"))
ax3.text(-12,T_y[2],"std: "+format(D3.std(),"5.2f"))
ax3.text(-12,T_y[3],"count: "+str(C500))
ax3.text(-12,T_y[4],"Fit Paramters:")
ax3.text(-12,T_y[5],"mu: "+format(x3_p[1],"5.2f"))
ax3.text(-12,T_y[6],"sigma: "+format(x3_p[2],"5.2f"))
ax3.text(-12,T_y[7],"A: "+format(x3_p[0],"5.2f"))

ax4.hist(D4, bins=8,color='c')
ax4.plot(x_p,gauss(x_p,*x4_p), color='k')
ax4.text(-12,T_y[0],"Data Stats:")
ax4.text(-12,T_y[1],"mean: "+format(D4.mean(),"5.2f"))
ax4.text(-12,T_y[2],"std: "+format(D4.std(),"5.2f"))
ax4.text(-12,T_y[3],"count: "+str(C1000))
ax4.text(-12,T_y[4],"Fit Paramters:")
ax4.text(-12,T_y[5],"mu: "+format(x4_p[1],"5.2f"))
ax4.text(-12,T_y[6],"sigma: "+format(x4_p[2],"5.2f"))
ax4.text(-12,T_y[7],"A: "+format(x4_p[0],"5.2f"))

ax5.hist(D5, bins=8,color='m')
ax5.plot(x_p,gauss(x_p,*x5_p), color='k')
ax5.text(-12,T_y[0],"Data Stats:")
ax5.text(-12,T_y[1],"mean: "+format(D5.mean(),"5.2f"))
ax5.text(-12,T_y[2],"std: "+format(D5.std(),"5.2f"))
ax5.text(-12,T_y[3],"count: "+str(C2000))
ax5.text(-12,T_y[4],"Fit Paramters:")
ax5.text(-12,T_y[5],"mu: "+format(x5_p[1],"5.2f"))
ax5.text(-12,T_y[6],"sigma: "+format(x5_p[2],"5.2f"))
ax5.text(-12,T_y[7],"A: "+format(x5_p[0],"5.2f"))

ax6.hist(D6, bins=8,color='tab:brown')
ax6.plot(x_p,gauss(x_p,*x6_p), color='k')
ax6.text(-12,T_y[0],"Data Stats:")
ax6.text(-12,T_y[1],"mean: "+format(D6.mean(),"5.2f"))
ax6.text(-12,T_y[2],"std: "+format(D6.std(),"5.2f"))
ax6.text(-12,T_y[3],"count: "+str(CALL))
ax6.text(-12,T_y[4],"Fit Paramters:")
ax6.text(-12,T_y[5],"mu: "+format(x6_p[1],"5.2f"))
ax6.text(-12,T_y[6],"sigma: "+format(x6_p[2],"5.2f"))
ax6.text(-12,T_y[7],"A: "+format(x6_p[0],"5.2f"))

plt.show()
