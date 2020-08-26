import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit
from scipy.stats import norm
from scipy.stats import chisquare
from iminuit import Minuit
from iminuit.cost import UnbinnedNLL, LeastSquares

def gauss(x,a,mu,sigma,c):
    y = a*np.exp(-(x - mu)**2 / (2 * sigma**2))+c
    return y
    
def Line(x, m, b):
	return x * m + b

df=pd.read_csv("3x315.txt", sep=",", header=None, low_memory=False)
df.columns=['EventID', 'CopyNumber', 'XPos', 'YPos','ZPos','Time', 'Wavelength']
df["EventID1"] = df["EventID"]


df1=pd.read_csv("Gamma3x315.txt", sep=",", header=None, low_memory=False)
df1.columns=['EventID', 'Time', 'gammaxpos', 'gammaypos', 'gammazpos']
df1["EventID1"] = df1["EventID"]
df = df[df.EventID.isin(df1.EventID)]
df1 = df1[df1.EventID.isin(df.EventID)]

Det_limit = int(64)
df_Front = df[df['CopyNumber'] < Det_limit]
df_Back = df[df['CopyNumber'] >= Det_limit]



df1.gammaxpos = df1.gammaxpos.str.replace('(', '')
df1.gammazpos = df1.gammazpos.str.replace(')', '')
df1.gammaxpos = df1.gammaxpos.astype(np.float64)
df1.gammazpos = df1.gammazpos.astype(np.float64)
Gamma_Data = df1[df1['gammaxpos'] >= int(0)]
Gamma_Data.gammaxpos = Gamma_Data.gammaxpos.subtract(163.91)
Gamma_Data.gammaxpos = Gamma_Data.gammaxpos.abs()
Gamma_DATA = Gamma_Data.groupby('EventID').last()


df_FrontCounts = df_Front.EventID.value_counts()
df_BackCounts = df_Back.EventID.value_counts()
df_FrontCounts = df_FrontCounts[df_FrontCounts > 10]
df_BackCounts = df_BackCounts[df_BackCounts > 10]
df_Difference = df_FrontCounts.subtract(df_BackCounts)

df_FrontCounts = df_FrontCounts.reset_index()
df_FrontCounts.columns=['EventID', 'Front_Counts']
df_FrontCounts["EventID1"] = df_FrontCounts["EventID"]

df_BackCounts = df_BackCounts.reset_index()
df_BackCounts.columns=['EventID', 'Back_Counts']
df_BackCounts["EventID1"] = df_BackCounts["EventID"]

df_Difference = df_Difference.reset_index()
df_Difference.columns=['EventID', 'Difference_Counts']
df_Difference["EventID1"] = df_Difference["EventID"]



DATA_FRONT = pd.merge(df_FrontCounts,Gamma_DATA, on='EventID1')
DATA_BACK = pd.merge(df_BackCounts,Gamma_DATA, on='EventID1')
DATA_DIFFERENCE = pd.merge(df_Difference,Gamma_DATA, on='EventID1')

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

P_DDIFF = P_DDIFF.dropna()
P_DBACK = P_DBACK.dropna()
doiList = P_DBACK.gammaxpos.tolist()
backCountsList = P_DBACK.Back_Counts.tolist()
diffCountsList = P_DDIFF.Difference_Counts.tolist()

k = Minuit(LeastSquares(diffCountsList, doiList, 10.0, Line), m = 0.02, b = 0)
k.migrad()
k.hesse()

fig, axs = plt.subplots()


