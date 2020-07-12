#Chris Layden
#Generate a 2D histogram of hits on detectors and 1D projections
import numpy as np
import pandas as pd
import matplotlib
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit

df = pd.read_csv("BaSO4_outer/copyNumber1.txt", sep=",", header=None, low_memory=False)
df.columns = ['EventID', 'CopyNumber', 'ZPos','XPos','YPos','PhotTime']
#Choose which detector to look at. 0 = front, 1 = back, 2 = right outer, 3 = left outer, 4 = bottom outer, 5 = top outer
#6 = right inner, 7 = left inner, 8 = bottom inner, 9 = top
backCrystal = df[df.CopyNumber == 1]
xPos = backCrystal['XPos']
yPos = backCrystal['YPos']
zPos = backCrystal['ZPos']
#Make sure to adjust which positions are used when plotting histograms based on which detector you're looking at


fig = plt.figure(figsize=(8, 8))
gs = fig.add_gridspec(4, 4)
ax1 = fig.add_subplot(gs[0:3, 1:4], projection = '3d')
ax2 = fig.add_subplot(gs[3, 1:4])
ax3 = fig.add_subplot(gs[0:3, 0])

numBins = 12
#remove range below if not looking at front or back detectors
hist, xedges, yedges = np.histogram2d(xPos, yPos, range = [[-3,3],[-3,3]], bins = numBins)
# Construct arrays for the anchor positions of the bars.
xpos, ypos = np.meshgrid(xedges[:-1], yedges[:-1],indexing = "ij")
xpos = xpos.ravel()
ypos = ypos.ravel()
bottom = 0
# Construct arrays with the dimensions for the bars.
dx = (xedges[-1] - xedges[0] ) / numBins
dy = (yedges[-1] - yedges[0] ) / numBins
top = hist.ravel()

ax1.bar3d(xpos, ypos, bottom, dx, dy, top, zsort='average',color = "w", edgecolor = "k")
ax1.set_xlabel("x (mm)")
ax1.set_ylabel("y (mm)")
ax1.set_zlabel("Entries")
ax1.ticklabel_format(axis = 'z', style="sci", scilimits=(0,0))


ax2.hist(xPos, bins = 100, histtype = 'step', color = 'k')
ax2.set_xlabel("x (mm)")
ax2.set_ylabel("Entries")
ax2.ticklabel_format(axis = 'y', style="sci", scilimits=(0,0))
ax2.text(0.05, 0.75, "mean: "  + format(np.mean(xPos), "4.4f")  + "\nstd: "  + format(np.std(xPos), "4.4f") , transform = ax2.transAxes)
 
 
ax3.hist(yPos, bins = 100, histtype = 'step', color = 'k', orientation = u'horizontal')
ax3.set_ylabel("y (mm)", rotation = 270)
ax3.set_xlabel("Entries")
ax3.ticklabel_format(axis = 'x', style="sci", scilimits=(0,0))
ax3.text(0.05, 0.9, "mean: "  + format(np.mean(yPos), "4.4f")  + "\nstd: "  + format(np.std(yPos), "4.4f") , transform = ax3.transAxes)


fig.suptitle("Position of Detector Hits - BaSO4 \nEvents: 5000   Total Entries: " + str(len(yPos)), fontsize = 20)

plt.show()
