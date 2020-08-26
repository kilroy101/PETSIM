#Chris Layden
import numpy as np
import pandas as pd
import matplotlib
import matplotlib.pyplot as plt
import mpl_toolkits.mplot3d as a3

cm = plt.cm.get_cmap('RdYlBu')
crystalLength = 30

sides = [[ [163.91,  0.1, 0.1], [163.91, 3.1, 0.1], [163.91 + crystalLength, 3.1, 0.1], [163.91 + crystalLength, 0.1, 0.1] ], \
				[ [163.91,  0.1, 3.1], [163.91, 3.1, 3.1], [163.91 + crystalLength, 3.1, 3.1], [163.91 + crystalLength, 0.1, 3.1] ], \
				[ [163.91,  0.1, 0.1], [163.91, 0.1, 3.1], [163.91 + crystalLength, 0.1, 3.1], [163.91 + crystalLength, 0.1, 0.1] ], \
				[ [163.91,  3.1, 0.1], [163.91, 3.1, 3.1], [163.91 + crystalLength, 3.1, 3.1], [163.91 + crystalLength, 3.1, 0.1] ], \
				[ [163.91,  0.1, 0.1], [163.91, 3.1, 0.1], [163.91, 3.1, 3.1], [163.91, 0.1, 3.1] ], \
				[ [163.91 + crystalLength,  0.1, 0.1], [163.91 + crystalLength, 3.1, 0.1], [163.91 + crystalLength, 3.1, 3.1], [163.91 + crystalLength, 0.1, 3.1] ]]


df = pd.read_csv("DETAILEDTRACKING3x315.txt", sep=",", header=None, low_memory=False, nrows = 999999)
df.columns = ['EventID','TrackID','PostPV', 'ZPos','YPos','XPos','PhotTime','Wavelength','StepNumber']

df1=pd.read_csv("3x315.txt", sep=",", header=None, low_memory=False)
df1.columns=['EventID','TrackID', 'CopyNumber', 'XPos', 'YPos','ZPos','Time', 'Wavelength']
EventPhotons = df[df.EventID == 0]
EventDetectedPhotons = df1[df1.EventID == 0]
EventDetectedPhotons = EventDetectedPhotons.sort_values(by = ['Time'])
trackIDList = EventDetectedPhotons.TrackID.drop_duplicates().tolist()
photonNum = 1
for currentTrackID in trackIDList:
	fig = plt.figure()
	ax = fig.add_subplot(projection = '3d')
	ax.set_xlim([160, 165 + crystalLength])
	ax.set_ylim([-0.5, 3.7])
	ax.set_zlim([-0.5,3.7])
	onePhoton = EventPhotons[EventPhotons.TrackID == currentTrackID]
	onePhoton = onePhoton.drop_duplicates(subset = ["ZPos", "YPos", "XPos"])
	zPositions = onePhoton.ZPos.tolist()
	yPositions = onePhoton.YPos.tolist()
	xPositions = onePhoton.XPos.tolist()
	ax.plot(zPositions, yPositions, xPositions, color = 'r')
	ax.scatter(zPositions, yPositions, xPositions, color = 'k')
	print(onePhoton.PhotTime.head(1))
	ax.set_title("Detected Photon Number " + str(photonNum))

	for k in range(len(zPositions)):
		ax.text(zPositions[k], yPositions[k], xPositions[k], str(k))
		# ax.annotate(k, (zPositions[k], yPositions[k], xPositions[k]))

	    

	for j in range(len(sides)):
	    vtx = sides[j]
	    shapes1 = a3.art3d.Poly3DCollection([vtx], alpha = 0.1)
	    shapes1.set_color('g')
	    shapes1.set_edgecolor('k')
	    ax.add_collection3d(shapes1)

	
	

	# plt.suptitle("Single-Gamma Optical Photon Outcomes: " + str(crystalLength) + " mm Crystal, 3.135 mm Detectors", y = 0.75, fontsize = 20)
	plt.show()
	photonNum+=1
