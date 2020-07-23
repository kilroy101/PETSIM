import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit

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
DATA_500 = Bottom_Far.groupby('EventID').nth(500).reset_index()
DATA_1000 = Bottom_Far.groupby('EventID').nth(1000).reset_index()
DATA_2000 = Bottom_Far.groupby('EventID').nth(2000).reset_index()

D1 = DATA_10.Time
D2 = DATA_20.Time
D3 = DATA_30.Time
D4 = DATA_50.Time
D5 = DATA_100.Time
D6 = DATA_500.Time
D7 = DATA_1000.Time
D8 = DATA_2000.Time

#fitting data to Gaussians

bins = np.linspace(0,D1.max(),78)
x = np.linspace(0,D1.max(),77)
y1 = D1.value_counts(bins=bins,sort=False).tolist()
y2 = D2.value_counts(bins=bins,sort=False).tolist()
y3 = D3.value_counts(bins=bins,sort=False).tolist()
y4 = D4.value_counts(bins=bins,sort=False).tolist()
y5 = D5.value_counts(bins=bins,sort=False).tolist()
y6 = D6.value_counts(bins=bins,sort=False).tolist()
y7 = D7.value_counts(bins=bins,sort=False).tolist()
y8 = D8.value_counts(bins=bins,sort=False).tolist()

def gauss(x,a,mu,sigma,c):
    y = a*np.exp(-(x - mu)**2 / (2 * sigma**2))+c
    return y

x1_p, x1_co = curve_fit(gauss,x,y1, p0=[100,2,0.1,0])
x2_p, x2_co = curve_fit(gauss,x,y2, p0=[100,2,0.1,0])
x3_p, x3_co = curve_fit(gauss,x,y3, p0=[100,2,0.1,0])
x4_p, x4_co = curve_fit(gauss,x,y4, p0=[100,2,0.1,0])
x5_p, x5_co = curve_fit(gauss,x,y5, p0=[50,2,0.15,0])
x6_p, x6_co = curve_fit(gauss,x,y6, p0=[50,2,0.15,0])
x7_p, x7_co = curve_fit(gauss,x,y7, p0=[50,2,0.15,0])
x8_p, x8_co = curve_fit(gauss,x,y8, p0=[50,2,0.15,0])

