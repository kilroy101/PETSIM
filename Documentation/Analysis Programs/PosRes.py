#Kyle Klein
#Description: Inputs optical photon data taken from GEANT4 and displays the position resolution of each event 

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

df=pd.read_csv("3x315.txt", sep=",", header=None, low_memory=False)
df.columns=['EventID', 'CopyNumber', 'XPos', 'YPos', 'ZPos', 'Time']

Det_limit = int(64)
Time_limit = int(5)
Pos_limit = int(0)

Bottom  = df[df['XPos'] >= Pos_limit]
Top = df[df['XPos'] <= Pos_limit]

Bottom_Near = Bottom[Bottom['CopyNumber'] <= Det_limit]
Bottom_Far = Bottom[Bottom['CopyNumber'] >= Det_limit]
Top_Near = Top[Top['CopyNumber'] <= Det_limit]
Top_Far = Top[Top['CopyNumber'] >= Det_limit]

STD_y = Bottom.groupby('EventID').YPos.std()
STD_z = Bottom.groupby('EventID').ZPos.std()

STD_y = STD_y/2.235
STD_z = STD_z/2.235


hist_y = STD_y.hist(bins=40)

plt.xlim(0,1.5)
plt.ylim(0,1000)
plt.xlabel('Position Resolution')
plt.ylabel('counts')
plt.title('Position Resolution along the Y axis')
plt.grid()
plt.text(1.2,950,"mean: 0.56")
plt.text(1.2,900,"std: 0.13")
plt.text(1.2,850,"entries: 1211")
plt.show()
print(STD_y.mean())
print(STD_y.std())
print(STD_y.count())
