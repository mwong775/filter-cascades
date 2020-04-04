import numpy as np
import matplotlib.pyplot as plt

input = open("key-memory.csv", 'r', encoding='utf-8')

key = []
sscf12 = []
mf = []
vf12 = []
vf11 = []
bf12 = []

for line in input.readlines():
    if line.find('key') != -1: continue
    cur_line = line.strip()
    row = [0, 0, 0, 0, 0, 0]
    row[0], row[1], row[2], row[3], row[4], row[5] = map(float, cur_line.split(','))
    if (row[0] < (1 << 11) * 1.5) :
        continue
    key.append(row[0])
    sscf12.append(row[1])
    mf.append(row[2] + row[0] * (5.0 / 8))
    vf12.append(row[3])
    vf11.append(row[4])
    bf12.append(row[5])
    if (len(key) == 900) : break

key = np.array(key)
sscf12 = np.array(sscf12)
mf = np.array(mf)
vf12 = np.array(vf12)
vf11 = np.array(vf11)
bf12 = np.array(bf12)

plt.figure(figsize = (12, 9))
plt.semilogx(key, sscf12 / (1 << 20), label = "Semi-Sort Cuckoo Filter", linewidth = 3)
#plt.plot(key, vf12, label = "Vacuum Filter 12")
plt.semilogx(key, bf12 / (1 << 20), label = "Bloom Filter", linewidth = 3)
#plt.semilogx(key, mf / (1 << 20), linestyle = '-', label = "Morton Filter", linewidth = 3)
plt.semilogx(key, vf12 / (1 << 20), label = "Vacuum Filter", linewidth = 3)
#print("hahaha")

plt.legend(loc = "upper left", fontsize = 28, frameon=False)
plt.xlabel("Number of Items", fontsize = 28)
#plt.ylabel("Bits per Item")
plt.ylabel("Memory Cost (MB)", fontsize = 28)

tick_values = [2**x for x in range(15, 21)]
plt.xlim(((1 << 15) * 0.75, 1 << 20))
print(tick_values)
plt.xticks(tick_values,[("2^%.0f" % np.log2(x))  for x in tick_values], fontsize = 22)
plt.yticks(np.linspace(0, 2.5, 6), fontsize = 22)
#plt.yticks(np.linspace(0, 1400000, 8), fontsize = 22)
#while
#plt.yticks(np.linspace(0, 1500000, 6), fontsize = 22)

#ytick = np.linspace(float(sscf12[0]), float(sscf12[-1]), 10)
#print(ytick)
#plt.ylim((sscf12[0], sscf12[-1]))
#plt.yticks(ytick)

plt.savefig("key-mem2.png")
#plt.show()

sscf12 = sscf12 / key * 8
mf = mf / key * 8
vf12 = vf12 / key * 8
vf11 = vf11 / key * 8
bf12 = bf12 / key * 8

print(vf12[0])
print(bf12[0])

plt.figure(figsize = (12, 9))
#plt.plot(key, sscf12, label = "Semi-Sort Cuckoo Filter", linewidth = 3)
#plt.plot(key, vf12, label = "Vacuum Filter 12")
#plt.plot(key, vf11, label = "Vacuum Filter", linewidth = 3)
#plt.plot(key, bf12, label = "Bloom Filter", linewidth = 3)
#plt.plot(key, mf, linestyle = '-', label = "Morton Filter", linewidth = 3)
plt.semilogx(key[::1], sscf12[::1], label = "Semi-Sort Cuckoo Filter", marker = 'o', linewidth = 3, markersize = 1)
#plt.plot(key, vf12, label = "Vacuum Filter 12")
#plt.semilogx(key[::1], mf[::1], linestyle = '-', label = "Morton Filter", marker = 'o', linewidth = 3,markersize = 1)
plt.semilogx(key[::1], bf12[::1], label = "Bloom Filter", linewidth = 3, marker = 'o', markersize = 1)
plt.semilogx(key[::1], vf12[::1], label = "Vacuum Filter", linewidth = 3, markersize = 1, marker = 'o')
#print("hahaha")

plt.legend(loc = "upper right", fontsize = 24, ncol = 2, frameon=False)
plt.xlabel("Number of Items", fontsize = 28)
plt.ylabel("Bits per Item", fontsize = 28)
#plt.ylabel("Memory Cost (Bytes)", fontsize = 24)

tick_values = [2**x for x in range(12, 21)]
print(tick_values)
plt.xticks(tick_values,[("2^%.0f" % np.log2(x))  for x in tick_values], fontsize = 22)
plt.xlim(((1 << 11) * 1.5, 1 << 20))
plt.yticks(np.linspace(10, 40, 6), fontsize = 22)
plt.savefig("key-mem.png")
exit()

plt.semilogx(w, bf, lw = 1.5, linestyle = "--", label = "Bloom Filter")
#plt.semilogx(w, vf, lw = 1.5, label = "Vacuum Filter")
#plt.semilogx(w, cf_best, lw = 1.5, linestyle = "-.", label = "Cuckoo Filter Best Case")
plt.semilogx(w, vf, lw = 1.5, color = "black", label = "Vacuum Filter / CF Best Case")
plt.semilogx(w, cf_avg, lw = 1.5, linestyle = "-", label = "Cuckoo Filter Average Case")
plt.semilogx(w, cf_worst, lw = 1.5, label = "Cuckoo Filter Worst Case")
plt.semilogx(w, low_bound, lw = 1, linestyle = ":", color = "black", label = "Lower Bound")
plt.xlabel("False Postive Rate")
plt.ylabel("Bits per Item")
plt.legend(loc = "upper right")
plt.show()

exit()

#print(abs_time)
print(velocity)
#print(height)
plt.subplot(4, 1, 1)
plt.plot(abs_time, a_z)
plt.ylabel('Acceleration : m/s^2')

plt.subplot(4, 1, 2)
plt.plot(abs_time, velocity)
plt.ylabel('Velocity : m/s')

plt.subplot(4, 1, 3)
plt.plot(abs_time, height)
plt.ylabel('Height : m')

plt.subplot(4, 1, 4)
plt.plot(abs_time, h)
plt.ylabel('Height_M : m')

plt.xlabel('time : s')
plt.show()