# # #indivdual histograms and fits
# fig1 = plt.figure()
# fig2 = plt.figure()
# fig3 = plt.figure()
# fig4 = plt.figure()
# fig5 = plt.figure()
# fig6 = plt.figure()
# fig7 = plt.figure()
# fig8 = plt.figure()
# ax1 = fig1.add_subplot(111,xlim=(0,3), ylim=(0,1200),xlabel="Time in ns" ,ylabel="Counts",title="Time of 10th Photon")
# ax2 = fig2.add_subplot(111,xlim=(0,3), ylim=(0,1200),xlabel="Time in ns",ylabel="Counts",title="Time of 20th Photon")
# ax3 = fig3.add_subplot(111,xlim=(0,3), ylim=(0,1200),xlabel="Time in ns",ylabel="Counts",title="Time of 30th Photon")
# ax4 = fig4.add_subplot(111,xlim=(0,3), ylim=(0,1200),xlabel="Time in ns",ylabel="Counts",title="Time of 50th Photon")
# ax5 = fig5.add_subplot(111,xlim=(0,3), ylim=(0,1200),xlabel="Time in ns",ylabel="Counts",title="Time of 100th Photon")
# ax6 = fig6.add_subplot(111,xlim=(0,3), ylim=(0,1200),xlabel="Time in ns",ylabel="Counts",title="Time of 500th Photon")
# ax7 = fig7.add_subplot(111,xlim=(0,3), ylim=(0,1200),xlabel="Time in ns",ylabel="Counts",title="Time of 1000th Photon")
# ax8 = fig8.add_subplot(111,xlim=(0,3), ylim=(0,1200),xlabel="Time in ns",ylabel="Counts",title="Time of 2000th Photon")
#
# ax1.hist(D1, bins=78, color='r')
# ax1.plot(x,gauss(x,*x1_p), color='k')
# ax1.text(0.1,1150,"Data Stats:")
# ax1.text(0.1,1100,"mean: "+format(D1.mean(),"5.2f"))
# ax1.text(0.1,1050,"std: "+format(D1.std(),"5.2f"))
# ax1.text(0.1,1000,"count: "+str(C10))
# ax1.text(0.1,950,"Fit Paramters:")
# ax1.text(0.1,900,"mu: "+format(x1_p[1],"5.2f"))
# ax1.text(0.1,850,"sigma: "+format(x1_p[2],"5.2f"))
# ax1.text(0.1,800,"A: "+format(x1_p[0],"5.2f"))
#
# ax2.hist(D2, bins=78, color='g')
# ax2.plot(x,gauss(x,*x2_p), color='k')
# ax2.text(0.1,1150,"Data Stats:")
# ax2.text(0.1,1100,"mean: "+format(D2.mean(),"5.2f"))
# ax2.text(0.1,1050,"std: "+format(D2.std(),"5.2f"))
# ax2.text(0.1,1000,"count: "+str(C20))
# ax2.text(0.1,950,"Fit Paramters:")
# ax2.text(0.1,900,"mu: "+format(x2_p[1],"5.2f"))
# ax2.text(0.1,850,"sigma: "+format(x2_p[2],"5.2f"))
# ax2.text(0.1,800,"A: "+format(x2_p[0],"5.2f"))
#
# ax3.hist(D3, bins=78,color='b')
# ax3.plot(x,gauss(x,*x3_p), color='k')
# ax3.text(0.1,1150,"Data Stats:")
# ax3.text(0.1,1100,"mean: "+format(D3.mean(),"5.2f"))
# ax3.text(0.1,1050,"std: "+format(D3.std(),"5.2f"))
# ax3.text(0.1,1000,"count: "+str(C30))
# ax3.text(0.1,950,"Fit Paramters:")
# ax3.text(0.1,900,"mu: "+format(x3_p[1],"5.2f"))
# ax3.text(0.1,850,"sigma: "+format(x3_p[2],"5.2f"))
# ax3.text(0.1,800,"A: "+format(x3_p[0],"5.2f"))
#
# ax4.hist(D4, bins=78,color='c')
# ax4.plot(x,gauss(x,*x4_p), color='k')
# ax4.text(0.1,1150,"Data Stats:")
# ax4.text(0.1,1100,"mean: "+format(D4.mean(),"5.2f"))
# ax4.text(0.1,1050,"std: "+format(D4.std(),"5.2f"))
# ax4.text(0.1,1000,"count: "+str(C50))
# ax4.text(0.1,950,"Fit Paramters:")
# ax4.text(0.1,900,"mu: "+format(x4_p[1],"5.2f"))
# ax4.text(0.1,850,"sigma: "+format(x4_p[2],"5.2f"))
# ax4.text(0.1,800,"A: "+format(x4_p[0],"5.2f"))
#
# ax5.hist(D5, bins=78,color='m')
# ax5.plot(x,gauss(x,*x5_p), color='k')
# ax5.text(0.1,1150,"Data Stats:")
# ax5.text(0.1,1100,"mean: "+format(D5.mean(),"5.2f"))
# ax5.text(0.1,1050,"std: "+format(D5.std(),"5.2f"))
# ax5.text(0.1,1000,"count: "+str(C100))
# ax5.text(0.1,950,"Fit Paramters:")
# ax5.text(0.1,900,"mu: "+format(x5_p[1],"5.2f"))
# ax5.text(0.1,850,"sigma: "+format(x5_p[2],"5.2f"))
# ax5.text(0.1,800,"A: "+format(x5_p[0],"5.2f"))
#
# ax6.hist(D6, bins=78,color='tab:brown')
# ax6.plot(x,gauss(x,*x6_p), color='k')
# ax6.text(0.1,1150,"Data Stats:")
# ax6.text(0.1,1100,"mean: "+format(D6.mean(),"5.2f"))
# ax6.text(0.1,1050,"std: "+format(D6.std(),"5.2f"))
# ax6.text(0.1,1000,"count: "+str(C500))
# ax6.text(0.1,950,"Fit Paramters:")
# ax6.text(0.1,900,"mu: "+format(x6_p[1],"5.2f"))
# ax6.text(0.1,850,"sigma: "+format(x6_p[2],"5.2f"))
# ax6.text(0.1,800,"A: "+format(x6_p[0],"5.2f"))
#
# ax7.hist(D7, bins=50,color='tab:pink')
# ax7.plot(x,gauss(x,*x7_p), color='k')
# ax7.text(0.1,1150,"Data Stats:")
# ax7.text(0.1,1100,"mean: "+format(D7.mean(),"5.2f"))
# ax7.text(0.1,1050,"std: "+format(D7.std(),"5.2f"))
# ax7.text(0.1,1000,"count: "+str(C1000))
# ax7.text(0.1,950,"Fit Paramters:")
# ax7.text(0.1,900,"mu: "+format(x7_p[1],"5.2f"))
# ax7.text(0.1,850,"sigma: "+format(x7_p[2],"5.2f"))
# ax7.text(0.1,800,"A: "+format(x7_p[0],"5.2f"))
#
# ax8.hist(D8, bins=50,color='tab:grey')
# ax8.plot(x,gauss(x,*x8_p), color='k')
# ax8.text(0.1,1150,"Data Stats:")
# ax8.text(0.1,1100,"mean: "+format(D8.mean(),"5.2f"))
# ax8.text(0.1,1050,"std: "+format(D8.std(),"5.2f"))
# ax8.text(0.1,1000,"count: "+str(C2000))
# ax8.text(0.1,950,"Fit Paramters:")
# ax8.text(0.1,900,"mu: "+format(x8_p[1],"5.2f"))
# ax8.text(0.1,850,"sigma: "+format(x8_p[2],"5.2f"))
# ax8.text(0.1,800,"A: "+format(x8_p[0],"5.2f"))

