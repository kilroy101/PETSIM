import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from iminuit import Minuit
from iminuit.cost import UnbinnedNLL, LeastSquares
from scipy.optimize import curve_fit

whatDetector = 'Difference' #'Front', 'Back', 'Aug' or 'Difference'

def gauss(x,a,mu,sigma,c):
    y = a*np.exp(-(x - mu)**2 / (2 * sigma**2))+c
    return y

def Line(x, m, b):
	return x * m + b

df=pd.read_csv("3x315.txt", sep=",", header=None, low_memory=False)
df.columns=['EventID', 'CopyNumber', 'XPos', 'YPos', 'ZPos', 'Time','Wavelength']
eventCounts = df.EventID.value_counts()

Det_limit = int(64)
Time_limit = int(5)
Pos_limit = int(0)

#df = df[df['Time'] <= Time_limit]
df  = df[df['XPos'] >= Pos_limit]

df_Front = df[df['CopyNumber'] <= Det_limit]
df_Back = df[df['CopyNumber'] >= Det_limit]


df_Front = df_Front.EventID.value_counts()
df_Back = df_Back.EventID.value_counts()
df_Front = df_Front[df_Front > 10]
df_Back = df_Back[df_Back > 10]
df_Difference = df_Front.subtract(df_Back)
df_Total = df_Front.add(df_Back)
df_AugmentedRatio = df_Difference.divide(df_Total)

df_Front = df_Front.reset_index()
df_Front.columns=['EventID', 'Front_Counts']
df_Front["EventID1"] = df_Front["EventID"]

df_Back = df_Back.reset_index()
df_Back.columns=['EventID', 'Back_Counts']
df_Back["EventID1"] = df_Back["EventID"]

df_Difference = df_Difference.reset_index()
df_Difference.columns=['EventID', 'Difference_Counts']
df_Difference["EventID1"] = df_Difference["EventID"]

df_AugmentedRatio = df_AugmentedRatio.reset_index()
df_AugmentedRatio.columns = ['EventID', 'Augmented_Difference_Counts']
df_AugmentedRatio["EventID1"] = df_Difference["EventID"]

df1=pd.read_csv("Gamma3x315.txt", sep=",", header=None, low_memory=False)
df1.columns=['EventID', 'Time', 'gammaxpos', 'gammaypos', 'gammazpos']
df1["EventID1"] = df1["EventID"]

df1.gammaxpos = df1.gammaxpos.str.replace('(', '')
df1.gammazpos = df1.gammazpos.str.replace(')', '')

df1.gammaxpos = df1.gammaxpos.astype(np.float64)
df1.gammazpos = df1.gammazpos.astype(np.float64)
Gamma_Data = df1[df1['gammaxpos'] >= int(0)]
Gamma_Data.gammaxpos = Gamma_Data.gammaxpos.subtract(163.91)
Gamma_Data.gammaxpos = Gamma_Data.gammaxpos.abs()
Gamma_DATA = Gamma_Data.groupby('EventID').last()

DATA_FRONT = pd.merge(df_Front,Gamma_DATA, on='EventID1')
DATA_BACK = pd.merge(df_Back,Gamma_DATA, on='EventID1')
DATA_DIFFERENCE = pd.merge(df_Difference,Gamma_DATA, on='EventID1')
DATA_AUG = pd.merge(df_AugmentedRatio,Gamma_DATA, on='EventID1')

df2=pd.read_csv("Edata15.txt", sep=",", header=None, low_memory=False)
df2.columns=['CP','EventID', 'Energy', 'Sec_x','TrackID']
E_data = df2[df2['Sec_x'] >= 0]
E_dataCopy = E_data.copy()
oof = E_data.EventID.value_counts()
oof = oof.reset_index()
oof.columns=['EventID', '# of processes']

E_data = E_data.drop_duplicates(['EventID','TrackID'])
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



