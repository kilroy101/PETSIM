import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

df=pd.read_csv("3x315.txt", sep=",", header=None, low_memory=False)
df.columns=['EventID', 'CopyNumber', 'XPos', 'YPos', 'ZPos', 'Time']

Det_limit = int(0)
Time_limit = int(5)
Pos_limit = int(0)

#df = df[df['Time'] <= Time_limit]
Bottom  = df[df['XPos'] >= Pos_limit]
Top = df[df['XPos'] <= Pos_limit]

Bottom_Near = Bottom[Bottom['CopyNumber'] <= Det_limit]
Bottom_Far = Bottom[Bottom['CopyNumber'] >= Det_limit]
Top_Near = Top[Top['CopyNumber'] <= Det_limit]
Top_Far = Top[Top['CopyNumber'] >= Det_limit]

DATA = Bottom_Far.EventID.value_counts()

hist, bins, placeholder = plt.hist(DATA,log=True,bins=50)
plt.clf()
logbins = np.logspace(np.log10(bins[0]),np.log10(bins[-1]),len(bins))
plt.hist(DATA,log=True,bins=logbins)
plt.xscale('log')
plt.ylim(0,3000)
plt.xlabel("# of photons detected")
plt.ylabel("Counts")
plt.title("Photons Detected in an Event: BaSO4")
plt.text(6300,2000,"mean: "+format(DATA.mean(),'7.2f'))
plt.text(6300,1550,"std: "+format(DATA.std(),'7.2f'))
plt.text(6300,1200,"count: "+format(DATA.count(),'7.2f'))
plt.show()