#2d stacked histogram
# labels = ['10th photon', '20th photon', '30th photon', '50th photon', '100th photon']
# colors = ['r','g', 'b', 'c', 'm']
# labels1 = ['10th photon', '100th photon', '500th photon', '1000th photon', '2000th photon']
# colors1 = ['r','m', 'tab:brown', 'tab:pink', 'tab:grey']
# fig = plt.hist([D1,D2,D3,D4,D5], bins = 78, density=False, histtype='bar', color=colors,label=labels,stacked=True)
# #fig1 = plt.hist([D1,D5,D6,D7,D8], bins = 78, density=False, histtype='bar', color=colors1,label=labels1,stacked=True)
#
# #2d stacked histogram constraints
# plt.xlabel("Time in ns")
# plt.ylabel("Counts")
# plt.title("Time of nth detected photon Bare")
# plt.legend()
# plt.xlim(100,250)
# # plt.ylim(0,2200)
# plt.text(170,940, "count: "+format(D1.count(),"4"))
# plt.text(170,880, "count: "+format(D2.count(),"4"))
# plt.text(170,820, "count: "+format(D3.count(),"4"))
# plt.text(170,760, "count: "+format(D4.count(),"4"))
# plt.text(170,700, "count: "+format(D5.count(),"4"))
# #large 2d histogram version
# plt.ylim(0,1000)
# # plt.text(170,940, "count: "+format(D1.count(),"4"))
# # plt.text(170,880, "count: "+format(D5.count(),"4"))
# # plt.text(170,820, "count: "+format(D6.count(),"4"))
# # plt.text(170,760, "count: "+format(D7.count(),"4"))
# # plt.text(170,700, "count: "+format(D8.count(),"4"))
# plt.show()

#mock timing resolution for micro+ case
D1 = pd.DataFrame(D1)
D2 = pd.DataFrame(D2)
D3 = pd.DataFrame(D3)
D4 = pd.DataFrame(D4)
D5 = pd.DataFrame(D5)
D6 = pd.DataFrame(D6)
D7 = pd.DataFrame(D7)
D8 = pd.DataFrame(D8)

TRes1 = []
TRes2 = []
TRes3 = []
TRes4 = []
TRes5 = []
TRes6 = []
TRes7 = []
TRes8 = []
for i in range(len(D1) // 2):
     TRes1.append(D1["Time"][i] - D1["Time"][2*i])
for i in range(len(D2) // 2):
     TRes2.append(D2["Time"][i] - D2["Time"][2*i])
for i in range(len(D3) // 2):
     TRes3.append(D3["Time"][i] - D3["Time"][2*i])
for i in range(len(D4) // 2):
     TRes4.append(D4["Time"][i] - D4["Time"][2*i])
for i in range(len(D5) // 2):
     TRes5.append(D5["Time"][i] - D5["Time"][2*i])
for i in range(len(D6) // 2):
     TRes6.append(D6["Time"][i] - D6["Time"][2*i])
for i in range(len(D7) // 2):
     TRes7.append(D7["Time"][i] - D7["Time"][2*i])
for i in range(len(D8) // 2):
     TRes8.append(D8["Time"][i] - D8["Time"][2*i])

labels = ['10th photon', '20th photon', '30th photon', '50th photon', '100th photon']
colors = ['r','g', 'b', 'c', 'm']
labels1 = ['10th photon', '100th photon', '500th photon', '1000th photon', '2000th photon']
colors1 = ['r','m', 'tab:brown', 'tab:pink', 'tab:grey']
#fig = plt.hist([TRes1,TRes2,TRes3,TRes4,TRes5], bins = 45, density=False, histtype='bar', color=colors,label=labels,stacked=True)
fig1 = plt.hist([TRes1,TRes5,TRes6,TRes7,TRes8], bins = 45, density=False, histtype='bar', color=colors1,label=labels1,stacked=True)

#2d stacked histogram constraints
plt.xlabel("Time Difference in ns")
plt.ylabel("Counts")
plt.title("Mock Time Resolution: BaSO4")
plt.legend()
plt.xlim(-200, 200)
# plt.ylim(0,2200)
# plt.text(10,375, "count: "+format(len(TRes1),"4"))
# plt.text(10,352, "count: "+format(len(TRes2),"4"))
# plt.text(10,330, "count: "+format(len(TRes3),"4"))
# plt.text(10,306, "count: "+format(len(TRes4),"4"))
# plt.text(10,284, "count: "+format(len(TRes5),"4"))
#large 2d histogram version
plt.ylim(0,400)
plt.text(0,375, "count: "+format(len(TRes1),"4"))
plt.text(0,352, "count: "+format(len(TRes5),"4"))
plt.text(0,330, "count: "+format(len(TRes6),"4"))
plt.text(0,306, "count: "+format(len(TRes7),"4"))
plt.text(0,284, "count: "+format(len(TRes8),"4"))
plt.show()