P = pd.DataFrame(eid_p)
if not P.empty:
	P.columns = ['EventID']
	P_DFRONT = pd.merge(P,DATA_FRONT, on='EventID')
	P_DBACK = pd.merge(P,DATA_BACK, on='EventID')
	P_DDIFF = pd.merge(P,DATA_DIFFERENCE, on='EventID')
	P_DAUG = pd.merge(P,DATA_AUG, on='EventID')
	# if whatDetector == 'Front':
		# ax1 = P_DFRONT.plot(kind='scatter', y ='gammaxpos', x='Front_Counts', label = 'phot',color='b')
	# elif whatDetector == 'Back':
		# ax1 = P_DBACK.plot(kind='scatter', y ='gammaxpos', x='Back_Counts', label = 'phot',color='b')
	# elif whatDetector == 'Difference':
		# ax1 = P_DDIFF.plot(kind='scatter', y ='gammaxpos', x='Difference_Counts', label = 'phot',color='b')
	# elif whatDetector == 'Aug':
		# ax1 = P_DAUG.plot(kind='scatter', y ='gammaxpos', x='Augmented_Difference_Counts', label = 'phot',color='b')

P_DDIFF = P_DDIFF.dropna()
doiListPhot = P_DDIFF.gammaxpos.tolist()
differenceCountsPhot = P_DDIFF.Difference_Counts.tolist()

DATA_DIFFERENCE = DATA_DIFFERENCE.dropna()
doiList = DATA_DIFFERENCE.gammaxpos.tolist()
differenceCounts = DATA_DIFFERENCE.Difference_Counts.tolist()


m = Minuit(LeastSquares(differenceCountsPhot, doiListPhot,10.0, Line), m = -500, b = 3000)
m.migrad()
m.hesse()
# print(m.values)
# x = range(int(min(differenceCounts)), int(max(differenceCounts)))
# y = [Line(k, *m.values[0:2]) for k in x]
# ax1.plot(x, y, color = 'k')
# C = pd.DataFrame(eid_c)
# if not C.empty:
	# C.columns = ['EventID']
	# C_DFRONT = pd.merge(C,DATA_FRONT, on='EventID')
	# C_DBACK = pd.merge(C,DATA_BACK, on='EventID')
	# C_DDIFF = pd.merge(C,DATA_DIFFERENCE, on='EventID')
	# C_DDIFF = pd.merge(C,DATA_DIFFERENCE, on='EventID')
	# C_DAUG = pd.merge(C,DATA_AUG, on='EventID')
	# if whatDetector == 'Front':
		# ax2 = C_DFRONT.plot(kind='scatter', y ='gammaxpos', x='Front_Counts', label = 'compt',color='g', ax = ax1)
	# elif whatDetector == 'Back':
		# ax2 = C_DBACK.plot(kind='scatter', y ='gammaxpos', x='Back_Counts', label = 'compt',color='g', ax = ax1)
	# elif whatDetector == 'Difference':
		# ax2 = C_DDIFF.plot(kind='scatter', y ='gammaxpos', x='Difference_Counts', label = 'compt',color='g', ax = ax1)
	# elif whatDetector == 'Aug':
		# ax2 = C_DAUG.plot(kind='scatter', y ='gammaxpos', x='Augmented_Difference_Counts', label = 'compt',color='g', ax = ax1)
		
# CP = pd.DataFrame(eid_cp)
# if not CP.empty:
	# CP.columns = ['EventID']
	# CP_DFRONT = pd.merge(CP,DATA_FRONT, on='EventID')
	# CP_DBACK = pd.merge(CP,DATA_BACK, on='EventID')
	# CP_DDIFF = pd.merge(CP,DATA_DIFFERENCE, on='EventID')
	# CP_DAUG = pd.merge(CP,DATA_AUG, on='EventID')
	# if whatDetector == 'Front':
		# ax2 = CP_DFRONT.plot(kind='scatter', y ='gammaxpos', x='Front_Counts', label = 'compt-phot',color='r', ay =ax1)
	# elif whatDetector == 'Back':
		# ax2 = CP_DBACK.plot(kind='scatter', y ='gammaxpos', x='Back_Counts', label = 'compt-phot',color='r', ay =ax1)
	# elif whatDetector == 'Difference':
		# ax2 = CP_DDIFF.plot(kind='scatter', y ='gammaxpos', x='Difference_Counts', label = 'compt-phot',color='r', ax = ax1)
	# elif whatDetector == 'Aug':
		# ax2 = CP_DAUG.plot(kind='scatter', y ='gammaxpos', x='Augmented_Difference_Counts', label = 'compt-phot',color='r', ax = ax1)
	
