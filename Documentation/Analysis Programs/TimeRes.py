#Kyle Klein
#Description: Inputs optical photon data taken from GEANT4 and shows timing resolution
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

df=pd.read_csv("3x315.txt", sep=",", header=None, low_memory=False)
df.columns=['EventID', 'CopyNumber', 'XPos', 'YPos', 'ZPos', 'Time']

Det_limit = int(64)
Time_limit = int(5)
Pos_limit = int(0)

df = df[df['Time'] <= Time_limit]
Bottom  = df[df['XPos'] >= Pos_limit]
Top = df[df['XPos'] <= Pos_limit]

Bottom_Near = Bottom[Bottom['CopyNumber'] <= Det_limit]
Bottom_Far = Bottom[Bottom['CopyNumber'] >= Det_limit]
Top_Near = Top[Top['CopyNumber'] <= Det_limit]
Top_Far = Top[Top['CopyNumber'] >= Det_limit]

BNMean = Bottom_Near.groupby('EventID').Time.mean()
BFMean = Bottom_Far.groupby('EventID').Time.mean()
TNMean = Top_Near.groupby('EventID').Time.mean()
TFMean = Top_Far.groupby('EventID').Time.mean()

BNMean = BNMean.rename_axis('EventID1').reset_index()
BNMean.columns=['EventID', 'Time']

BFMean = BFMean.rename_axis('EventID1').reset_index()
BFMean.columns=['EventID', 'Time']

TNMean = TNMean.rename_axis('EventID1').reset_index()
TNMean.columns=['EventID', 'Time']

TFMean = TFMean.rename_axis('EventID1').reset_index()
TFMean.columns=['EventID', 'Time']

TopTime = pd.merge(TFMean, TNMean, on='EventID')
TopTime["TopTime"] = 0.5*(TopTime["Time_x"] - TopTime["Time_y"])
TopTime.TopTime = TopTime.TopTime.abs()

BotTime = pd.merge(BFMean, BNMean, on='EventID')
BotTime["BotTime"] = 0.5*(BotTime["Time_x"] - BotTime["Time_y"])
BotTime.BotTime = BotTime.BotTime.abs()

time = pd.DataFrame()
time["TRes"] = TopTime["TopTime"] - BotTime["BotTime"]
time.TRes = time.TRes.abs()

tRes = time.TRes.hist(bins=30)

plt.xlim(0,0.1)
plt.ylim(0,1000)
plt.xlabel('Time in ns')
plt.ylabel('counts')
plt.title("Time Difference")
plt.text(0.08,950,"mean: 0.021")
plt.text(0.08,900,"std: 0.014")
plt.text(0.08,850,"entries: 1207")
plt.grid()
plt.show()
print(time.TRes.mean())
print(time.TRes.std())
print(time.TRes.count())
