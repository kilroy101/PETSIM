#Chris Layden
#Determine the energy resolution of a detector

import numpy as np
import pandas as pd
import matplotlib
import matplotlib.pyplot as plt
from iminuit import Minuit
from iminuit.cost import UnbinnedNLL, LeastSquares

def gauss(x,a,mu,sigma,c):
    y = a*np.exp(-(x - mu)**2 / (2 * sigma**2))+c
    return y

df = pd.read_csv("3x315.txt", sep=",", header=None, low_memory=False)
# df = pd.read_csv("PHOT_15_40x3_pix_time_g5.txt", sep=",", header=None, low_memory=False)
df.columns = ['EventID', 'CopyNumber', 'ZPos','YPos','XPos','PhotTime','Wavelength']

hPlanckTimesc = 1239.842
df["Energy"] = np.power(df["Wavelength"], -1) * hPlanckTimesc

eventEnergies = []
energyVariances = []
variancesOverEnergy = []
eventByEvent = df.groupby('EventID')
for index, frame in eventByEvent:
	eventEnergy = frame.Energy.sum()
	energyVariance = np.var(frame.Energy)
	varianceOverEnergy = energyVariance / eventEnergy
	eventEnergies.append(eventEnergy)
	energyVariances.append(energyVariance)
	variancesOverEnergy.append(varianceOverEnergy)
	
photopeakEnergies = [i for i in eventEnergies if i > 7500]

	
numBins = 50
fig, ax = plt.subplots()
hist, xedges = np.histogram(eventEnergies, bins = 100)
photoPeak = xedges[hist.argmax()]

(binCounts, bins, patches) = ax.hist(photopeakEnergies, bins = numBins, range = [photoPeak - 1000, photoPeak + 1000])
binCenters = np.linspace(min(bins),max(bins),numBins)

k = Minuit(LeastSquares(binCenters, binCounts, 10.0, gauss), a = 100, mu = photoPeak, sigma = 100, c = 0)
k.migrad()
k.hesse()
energyResolution = k.values[2] / k.values[1] * 100

fitPoints = np.linspace(min(bins), max(bins), 1000)
ax.plot(fitPoints,gauss(fitPoints,*k.values[0:4]), color='k')
ax.text(0.7,0.9,"Data Stats:", weight = 'bold', transform = ax.transAxes)
ax.text(0.7,0.85,"mean: "+format(np.mean(photopeakEnergies),"5.4f"), transform = ax.transAxes)
ax.text(0.7,0.8,"std: "+format(np.std(photopeakEnergies),"5.4f"), transform = ax.transAxes)
ax.text(0.7,0.75,"count: "+str(len(photopeakEnergies)), transform = ax.transAxes)
ax.text(0.7,0.7,"Fit Paramters:", weight = 'bold', transform = ax.transAxes)
ax.text(0.7,0.65,"A: "+format(k.values[0],"5.4f"), transform = ax.transAxes)
ax.text(0.7,0.6, r"$\mu:$ "+format(k.values[1],"5.4f"), transform = ax.transAxes)
ax.text(0.7,0.55,r"$\sigma:$ "+format(abs(k.values[2]),"5.4f"), transform = ax.transAxes)
ax.text(0.7,0.5,"FWHM: " + format(2.355 * abs(k.values[2]),"5.4f"), transform = ax.transAxes)
ax.text(0.7,0.45,"Energy Resolution: " + format(energyResolution,"3.2f") + "%", transform = ax.transAxes)
# ax.text(0.75,0.4,"Fit Quality:", weight = 'bold', transform = ax.transAxes)
# ax.text(0.75,0.35, r"$\chi^{2}:$"+format(chisquare,"5.4f"), transform = axs.transAxes)
ax.set_title("Energy Resolution of 511 keV Gamma Photopeak\nDR, 30mm crystal, 3mm Detectors")
ax.set_ylim(bottom = 0)


ax.set(xlabel = "Detected Energy (eV)", ylabel = "Counts")
plt.show()
