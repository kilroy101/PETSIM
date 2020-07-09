#Kyle Klein
#meant for micro+ geoemtry to understand photon distribution around the crystal
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit
from scipy.stats import norm

df=pd.read_csv("3x315.txt", sep=",", header=None, low_memory=False)
df.columns=['EventID', 'CopyNumber', 'XPos', 'YPos', 'ZPos', 'Time']

DATA = df.groupby(["EventID", "CopyNumber"]).size().unstack(fill_value=0).reset_index()
DATA.columns = ["EventID", "0","1","2","3","4","5","6","7","8","9"]
DATA["Total_Photon"] = DATA[["0","1","6","7","8","9"]].sum(axis=1)
DATA["Front"] = DATA["0"] / DATA["Total_Photon"]
DATA["Back"] = DATA["1"] / DATA["Total_Photon"]
DATA["Right_O"] = DATA["2"] / DATA["Total_Photon"]
DATA["Left_O"] = DATA["3"] / DATA["Total_Photon"]
DATA["Bottom_O"] = DATA["4"] / DATA["Total_Photon"]
DATA["Top_O"] = DATA["5"] / DATA["Total_Photon"]
DATA["Right_I"] = DATA["6"] / DATA["Total_Photon"]
DATA["Left_I"] = DATA["7"] / DATA["Total_Photon"]
DATA["Bottom_I"] = DATA["8"] / DATA["Total_Photon"]
DATA["Top_I"] = DATA["9"] / DATA["Total_Photon"]

# print(DATA["0"].sum())
# print(DATA["1"].sum())

# #individual histograms
fig1 = plt.figure()
fig2 = plt.figure()
fig3 = plt.figure()
fig4 = plt.figure()
fig5 = plt.figure()
fig6 = plt.figure()
fig7 = plt.figure()
fig8 = plt.figure()
fig9 = plt.figure()
fig10 = plt.figure()
ax1 = fig1.add_subplot(111,xlim=(0,1), ylim=(0,2500),xlabel="Ratio of hits on this face to total photons",ylabel="Counts",title="Front Detector")
ax2 = fig2.add_subplot(111,xlim=(0,1), ylim=(0,2500),xlabel="Ratio of hits on this face to total photons",ylabel="Counts",title="Back Detector")
ax3 = fig3.add_subplot(111,xlim=(0,0.0025), ylim=(0,3000),xlabel="Ratio of hits on this face to total photons",ylabel="Counts",title="Left Outside Detector")
ax4 = fig4.add_subplot(111,xlim=(0,0.0025), ylim=(0,3000),xlabel="Ratio of hits on this face to total photons",ylabel="Counts",title="Right Outside Detector")
ax5 = fig5.add_subplot(111,xlim=(0,0.0025), ylim=(0,3000),xlabel="Ratio of hits on this face to total photons",ylabel="Counts",title="Top Outside Detector")
ax6 = fig6.add_subplot(111,xlim=(0,0.0025), ylim=(0,3000),xlabel="Ratio of hits on this face to total photons",ylabel="Counts",title="Bottom Outside Detector")
ax7 = fig7.add_subplot(111,xlim=(0,1), ylim=(0,2500),xlabel="Ratio of hits on this face to total photons",ylabel="Counts",title="Left Inside Detector")
ax8 = fig8.add_subplot(111,xlim=(0,1), ylim=(0,2500),xlabel="Ratio of hits on this face to total photons",ylabel="Counts",title="Right Inside Detector")
ax9 = fig9.add_subplot(111,xlim=(0,1), ylim=(0,2500),xlabel="Ratio of hits on this face to total photons",ylabel="Counts",title="Top Inside Detector")
ax10 = fig10.add_subplot(111,xlim=(0,1), ylim=(0,2500),xlabel="Ratio of hits on this face to total photons",ylabel="Counts",title="Bottom Inside Detector")