# CC = pd.DataFrame(eid_cc)
# if not CC.empty:
	# CC.columns = ['EventID']
	# CC_DFRONT = pd.merge(CC,DATA_FRONT, on='EventID')
	# CC_DBACK = pd.merge(CC,DATA_BACK, on='EventID')
	# CC_DDIFF = pd.merge(CC,DATA_DIFFERENCE, on='EventID')
	# CC_DAUG = pd.merge(CC,DATA_AUG, on='EventID')
	# if whatDetector == 'Front':
		# ax2 = CC_DFRONT.plot(kind='scatter', y ='gammaxpos', x='Front_Counts', label = 'compt-compt',color='c', ay =ax1)
	# elif whatDetector == 'Back':
		# ax2 = CC_DBACK.plot(kind='scatter', y ='gammaxpos', x='Back_Counts', label = 'compt-compt',color='c', ay =ax1)
	# elif whatDetector == 'Difference':
		# ax2 = CC_DDIFF.plot(kind='scatter', y ='gammaxpos', x='Difference_Counts', label = 'compt-compt',color='c', ax = ax1)
	# elif whatDetector == 'Aug':
		# ax2 = CC_DAUG.plot(kind='scatter', y ='gammaxpos', x='Augmented_Difference_Counts', label = 'compt-compt',color='c', ax = ax1)
	
# O = pd.DataFrame(eid_other)
# if not O.empty:
	# O.columns = ['EventID']
	# O_DFRONT = pd.merge(O,DATA_FRONT, on='EventID')
	# O_DBACK = pd.merge(O,DATA_BACK, on='EventID')
	# O_DDIFF = pd.merge(O,DATA_DIFFERENCE, on='EventID')
	# O_DAUG = pd.merge(O,DATA_AUG, on='EventID')
	# if whatDetector == 'Front':
		# ax2 = O_DFRONT.plot(kind='scatter', y ='gammaxpos', x='Front_Counts', label = 'other',color='m', ay =ax1)
	# elif whatDetector == 'Back':
		# ax2 = O_DBACK.plot(kind='scatter', y ='gammaxpos', x='Back_Counts', label = 'other',color='m', ay =ax1)
	# elif whatDetector == 'Difference':
		# ax2 = O_DDIFF.plot(kind='scatter', y ='gammaxpos', x='Difference_Counts', label = 'other',color='m', ax = ax1)
	# elif whatDetector == 'Aug':
		# ax2 = O_DAUG.plot(kind='scatter', y ='gammaxpos', x='Augmented_Difference_Counts', label = 'other',color='m', ax = ax1)

# if whatDetector == 'Front':
	# plt.title("Vikuity DR Pixelated Array, Center Gammas: Front Detector")
# elif whatDetector == 'Back':
	# plt.title("Vikuity DR Geometry Pixelated Array, Center Gammas: Back Detector")

# ax1.set_ylabel("DOI (mm)")
# ax1.set_xlabel("Difference in Front and Back Counts")
# ax1.set_ylim([0, 30])
# plt.suptitle("Difference in Detector Counts vs. DOI")