crystalLength = 30
nChoices = [5]
refractiveIndex = 1.82
for index in range(len(nChoices)):
	nthPhotonTimes = []
	backTriggers = []
	frontTriggers = []
	doiList = []
	doiTruthList = []
	photDOIList = []
	photDOITruthList = []
	comptonDOIList = []
	photPhotonTimes = []
	photFrontTriggers = []
	photBackTriggers = []
	comptonPhotonTimes = []
	diffList = []
	photDiffList = []
	frontWeights = []
	n = nChoices[index]
	eventByEvent = df.groupby('EventID')
	for event, frame in eventByEvent:
		eventNum = frame.EventID.iloc[0]
		frontFrame = frame[frame['CopyNumber'] < Det_limit]
		backFrame = frame[frame['CopyNumber'] >= Det_limit]
		frontSorted = frontFrame.Time.sort_values()
		backSorted = backFrame.Time.sort_values()
		sortedFrame = frame.Time.sort_values()
		if len(sortedFrame) > 4000:
			eventGamma = Gamma_DATA[Gamma_DATA['EventID1'] == eventNum]
			differenceCounts = float(DATA_DIFFERENCE[DATA_DIFFERENCE['EventID1'] == eventNum].Difference_Counts)
			doi = Line(differenceCounts, *k.values[0:2])
			doiTruth = eventGamma['gammaxpos'].iloc[0]
			frontTrigger = frontSorted.iloc[n-1]
			backTrigger = backSorted.iloc[n-1]
			photonTime = sortedFrame.iloc[n-1]
			frontTrigger = frontTrigger - (refractiveIndex + 1) * doi / 299.792 #speed of light in mm/ns
			backTrigger = backTrigger - refractiveIndex * (15 - doi) / 299.792 - doi / 299.792 # back detector
			frontTriggers.append(frontTrigger)
			backTriggers.append(backTrigger)
			nthPhotonTimes.append((backTrigger+frontTrigger)/2)
			doiList.append(doi)
			# frontWeight = len(frontSorted) / (len(frontSorted) + len(backSorted))
			frontWeight = 0.6 - 0.3 * doi / crystalLength
			frontWeights.append(frontWeight)
			doiTruthList.append(doiTruth)
			diffList.append(differenceCounts)
			if eventNum in eid_p:
				photPhotonTimes.append(photonTime)
				photBackTriggers.append(backTrigger)
				photFrontTriggers.append(frontTrigger)
				photDOIList.append(doi)
				photDiffList.append(differenceCounts)
				photDOITruthList.append(doiTruth)
			# else:
				# doiList.append(doi)
				# nthPhotonTimes.append(photonTime)

	# m = Minuit(LeastSquares(doiList,nthPhotonTimes,0.01,Line), m = 0.01, b = 0)
	# m = Minuit(LeastSquares(photDOITruthList,photPhotonTimes,0.01,Line), m = 0.01, b = 0)
	# m = Minuit(LeastSquares(diffList,nthPhotonTimes,0.01,Line), m = 0.01, b = 0)
	frontFit = Minuit(LeastSquares(photDiffList,photFrontTriggers,0.01,Line), m = 0.01, b = 0)
	frontFit.migrad()
	frontFit.hesse()
	backFit = Minuit(LeastSquares(photDiffList,photBackTriggers,0.01,Line), m = 0.01, b = 0)
	backFit.migrad()
	backFit.hesse()
	# frontTriggers = [(frontTriggers[k] - Line(diffList[k], *frontFit.values[0:2])) for k in range(len(diffList))]
	# backTriggers = [(backTriggers[k] - Line(diffList[k], *backFit.values[0:2])) for k in range(len(diffList))]
	averagedTriggerTimes = []
	for ind in range(len(frontTriggers)):
		# averagedTriggerTimes.append((frontTriggers[ind]+backTriggers[ind])/2)
		averagedTriggerTimes.append(frontWeights[ind] * frontTriggers[ind]+(1 - frontWeights[ind]) * backTriggers[ind])
		
	# numSections = 8
	# frontTriggersSplitUp = [[] for _ in range(numSections)]
	# backTriggersSplitUp = [[] for _ in range(numSections)]
	# for z in range(len(doiList)):
		# sectionLength = crystalLength / numSections
		# for crystalSection in range(numSections):
			# if (doiList[z] >= crystalSection * sectionLength) and (doiList[z] < (crystalSection + 1) * sectionLength):
				# frontTriggersSplitUp[crystalSection].append(frontTriggers[z])
				# backTriggersSplitUp[crystalSection].append(backTriggers[z])
				# break

	# frontVariances = [np.std(frontTriggersSplitUp[k]) for k in range(numSections)]
	# backVariances = [np.std(backTriggersSplitUp[k]) for k in range(numSections)]
	# varianceWeights = [(1 / frontVariances[k] / (1 / frontVariances[k] + 1 / backVariances[k])) for k in range(numSections)]
	# axs.plot(varianceWeights)
	# plt.show()

	# x = range(-5, 20)
	# # x = range(int(min(diffList)), int(max(diffList)))
	# y = [Line(k, *m.values[0:2]) for k in x]
	# axs[i,j].plot(x, y, color = 'k')
	# axs[i,j].set_xlabel("DOI (mm)")
	# axs[i,j].set_ylabel("Unadjusted Trigger Time (ns)")
	# axs[i,j].set_ylim([100.5, 106])
	# axs[i,j].scatter(doiList, nthPhotonTimes, color = 'b')
	# # axs[i,j].scatter(comptonDOIList, comptonPhotonTimes, color = 'g')
	# # axs[i,j].scatter(photDOIList, photPhotonTimes, color = 'b')
	# # axs[i,j].scatter(diffList, nthPhotonTimes, color = 'b', s = 3)
	# # axs[i,j].scatter(photDiffList, photPhotonTimes, color = 'b')
	# # axs[i,j].set_xlabel("Estimated DOI (mm)")
	
	# axs[i,j].text(0.75,0.9,"slope: "+format(m.values[0],"6.6f"), transform = axs[i,j].transAxes)
	# axs[i,j].text(0.75,0.85,"intercept: "+format(m.values[1],"5.2f"), transform = axs[i,j].transAxes)
	
	# nthPhotonTimes = [(nthPhotonTimes[k] - Line(doiTruthList[k], *m.values[0:2])) for k in range(len(doiTruthList))]
	# nthPhotonTimes = [(nthPhotonTimes[k] - Line(diffList[k], *m.values[0:2])) for k in range(len(diffList))]
	
	timeDiff = []
	# for ind in range(0,len(nthPhotonTimes)-1,2):
		# timeDiff.append(nthPhotonTimes[ind] - nthPhotonTimes[ind+1])
	for ind in range(0,len(averagedTriggerTimes)-1,2):
		timeDiff.append(averagedTriggerTimes[ind] - averagedTriggerTimes[ind+1])
	
	timeDiff = np.asarray(timeDiff)
	numBins = 150
	(binCounts, bins, patches) = axs.hist(timeDiff,bins = numBins, range = [-1, 1])
	binCenters = np.linspace(min(bins),max(bins),numBins)
	x1_p, x1_co = curve_fit(gauss,binCenters,binCounts, p0=[10,0,n/10,0],maxfev=5000)
	
	
	
		

	chisquare = 0
	for m in range(numBins):
		err = (binCounts[m]  - gauss(binCenters[m], *x1_p ))**2 / (gauss(binCenters[m], *x1_p))
		chisquare += err
		
	dof = numBins - len(x1_p)
	chisquare = chisquare/dof
	
	fitPoints = np.linspace(-0.5, 0.5, 1000)
	axs.plot(fitPoints,gauss(fitPoints,*x1_p), color='k')
	axs.text(0.75,0.9,"Data Stats:", weight = 'bold', transform = axs.transAxes)
	axs.text(0.75,0.85,"mean: "+format(np.mean(timeDiff),"5.4f"), transform = axs.transAxes)
	axs.text(0.75,0.8,"std: "+format(np.std(timeDiff),"5.4f"), transform = axs.transAxes)
	axs.text(0.75,0.75,"count: "+str(len(timeDiff)), transform = axs.transAxes)
	axs.text(0.75,0.7,"Fit Paramters:", weight = 'bold', transform = axs.transAxes)
	axs.text(0.75,0.65,"A: "+format(x1_p[0],"5.4f"), transform = axs.transAxes)
	axs.text(0.75,0.6, r"$\mu:$ "+format(x1_p[1],"5.4f"), transform = axs.transAxes)
	axs.text(0.75,0.55,r"$\sigma:$ "+format(abs(x1_p[2]),"5.4f"), transform = axs.transAxes)
	axs.text(0.75,0.5,"FWHM: " + format(2.355 * abs(x1_p[2]),"5.4f"), transform = axs.transAxes)
	axs.text(0.75,0.45,"Fit Quality:", weight = 'bold', transform = axs.transAxes)
	axs.text(0.75,0.4, r"$\chi^{2}:$"+format(chisquare,"5.4f"), transform = axs.transAxes)
	axs.set_xlim([-0.25, 0.25])
	axs.set_ylim(bottom = 0)
	axs.set_xlabel("Time Dfference (ns)")
	axs.set_ylabel("Number of Gamma Pairs")	
	

# axs.set_title("Differences in Trigger Times Between Pairs of Identical Gammas\nAverage of Trigger Times from Front and Back Detectors", fontsize = 18)
# fig.suptitle("nth Photon Time vs.  DOI - All Processes")
plt.show()
	