text_x = 0.8
text_y = [2400,2300,2200]
ax1.hist(DATA.Front, bins=50, color='b')
ax1.text(text_x,text_y[0],"mean: "+format(DATA.Front.mean(),"5.2f"))
ax1.text(text_x,text_y[1],"std: "+format(DATA.Front.std(),"5.2f"))
ax1.text(text_x,text_y[2],"count: "+format(DATA.Front.count(),"4"))

ax2.hist(DATA.Back, bins=50, color='b')
ax2.text(text_x,text_y[0],"mean: "+format(DATA.Back.mean(),"5.2f"))
ax2.text(text_x,text_y[1],"std: "+format(DATA.Back.std(),"5.2f"))
ax2.text(text_x,text_y[2],"count: "+format(DATA.Back.count(),"4"))

ax3.hist(DATA.Left_O, bins=50, color='b')
ax3.text(text_x,text_y[0],"mean: "+format(DATA.Left_O.mean(),"5.2f"))
ax3.text(text_x,text_y[1],"std: "+format(DATA.Left_O.std(),"5.2f"))
ax3.text(text_x,text_y[2],"count: "+format(DATA.Left_O.count(),"4"))

ax4.hist(DATA.Right_O, bins=50, color='b')
ax4.text(text_x,text_y[0],"mean: "+format(DATA.Right_O.mean(),"5.2f"))
ax4.text(text_x,text_y[1],"std: "+format(DATA.Right_O.std(),"5.2f"))
ax4.text(text_x,text_y[2],"count: "+format(DATA.Right_O.count(),"4"))

ax5.hist(DATA.Top_O, bins=50, color='b')
ax5.text(text_x,text_y[0],"mean: "+format(DATA.Top_O.mean(),"5.2f"))
ax5.text(text_x,text_y[1],"std: "+format(DATA.Top_O.std(),"5.2f"))
ax5.text(text_x,text_y[2],"count: "+format(DATA.Top_O.count(),"4"))

ax6.hist(DATA.Bottom_O, bins=50, color='b')
ax6.text(text_x,text_y[0],"mean: "+format(DATA.Bottom_O.mean(),"5.2f"))
ax6.text(text_x,text_y[1],"std: "+format(DATA.Bottom_O.std(),"5.2f"))
ax6.text(text_x,text_y[2],"count: "+format(DATA.Bottom_O.count(),"4"))

ax7.hist(DATA.Left_I, bins=50, color='b')
ax7.text(text_x,text_y[0],"mean: "+format(DATA.Left_I.mean(),"5.2f"))
ax7.text(text_x,text_y[1],"std: "+format(DATA.Left_I.std(),"5.2f"))
ax7.text(text_x,text_y[2],"count: "+format(DATA.Left_I.count(),"4"))

ax8.hist(DATA.Right_I, bins=50, color='b')
ax8.text(text_x,text_y[0],"mean: "+format(DATA.Right_I.mean(),"5.2f"))
ax8.text(text_x,text_y[1],"std: "+format(DATA.Right_I.std(),"5.2f"))
ax8.text(text_x,text_y[2],"count: "+format(DATA.Right_I.count(),"4"))

ax9.hist(DATA.Top_I, bins=50, color='b')
ax9.text(text_x,text_y[0],"mean: "+format(DATA.Top_I.mean(),"5.2f"))
ax9.text(text_x,text_y[1],"std: "+format(DATA.Top_I.std(),"5.2f"))
ax9.text(text_x,text_y[2],"count: "+format(DATA.Top_I.count(),"4"))

ax10.hist(DATA.Bottom_I, bins=50, color='b')
ax10.text(text_x,text_y[0],"mean: "+format(DATA.Bottom_I.mean(),"5.2f"))
ax10.text(text_x,text_y[1],"std: "+format(DATA.Bottom_I.std(),"5.2f"))
ax10.text(text_x,text_y[2],"count: "+format(DATA.Bottom_I.count(),"4"))

plt.show()
