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


df_Counts = df.EventID.value_counts()
df_Counts = df_Counts[df_Counts > 10]
df_Counts = df_Counts.reset_index()
df_Counts.columns=['EventID', 'Counts']
df_Counts["EventID1"] = df_Counts["EventID"]




DATA = pd.merge(df_Counts,Gamma_DATA, on='EventID1')


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
	P_D = pd.merge(P,DATA, on='EventID')

doiList = P_D.gammaxpos.tolist()


fig, axs = plt.subplots()


crystalLength = 30
nChoices = [5]
refractiveIndex = 1.82
for index in range(len(nChoices)):
	nthPhotonTimes = []
	doiList = []
	doiTruthList = []
	photDOIList = []
	photDOITruthList = []
	comptonDOIList = []
	comptonPhotonTimes = []
	n = nChoices[index]
	eventByEvent = df.groupby('EventID')
	for event, frame in eventByEvent:
		eventNum = frame.EventID.iloc[0]
		sortedFrame = frame.Time.sort_values()
		if len(sortedFrame) > 4100:
			eventGamma = Gamma_DATA[Gamma_DATA['EventID1'] == eventNum]
			photonTime = sortedFrame.iloc[n-1]
			nthPhotonTimes.append(photonTime)
			# if eventNum in eid_p:
				# photPhotonTimes.append(photonTime)
				# photBackTriggers.append(backTrigger)
				# photFrontTriggers.append(frontTrigger)
				# photDOIList.append(doi)
				# photDiffList.append(differenceCounts)
				# photDOITruthList.append(doiTruth)


	
	timeDiff = []
	# for ind in range(0,len(nthPhotonTimes)-1,2):
		# timeDiff.append(nthPhotonTimes[ind] - nthPhotonTimes[ind+1])
	for ind in range(0,len(nthPhotonTimes)-1,2):
		timeDiff.append(nthPhotonTimes[ind] - nthPhotonTimes[ind+1])
	
	timeDiff = np.asarray(timeDiff)
	numBins = 50
	(binCounts, bins, patches) = axs.hist(timeDiff,bins = numBins, range = [-0.25, 0.25])
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
	

axs.set_title("Differences in Trigger Times Between Pairs of Identical Gammas\n15mm Crystal, 3mm-Wide Detectors", fontsize = 18)
# fig.suptitle("nth Photon Time vs.  DOI - All Processes")
plt.show()
	
