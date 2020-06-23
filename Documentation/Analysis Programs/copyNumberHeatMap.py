#Chris Layden
#Generate a heat map of hits on grid of detector cells
import numpy as np
import pandas as pd
import matplotlib
import matplotlib.pyplot as plt

df = pd.read_csv("PHOT_15_40x3_pix_time_g5.txt", sep=",", header=None, low_memory=False)
df.columns = ['EventID', 'CopyNumber', 'XPos','PhotTime']

detectorGridSize = 8
detectorNumList, rowNumList, colNumList = [ ], [ ], [ ]
# From copyNumber of each hit, determine the row and column of the detector that was hit
for row in df["CopyNumber"]:
	detectorNum = row % (detectorGridSize ** 2)
	detectorNumList.append(detectorNum)
	rowNumList.append(detectorNum // detectorGridSize)
	colNumList.append(detectorNum % detectorGridSize)
		
# Creating a 2d histogram, in the form of a numpy array, of number of hits in each detector cell
heatmap, xedges, yedges = np.histogram2d(colNumList, rowNumList, bins = detectorGridSize)


fig, ax = plt.subplots()
# Displaying the heat map. Must transpose it due to structure of the numpy array.
im = ax.imshow(heatmap.T)

# Create a colorbar legend
cbar = ax.figure.colorbar(im, ax=ax)
cbar.ax.set_ylabel("Hits", rotation=-90, va="bottom")


# Loop over detector grid dimensions and label the cells
k = 0
for i in range(detectorGridSize):
    for j in range(detectorGridSize):
        text = ax.text(j, i, k, ha="center", va="center", color="w")
        k += 1

ax.axis('off')
plt.show()
