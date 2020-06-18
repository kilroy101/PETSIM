#Kyle Klein
#takes photon data and fits an exponential decay to recover decay constant of the crystal

import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit

df=pd.read_csv("PHOT_BaSO4.txt", sep=",", header=None, low_memory=False)
df.columns=['EventID','CopyNumber','Xpos', 'Ypos','Zpos','Time']
PT = df.Time
PTime_mode = df.Time.mode()
PTime = df[df["Time"] > float(PTime_mode)]
data = PTime.Time

def function(x,a,b,c):
    e = 2.71828
    y = a*e**(-b*x) + c
    return y
bins = np.linspace(PTime_mode,0.15,100)
x = np.linspace(PTime_mode,0.15,99)
y = data.value_counts(bins=bins).tolist()

fig_p, fig_co = curve_fit(function,x,y, p0=[10000,0.22,150])
plt.scatter(x,y)
plt.plot(x,function(x,*fig_p))
plt.xlabel('Time in ns')
plt.ylabel('Counts')
plt.title('Total Photon Timing data with exponential fit')
plt.xlim(0,0.16)
plt.ylim(0,310000)
plt.text(0.12,290000,'Tau = '+format(fig_p[1]/2, '5.2f')+' ns')
plt.show()
