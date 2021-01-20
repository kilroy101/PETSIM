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


df = pd.read_csv("3x315.txt", sep=",", header=None, low_memory=False, nrows = 999999)
# df = pd.read_csv("PHOT_15_40x3_pix_time_g5.txt", sep=",", header=None, low_memory=False)
df.columns = ['EventID', 'CopyNumber', 'ZPos','YPos','XPos','PhotTime','Wavelength']


df1 = pd.read_csv("KILLED3x315.txt", sep=",", header=None, low_memory=False, nrows = 999999)
df1.columns = ['EventID',  'ZPos','YPos','XPos','Time','Wavelength','StepNumber','KillProcess']
df1Absorption = df1[df1.KillProcess == 'OpAbsorption']
df1Transportation = df1[df1.KillProcess == 'Transportation']
df1Other = df1[(df1.KillProcess != 'OpAbsorption' ) & (df1.KillProcess != 'Transportation')]

df2 = pd.read_csv("PRODUCED3x315.txt", sep=",", header=None, low_memory=False, nrows = 999999)
df2.columns = ['EventID',  'ZPos','YPos','XPos','Time','Wavelength']

df3 = pd.read_csv("DETECTORLOSS3x315.txt", sep=",", header=None, low_memory=False, nrows = 999999)
df3.columns = ['EventID',  'ZPos','YPos','XPos','Time','Wavelength','StepNumber','KillProcess']


for i in range(100):
	# fig = plt.figure(figsize=(7, 9))
	# gs = fig.add_gridspec(7, 9)
	# ax1 = fig.add_subplot(gs[0:3, 1:4], projection = '3d')
	# ax2 = fig.add_subplot(gs[0:3, 5:8], projection = '3d')
	# ax3 = fig.add_subplot(gs[4:7, 0:3], projection = '3d')
	# ax4 = fig.add_subplot(gs[4:7, 3:6], projection = '3d')
	# ax5 = fig.add_subplot(gs[4:7, 6:9], projection = '3d')
	fig = plt.figure(figsize=(3, 19))
	gs = fig.add_gridspec(3, 19)
	ax1 = fig.add_subplot(gs[0:3, 0:3], projection = '3d')
	ax2 = fig.add_subplot(gs[0:3, 4:7], projection = '3d')
	ax3 = fig.add_subplot(gs[0:3, 8:11], projection = '3d')
	ax4 = fig.add_subplot(gs[0:3, 12:15], projection = '3d')
	ax5 = fig.add_subplot(gs[0:3, 16:19], projection = '3d')
	
	producedPhots = df2[df2.EventID == i]
	producedPhots = producedPhots.sort_values('Time')
	producedScatter = ax1.scatter(producedPhots.ZPos, producedPhots.YPos, producedPhots.XPos, s = 2, color = 'g')
	
	detectedPhots = df[df.EventID == i]
	detectedScatter = ax2.scatter(detectedPhots.ZPos, detectedPhots.YPos, detectedPhots.XPos, s = 2, color = 'k')
	
	killedPhots = df1[df1.EventID == i]
	# killedPhotsInCrystal = killedPhots[(killedPhots.ZPos >= 163.91) & (killedPhots.ZPos <= 178.91) & (killedPhots.YPos >= 0.1) & (killedPhots.YPos <= 3.1) & (killedPhots.XPos >= 0.1) & (killedPhots.XPos <= 3.1) ]
	# killedPhotsNotInCrystal = killedPhots[(killedPhots.ZPos < 163.91) | (killedPhots.ZPos > 178.91) | (killedPhots.XPos < 0.1) | (killedPhots.XPos > 3.1) | (killedPhots.YPos < 0.1) | (killedPhots.YPos > 3.1)]
	killedAbsorptionPhots = df1Absorption[df1Absorption.EventID == i]
	killedTransportationPhots = df1Transportation[df1Transportation.EventID == i]
	# killedOtherPhots = df1Other[df1Other.EventID == i]
	killedAbsorptionScatter = ax3.scatter(killedAbsorptionPhots.ZPos, killedAbsorptionPhots.YPos, killedAbsorptionPhots.XPos, s = 2, color = 'b', label = 'Absorbed in Crystal')
	killedTransportationScatter = ax4.scatter(killedTransportationPhots.ZPos, killedTransportationPhots.YPos, killedTransportationPhots.XPos, s = 2, color = 'r', label = 'Absorbed in Separator')
	# killedOtherScatter = ax3.scatter(killedOtherPhots.ZPos, killedOtherPhots.YPos, killedOtherPhots.XPos, s = 2, color = 'g', label = 'Other')
	# cbar = plt.colorbar(photonScatter)
	# cbar.set_label("Time of Photon Creation (ns)", rotation = 270, labelpad = 15)
	
	detectorLossPhots = df3[df3.EventID == i]
	detectorLossScatter = ax5.scatter(detectorLossPhots.ZPos, detectorLossPhots.YPos, detectorLossPhots.XPos, s = 2, color = 'magenta')

	for j in range(len(sides)):
	    vtx = sides[j]
	    shapes1 = a3.art3d.Poly3DCollection([vtx], alpha = 0.1)
	    shapes1.set_color('g')
	    shapes1.set_edgecolor('k')
	    ax1.add_collection3d(shapes1)
	    shapes2 = a3.art3d.Poly3DCollection([vtx], alpha = 0.1)
	    shapes2.set_color('g')
	    shapes2.set_edgecolor('k')
	    ax2.add_collection3d(shapes2)
	    shapes3 = a3.art3d.Poly3DCollection([vtx], alpha = 0.1)
	    shapes3.set_color('g')
	    shapes3.set_edgecolor('k')
	    ax3.add_collection3d(shapes3)
	    shapes4 = a3.art3d.Poly3DCollection([vtx], alpha = 0.1)
	    shapes4.set_color('g')
	    shapes4.set_edgecolor('k')
	    ax4.add_collection3d(shapes4)
	    shapes5 = a3.art3d.Poly3DCollection([vtx], alpha = 0.1)
	    shapes5.set_color('g')
	    shapes5.set_edgecolor('k')
	    ax5.add_collection3d(shapes5)
	    
	ax1.set_xlim([160, 165 + crystalLength])
	ax1.set_ylim([-0.5, 3.7])
	ax1.set_zlim([-0.5,3.7])
	ax1.set_title("Produced:  " + str(len(producedPhots)))
	ax2.set_xlim([160, 165 + crystalLength])
	ax2.set_ylim([-0.5, 3.7])
	ax2.set_zlim([-0.5,3.7])
	ax2.set_title("Detected:  " + str(len(detectedPhots)))
	ax3.set_xlim([160, 165 + crystalLength])
	ax3.set_ylim([-0.5, 3.7])
	ax3.set_zlim([-0.5,3.7])
	ax3.set_title("Absorbed in Crystal:  " + str(len(killedAbsorptionPhots)))
	ax4.set_xlim([160, 165 + crystalLength])
	ax4.set_ylim([-0.5, 3.7])
	ax4.set_zlim([-0.5,3.7])
	ax4.set_title("Absorbed in Separator:  " + str(len(killedTransportationPhots)))
	ax5.set_xlim([160, 165 + crystalLength])
	ax5.set_ylim([-0.5, 3.7])
	ax5.set_zlim([-0.5,3.7])
	ax5.set_title("Lost in Detector:  " + str(len(detectorLossPhots)))
	

	plt.suptitle("Single-Gamma Optical Photon Outcomes: " + str(crystalLength) + " mm Crystal, 3.135 mm Detectors", y = 0.75, fontsize = 20)
	plt.show()
	# input("Continue?")
