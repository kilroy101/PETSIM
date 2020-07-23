import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

# number of detected photons histogram
df = pd.read_csv("3x315.txt", sep=",", header=None, low_memory=False)
df.columns = ['EventID', 'CopyNumber', 'ZPos','YPos','XPos','PhotTime']
df["ZPos"] = df["ZPos"] - 142.5

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

#log-log scale histogram of detected photons
DATA = Bottom_Far.EventID.value_counts()
jazz = DATA.reset_index()
jazz.columns = ["EventID", "Counts"]
Cabbage = jazz[jazz["Counts"] >= 1000]
Standard = jazz[jazz["Counts"] >= 100]
C_1000 = Cabbage.Counts.count()
C_100 = Standard.Counts.count()

hist, bins, placeholder = plt.hist(DATA,bins=50)
plt.clf()
logbins = np.logspace(np.log10(bins[0]),np.log10(bins[-1]),len(bins))
plt.hist(DATA,bins=logbins)
plt.xscale('log')
plt.yscale('log')
plt.xlim(1,10000)
plt.ylim(1,3000)
plt.xlabel("# of photons detected")
plt.ylabel("Counts")
plt.title("Photons Detected in an Event: Vikuiti, Glue n=1.67")
plt.text(1000,2000,"mean: "+format(DATA.mean(),'7.2f'))
plt.text(1000,1550,"std: "+format(DATA.std(),'7.2f'))
plt.text(1000,1200,"count: "+format(DATA.count(),'7.2f'))
plt.text(2,2000,"# of events with 1000 or more photons: "+str(C_1000))
plt.text(2,1500,"# of events with 100 or more photons: "+str(C_100))
plt.show()


# # number of produced photons histogram
# df = pd.read_csv("PRODUCED3x315.txt", sep=",", header=None, low_memory=False)
# df.columns=['EventID', 'CopyNumber', 'XPos', 'YPos', 'ZPos']
# #log-log scale histogram of detected photons
# DATA = df.EventID.value_counts()
# hist, bins, placeholder = plt.hist(DATA,bins=50)
# plt.clf()
# logbins = np.logspace(np.log10(bins[0]),np.log10(bins[-1]),len(bins))
# plt.hist(DATA,bins=logbins)
# plt.xscale('log')
# plt.yscale('log')
# plt.xlim(1,10000)
# plt.ylim(1,3000)
# plt.xlabel("# of photons detected")
# plt.ylabel("Counts")
# plt.title("Photons Produced in an Event: Vikuiti Glue 0.1 mm")
# plt.text(1000,2000,"mean: "+format(DATA.mean(),'7.2f'))
# plt.text(1000,1550,"std: "+format(DATA.std(),'7.2f'))
# plt.text(1000,1200,"count: "+format(DATA.count(),'7.2f'))
# plt.show()


#Gamma entry point histogram
# df1=pd.read_csv("Gamma3x315.txt", sep=",", header=None, low_memory=False)
# df1.columns=['EventID', 'Time', 'gammaxpos', 'gammaypos', 'gammazpos']
#
# df1.gammaxpos = df1.gammaxpos.str.replace('(', '')
# df1.gammazpos = df1.gammazpos.str.replace(')', '')
#
# df1.gammaxpos = df1.gammaxpos.astype(np.float64)
# df1.gammazpos = df1.gammazpos.astype(np.float64)
# Gamma_Data = df1[df1['gammaxpos'] >= int(0)]
# Gamma_Data.gammaxpos = Gamma_Data.gammaxpos.subtract(142.495)
# Gamma_Data.gammaxpos = Gamma_Data.gammaxpos.abs()
# Gamma_DATA = Gamma_Data.groupby('EventID').first()
#
# zPos = Gamma_DATA['gammaxpos']
# yPos = Gamma_DATA['gammaypos']
# xPos = Gamma_DATA['gammazpos']
#
# fig = plt.figure(figsize=(8, 8))
# gs = fig.add_gridspec(4, 4)
# ax1 = fig.add_subplot(gs[0:3, 1:4], projection = '3d')
# ax2 = fig.add_subplot(gs[3, 1:4])
# ax3 = fig.add_subplot(gs[0:3, 0])
#
# numBins = 12
# #remove range below if not looking at front or back detectors
# hist, xedges, yedges = np.histogram2d(xPos, yPos, range = [[-3,3],[-3,3]], bins = numBins)#range = [[-3,3],[-3,3]],
# # Construct arrays for the anchor positions of the bars.
# xpos, ypos = np.meshgrid(xedges[:-1], yedges[:-1],indexing = "ij")
# xpos = xpos.ravel()
# ypos = ypos.ravel()
# bottom = 0
# # Construct arrays with the dimensions for the bars.
# dx = (xedges[-1] - xedges[0] ) / numBins
# dy = (yedges[-1] - yedges[0] ) / numBins
# top = hist.ravel()
#
# ax1.bar3d(xpos, ypos, bottom, dx, dy, top, zsort='average',color = "w", edgecolor = "k")
# ax1.set_xlabel("x (mm)")
# ax1.set_ylabel("y (mm)")
# ax1.set_zlabel("Entries")
# ax1.ticklabel_format(axis = 'z', style="sci", scilimits=(0,0))
#
#
# ax2.hist(xPos, bins = numBins, range = (-1.8,1.8), histtype = 'step', color = 'k')
# ax2.set_xlabel("z (mm)")
# ax2.set_ylabel("Entries")
# ax2.ticklabel_format(axis = 'y', style="sci", scilimits=(0,0))
# ax2.text(0.05, 0.75, "mean: "  + format(np.mean(xPos), "4.4f")  + "\nstd: "  + format(np.std(xPos), "4.4f") , transform = ax2.transAxes)
#
#
# ax3.hist(yPos, bins = numBins, range = (-1.8,1.8), histtype = 'step', color = 'k', orientation = u'horizontal')
# ax3.set_ylabel("x (mm)", rotation = 270)
# ax3.set_xlabel("Entries")
# ax3.ticklabel_format(axis = 'x', style="sci", scilimits=(0,0))
# ax3.text(0.05, 0.93, "mean: "  + format(np.mean(yPos), "4.4f")  + "\nstd: "  + format(np.std(yPos), "4.4f") , transform = ax3.transAxes)
#
#
# fig.suptitle("Point of Gamma Entry - Vikuiti, Glue n = 1.5 \nEvents: 5000   Total Entries: " + str(len(yPos)), fontsize = 20)
#
# plt.show()
