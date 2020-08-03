import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit
from scipy.stats import norm

df=pd.read_csv("PHOT_15_40x3_pix_time_g5.txt", sep=",", header=None, low_memory=False)
df.columns=['EventID', 'CopyNumber', 'XPos', 'YPos','ZPos','Time']

df2=pd.read_csv("Edata15_40x3_pix_time_g5.txt", sep=",", header=None, low_memory=False)
# df2.columns=['CP','EventID', 'Energy', 'Sec_x']
df2.columns=['CP','EventID', 'ZZZ']
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
    elif i[1]["compt"] == 1:
        # has 1 compton
        eid_c.append(i[0])
    elif i[1]["phot"] == 1:
        # has 1 phot
        eid_p.append(i[0])
    elif i[1]["compt"] == 2:
        # has two comptons
        eid_cc.append(i[0])
    else:
        eid_other.append(i[0])

# df = df[df["EventID"].isin(eid_cc)]

fig, axs = plt.subplots(2, 2)
fig.tight_layout(pad=2.0)

i = 0
j = 0
nChoices = [1,10]
for index in range(len(nChoices)):
	nthPhotonTimes = []
	n = nChoices[index]
	eventByEvent = df.groupby('EventID')['Time']
	for event, frame in eventByEvent:
		sortedFrame = frame.sort_values()
		if len(sortedFrame) < 1000 and len(sortedFrame) > n:
			photonTime = sortedFrame.iloc[n-1]
			nthPhotonTimes.append(photonTime)
	
	timeDiff = []
	for k in range(0,len(nthPhotonTimes)-1,2):
		timeDiff.append(nthPhotonTimes[k] - nthPhotonTimes[k+1])
	
	
	
	def gauss(x,a,mu,sigma,c):
	    y = a*np.exp(-(x - mu)**2 / (2 * sigma**2))+c
	    return y
	    
	
	timeDiff = np.asarray(timeDiff)
	numBins = 100
	(binCounts, bins, patches) = axs[i,j].hist(timeDiff,bins = numBins,color='c')
	binCenters = np.linspace(min(timeDiff),max(timeDiff),numBins)
	


	x1_p, x1_co = curve_fit(gauss,binCenters,binCounts, p0=[10,0,n/10,0],maxfev=5000)
	
	axs[i,j].plot(binCenters,gauss(binCenters,*x1_p), color='k')
	axs[i,j].text(0.75,0.9,"Data Stats:", weight = 'bold', transform = axs[i,j].transAxes)
	axs[i,j].text(0.75,0.85,"mean: "+format(np.mean(timeDiff),"5.4f"), transform = axs[i,j].transAxes)
	axs[i,j].text(0.75,0.8,"std: "+format(np.std(timeDiff),"5.4f"), transform = axs[i,j].transAxes)
	axs[i,j].text(0.75,0.75,"count: "+str(len(timeDiff)), transform = axs[i,j].transAxes)
	axs[i,j].text(0.75,0.7,"Fit Paramters:", weight = 'bold', transform = axs[i,j].transAxes)
	axs[i,j].text(0.75,0.65,"mu: "+format(x1_p[1],"5.4f"), transform = axs[i,j].transAxes)
	axs[i,j].text(0.75,0.6,"sigma: "+format(x1_p[2],"5.4f"), transform = axs[i,j].transAxes)
	axs[i,j].text(0.75,0.55,"A: "+format(x1_p[0],"5.4f"), transform = axs[i,j].transAxes)
	axs[i,j].set_title("n = " + str(n))
	axs[i,j].set_xlabel("Time Dfference (ns)")
	axs[i,j].set_ylabel("Count")


	j=j+1
	if j == 2:
		i+=1
		j=0
fig.suptitle("Time Differences of nth Photons Between Pairs of Events With <1000 Hits")
plt.show()
	
