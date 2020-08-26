#Chris Layden
#Make a scatter plot for DR Pixelated array: each point an event, x axis: # photons detected in near detector, y axis: # detected in far detector

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit
from scipy.stats import norm
from scipy.stats import chisquare

df=pd.read_csv("3x315.txt", sep=",", header=None, low_memory=False)
df.columns=['EventID', 'CopyNumber', 'XPos', 'YPos','ZPos','Time','Wavelength']
Det_limit = int(64)


df1=pd.read_csv("Gamma3x315.txt", sep=",", header=None, low_memory=False)
df1.columns=['EventID', 'Time', 'gammaxpos', 'gammaypos', 'gammazpos']
df1["EventID1"] = df1["EventID"]

df = df[df.EventID.isin(df1.EventID)]
df1 = df1[df1.EventID.isin(df.EventID)]

df1.gammaxpos = df1.gammaxpos.str.replace('(', '')
df1.gammazpos = df1.gammazpos.str.replace(')', '')

df1.gammaxpos = df1.gammaxpos.astype(np.float64)
df1.gammazpos = df1.gammazpos.astype(np.float64)
Gamma_Data = df1[df1['gammaxpos'] >= int(0)]
Gamma_Data.gammaxpos = Gamma_Data.gammaxpos.subtract(163.91)
Gamma_Data.gammaxpos = Gamma_Data.gammaxpos.abs()
Gamma_DATA = Gamma_Data.groupby('EventID').last()

# def ColorRule(x):
	# if x >= 0 and x <= 3:
		# return "red"
	# elif x >= 3 and x <= 6:
		# return "orange"
	# elif x >= 6 and x <= 9:
		# return "green"
	# elif x >= 9 and x <= 12:
		# return "blue"
	# elif x >= 12 and x <= 15:
		# return "purple"
		
		
fig = plt.figure()
ax = fig.add_subplot()
		
countListBack = []
countListFront = []
doiList = []
eventByEvent = df.groupby('EventID')
for event, frame in eventByEvent:
	frontFrame = frame[frame['CopyNumber'] <= Det_limit]
	backFrame = frame[frame['CopyNumber'] >= Det_limit]
	if frontFrame.CopyNumber.count() == 0 or backFrame.CopyNumber.count() == 0:
		continue
	event = backFrame.EventID.iloc[0]
	countBack = (backFrame.XPos.count())
	countFront = (frontFrame.XPos.count())
	doi = float(Gamma_DATA[Gamma_DATA.EventID1 == event].gammaxpos)
	countListBack.append(countBack)
	countListFront.append(countFront)
	doiList.append(doi)
	# ax.scatter(countFront, countBack, s = 2, color = ColorRule(doi))

countListBack1 = []
countListBack2 = []
countListBack3 = []
countListBack4 = []
countListBack5 = []
countListBack6 = []
countListBack7 = []
countListBack8 = []
countListBack9 = []
countListBack10 = []
countListFront1 = []
countListFront2 = []
countListFront3 = []
countListFront4 = []
countListFront5 = []
countListFront6 = []
countListFront7 = []
countListFront8 = []
countListFront9 = []
countListFront10 = []
for i in range(len(countListBack)):
	if doiList[i] >= 0 and doiList[i] <= 3:
		countListBack1.append(countListBack[i])
		countListFront1.append(countListFront[i])
	elif doiList[i] >= 3 and doiList[i] <= 6:
		countListBack2.append(countListBack[i])
		countListFront2.append(countListFront[i])
	elif doiList[i] >= 6 and doiList[i] <= 9:
		countListBack3.append(countListBack[i])
		countListFront3.append(countListFront[i])
	elif doiList[i] >= 9 and doiList[i] <= 12:
		countListBack4.append(countListBack[i])
		countListFront4.append(countListFront[i])
	elif doiList[i] >= 12 and doiList[i] <= 15:
		countListBack5.append(countListBack[i])
		countListFront5.append(countListFront[i])
	elif doiList[i] >= 15 and doiList[i] <= 18:
		countListBack6.append(countListBack[i])
		countListFront6.append(countListFront[i])
	elif doiList[i] >= 18 and doiList[i] <= 21:
		countListBack7.append(countListBack[i])
		countListFront7.append(countListFront[i])
	elif doiList[i] >= 21 and doiList[i] <= 24:
		countListBack8.append(countListBack[i])
		countListFront8.append(countListFront[i])
	elif doiList[i] >= 24 and doiList[i] <= 27:
		countListBack9.append(countListBack[i])
		countListFront9.append(countListFront[i])
	elif doiList[i] >= 27 and doiList[i] <= 30:
		countListBack10.append(countListBack[i])
		countListFront10.append(countListFront[i])

pointSize = 5
ax.scatter(countListFront1, countListBack1, s = pointSize, color = 'red', label = '0 < DOI < 3 ')
ax.scatter(countListFront2, countListBack2, s = pointSize, color = 'orange', label = '3 < DOI < 6')
ax.scatter(countListFront3, countListBack3, s = pointSize, color = 'green', label = '6 < DOI < 9')
ax.scatter(countListFront4, countListBack4, s = pointSize, color = 'cyan', label = '9 < DOI < 12')
ax.scatter(countListFront5, countListBack5, s = pointSize, color = 'blue', label = '12 < DOI < 15')
ax.scatter(countListFront6, countListBack6, s = pointSize, color = 'purple', label = '15 < DOI < 18 ')
ax.scatter(countListFront7, countListBack7, s = pointSize, color = 'magenta', label = '18 < DOI < 21')
ax.scatter(countListFront8, countListBack8, s = pointSize, color = 'pink', label = '21 < DOI < 24')
ax.scatter(countListFront9, countListBack9, s = pointSize, color = 'brown', label = '24 < DOI < 27')
ax.scatter(countListFront10, countListBack10, s = pointSize, color = 'black', label = '27 < DOI < 30')
ax.set_xlim([00, 2700])
ax.set_ylim([00, 2500])
ax.legend()
# ax = fig.add_subplot(projection = '3d')
# numBins = 20
# hist, xedges, yedges = np.histogram2d(countListFront, countListBack, bins = numBins)
# # Construct arrays for the anchor positions of the bars.
# xpos, ypos = np.meshgrid(xedges[:-1], yedges[:-1],indexing = "ij")
# xpos = xpos.ravel()
# ypos = ypos.ravel()
# bottom = 0
# # Construct arrays with the dimensions for the bars.
# dx = (xedges[-1] - xedges[0] ) / numBins
# dy = (yedges[-1] - yedges[0] ) / numBins
# top = hist.ravel()
# ax.bar3d(xpos,ypos, bottom, dx, dy, top, zsort='average',color = "w", edgecolor = "k")
ax.set_xlabel("Photons in Front Detector")
ax.set_ylabel("Photons in Back Detector")
# ax.set_zlabel("Number of Events")
plt.suptitle("Photons in Front and Back Detectors for Each Gamma - 30 mm Crystal, 3.135 mm Detector")
plt.show()
