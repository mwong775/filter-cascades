import numpy as np
import matplotlib.pyplot as plt

input = open("throughput_del.csv", 'r', encoding='utf-8')

key = []
va = []
fb_va_ss = []
fb_std_ss = []
fb_va = []
fb_std = []

for line in input.readlines():
    if line.find('occ') != -1: continue
    cur_line = line.strip()
    row = [0, 0, 0, 0, 0, 0, 0]
    row[0], row[1], row[2], row[3], row[4], row[5] = map(float, cur_line.split(',')[0:-1])
    key.append(row[0])
    va.append(row[1])
    fb_va_ss.append(row[2])
    fb_std_ss.append(row[3])
    fb_va.append(row[4])
    fb_std.append(row[5])

key = np.array(key)
va = np.array(va)
fb_va_ss = np.array(fb_va_ss)
fb_std_ss = np.array(fb_std_ss)
fb_va = np.array(fb_va)
fb_std = np.array(fb_std)

plt.figure(figsize = (12, 9))
plt.plot(key, fb_std_ss, marker = "v", label = "CF-ss (Padding)", markersize = 12)
plt.plot(key, fb_va_ss, marker = "^", label = "VF-ss (Padding)", markersize = 12)
plt.plot(key, va, marker = "o", markersize = 12, label = "VF-ss (No Padding)")
plt.plot(key, fb_std, marker = "+", label = "CF", markersize = 12)
plt.plot(key, fb_va, marker = "*", label = "VF", markersize = 12)

plt.legend(loc = "upper left", ncol = 2, fontsize = 28, handletextpad = 0.8)
plt.xlabel("Occupancy", fontsize = 28)
#plt.ylabel("Bits per Item")
plt.ylabel("Delete Throughput (MOPS)", fontsize = 28)

#t = key[-1]
#xtick = np.linspace(float(key[0]), float(t), 8)
#print(xtick)
plt.xlim((-0.03, 1))
plt.xticks(np.linspace(0, 1, 5), fontsize = 28)
plt.yticks(np.linspace(0, 25, 6), fontsize = 28)

#ytick = np.linspace(float(sscf12[0]), float(sscf12[-1]), 10)
#print(ytick)
#plt.ylim((sscf12[0], sscf12[-1]))
#plt.yticks(ytick)

#fig = matplotlib.pyplot.gcf()
#fig.set_size_inches(18.5, 10.5)
#fig.savefig('test2png.png', dpi=100)
#plt.set_size_inches(18.5, 10.5)
plt.savefig("del-throughput.png")
#plt.show()

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

