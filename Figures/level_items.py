import numpy as np
import matplotlib.pyplot as plt
import csv

data = np.genfromtxt('level_items.csv',delimiter=',', dtype = int)
print(data)

lvl = [row[0] for row in data]
print(lvl[0:2])
bfc = [row[1] for row in data]
bfc = bfc[0:3]
print(bfc)
cfc = [row[2] for row in data]
cfc = cfc[0:4]
print(cfc)
cfssc = [row[3] for row in data]
cfssc = cfssc[0:7]
print(cfssc)
vfc = [row[4] for row in data]
vfc = vfc[0:5]
print(vfc)
vfssc = [row[5] for row in data]
# vfssc = [x for x in vfssc if x != 0]
print(vfssc)

fig = plt.figure(figsize = (12, 9))
plt.plot(lvl[0:3], bfc[0:3], marker = "+", label="BF")
plt.plot(lvl[0:4], cfc[0:4], marker = "*", label="CF")
plt.plot(lvl[0:7], cfssc[0:7], marker = "*", label="CF-ss")
plt.plot(lvl[0:5], vfc[0:5], marker = "o", label="VF")
plt.plot(lvl, vfssc, marker = "v", label="VF-ss")



plt.legend(loc = "upper right", fontsize = 18, ncol = 2)
plt.xlabel("Filter Level", fontsize = 24)
#plt.ylabel("Bits per Item")
plt.ylabel("False Positives", fontsize = 24)
plt.savefig("level_items.png")
plt.show()

exit()

# x = []
# y = []

# with open('level_items.csv','r') as csvfile:
#     plots = csv.reader(csvfile, delimiter=',')
#     for row in plots:
#         x.append(int(row[0]))
#         y.append(int(row[1]))

# plt.plot(x,y, label='Loaded from file!')
# plt.xlabel('x')
# plt.ylabel('y')
# plt.title('Interesting Graph\nCheck it out')
# plt.legend()
# plt.show()