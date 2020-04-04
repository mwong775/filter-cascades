import numpy as np
import matplotlib.pyplot as plt
import csv

data = np.genfromtxt('level_item_bits.csv',delimiter=',', dtype = float)
print(data)

lvl = [row[0] for row in data]
print(lvl)
bfc = [row[1] for row in data]
bfc = [x for x in bfc if x != 0]
print(bfc)
cfc = [row[2] for row in data]
cfc = [x for x in cfc if x != 0]
print(cfc)
cfssc = [row[3] for row in data]
cfssc = [x for x in cfssc if x != 0]
print(cfssc)
vfc = [row[4] for row in data]
vfc = [x for x in vfc if x != 0]
print(vfc)
vfssc = [row[5] for row in data]
# vfssc = [x for x in vfssc if x != 0]
print(vfssc)

fig = plt.figure(figsize = (12, 9))
plt.plot(lvl[0:3], bfc, marker = "+", label="BF")
plt.plot(lvl[0:4], cfc, marker = "*", label="CF")
plt.plot(lvl[0:7], cfssc, marker = "*", label="CF-ss")
plt.plot(lvl[0:5], vfc, marker = "o", label="VF")
plt.plot(lvl[0:8], vfssc, marker = "v", label="VF-ss")



plt.legend(loc = "upper right", fontsize = 18, ncol = 2)
plt.xlabel("Filter Level", fontsize = 24)
#plt.ylabel("Bits per Item")
plt.ylabel("Bits per Item", fontsize = 24)
plt.savefig("level_item_bits.png")
plt.show()

exit()