# countsBins = []
# countsBinsCenters = []
# countsBinsCentersDOI = []
# binDOIList = []
# for i in range(10):
	# newBin = [-600 + i * 120, -600 + (i+1) * 120]
	# # newBin = [min(differenceCountsList) + i * (max(differenceCountsList) - min(differenceCountsList)) / 10, min(differenceCountsList) + (i + 1) * (max(differenceCountsList) - min(differenceCountsList)) / 10]
	# newBinCenter = np.average(newBin)
	# countsBins.append(newBin)
	# countsBinsCenters.append(newBinCenter)
	# newBinCenterDOI = Line(newBinCenter, *m.values[0:2])
	# countsBinsCentersDOI.append(newBinCenterDOI)
	# binDOIList.append([])
	# for j in range(len(differenceCounts)):
		# if differenceCounts[j] > newBin[0] and differenceCounts[j] < newBin[1]:
			# binDOIList[i].append(doiList[j])
	
# fig, axs = plt.subplots(1, 2)
	
# for k in range(10):
	# axs[1].hist(binDOIList[k], bins = 25)
	# plt.show()

# plt.show()

fitVsTruthList = []
for i in range(len(differenceCounts)):
	fitVsTruth = Line(differenceCounts[i], *m.values[0:2]) - doiList[i]
	fitVsTruthList.append(fitVsTruth)
	
crystalLength = 30
ax = plt.subplot()
ax.hist(fitVsTruthList, range = [-crystalLength, crystalLength], bins = 25)
ax.set_title("Differences Between Fit DOI and Truth DOI: " + str(crystalLength) + " mm Crystal")
ax.set_xlabel("Difference Between Fit and Truth DOI (mm)")
ax.set_ylabel("Number of Gammas")
ax.text(0.05, 0.85, "std: " + format(np.std(fitVsTruthList), '3.3f') + " mm", transform = ax.transAxes)
plt.show()

# maxCountsDiff = 500
# numBins = 20
# xedges = np.linspace(-maxCountsDiff, maxCountsDiff, numBins + 1)
# doiSplitByDiffs = []
# for i in range(numBins):
	# doiSplitByDiffs.append([])
	# for j in range(len(doiList)):
		# if (differenceCounts[j] > xedges[i]) and (differenceCounts[j] < xedges[i+1]):
			# doiSplitByDiffs[i].append(doiList[j])
			
# fig, axs = plt.subplots(4, 5)
# plt.tight_layout(pad = 2, h_pad = 1, w_pad = 0)
# i, j = 0, 0
# for k in range(numBins):
	# axs[i,j].scatter(Line(xedges[k:k+2], *m.values[0:2]), [0,0], zorder = 2, label = "Predicted DOI value of Counts Difference Endpoints", color = 'r')
	# binCounts, bins, patches = axs[i,j].hist(doiSplitByDiffs[k], zorder = 1, color = 'b')
	# binCenters = np.linspace(min(bins), max(bins), 10)
	# # x1_p, x1_co = curve_fit(gauss, binCenters, binCounts, p0=[10,10,10,0],maxfev=5000, bounds = (10000, None))
	# n = Minuit(LeastSquares(binCenters,binCounts, 1,gauss), a = 10, mu = 15, sigma = 2, c = 0, limit_a = (0, 1000))
	# n.migrad()
	# n.hesse()
	# x = np.linspace(0, 15, 100)
	# # y = gauss(x, *x1_p)
	# y = gauss(x, *n.values[0:4])
	# axs[i,j].plot(x, y, color = 'k')
	# axs[i,j].set_xlim([0,15])
	# axs[i,j].set_xlabel("DOI (mm)")
	# axs[i,j].set_ylabel("Counts")
	# axs[i,j].set_title(format(int(xedges[k]), "3d") + " < Difference  < " + format(int(xedges[k+1]), "3d"))
	# axs[i,j].text(0.05, 0.85, "std: " + format(np.std(doiSplitByDiffs[k]), '3.3f') + " mm", transform = axs[i,j].transAxes)
	# axs[i,j].text(0.05, 0.7, "fit sigma: " + format(n.values[2], '3.3f') + " mm", transform = axs[i,j].transAxes)
	# # axs[i,j].legend(loc = 'upper right')
	
	# j=j+1
	# if j == 5:
		# i+=1
		# j=0
# plt.suptitle("DOI Resolution At Different Counts Difference Points: 15 mm Crystal")
# plt.show()

