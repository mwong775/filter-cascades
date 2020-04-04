import numpy as np
import matplotlib.pyplot as plt

input = open("throughput_lookup.csv", 'r', encoding='utf-8')

key = []
va = []
fb_va_ss = []
fb_std_ss = []
fb_va = []
fb_std = []
bloom = []

va1 = []
fb_va_ss1 = []
fb_std_ss1 = []
fb_va1 = []
fb_std1 = []
bloom1 = []

for line in input.readlines():
    if line.find('occ') != -1: continue
    cur_line = line.strip()
    row = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
    row[0], row[1], row[2], row[3], row[4], row[5], row[6], row[7], row[8], row[9], row[10], row[11], row[12] = map(float, cur_line.split(',')[0:-1])
    key.append(row[0])
    va.append(row[1])
    va1.append(row[2])
    #bloom.append(row[3])
    #bloom1.append(row[4])
    bloom.append(3)
    bloom1.append(3)
    fb_va_ss.append(row[5])
    fb_va_ss1.append(row[6])
    fb_std_ss.append(row[7])
    fb_std_ss1.append(row[8])
    fb_va.append(row[9])
    fb_va1.append(row[10])
    fb_std.append(row[11])
    fb_std1.append(row[12])

key = np.array(key)
va = np.array(va)
fb_va_ss = np.array(fb_va_ss)
fb_std_ss = np.array(fb_std_ss)
fb_va = np.array(fb_va)
fb_std = np.array(fb_std)
bloom = np.array(bloom)

plt.figure(figsize = (12, 9))
plt.plot(key, fb_std, marker = "+", label = "CF", markersize = 12)
plt.plot(key, fb_va, marker = "*", label = "VF", markersize = 12)
plt.plot(key, bloom, marker = "s", label = "BF", markersize = 12)
plt.plot(key, fb_std_ss, marker = "v", label = "CF-ss (Padding)", markersize = 12)
plt.plot(key, fb_va_ss, marker = "^", label = "VF-ss (Padding)", markersize = 12)
plt.plot(key, va, marker = "o", label = "VF-ss (No Padding)", markersize = 12)

plt.legend(loc = "upper right", fontsize = 28, ncol = 2)
plt.xlabel("Occupancy", fontsize = 28)
#plt.ylabel("Bits per Item")
plt.ylabel("Negative Lookup Throughput (MOPS)", fontsize = 28)

#plt.xticks(key[::2], fontsize = 28)
plt.xlim((-0.03, 1))
plt.xticks(np.linspace(0, 1, 5), fontsize = 28)
plt.yticks(np.linspace(0, 30, 6), fontsize = 28)

plt.savefig("look-neg-throughput.png")
#plt.show()

plt.figure(figsize = (12, 9))
plt.plot(key, fb_std1, marker = "+", label = "CF", markersize = 12)
plt.plot(key, fb_va1, marker = "*", label = "VF", markersize = 12)
plt.plot(key, bloom1, marker = "s", label = "BF", markersize = 12)
plt.plot(key, fb_std_ss1, marker = "v", label = "CF-ss (Padding)", markersize = 12)
plt.plot(key, fb_va_ss1, marker = "^", label = "VF-ss (Padding)", markersize = 12)
plt.plot(key, va1, marker = "o", label = "VF-ss (No Padding)", markersize = 12)

plt.legend(loc = "upper right", fontsize = 28, ncol = 2)
plt.xlabel("Occupancy", fontsize = 28)
#plt.ylabel("Bits per Item")
plt.ylabel("Positive Lookup Throughput (MOPS)", fontsize = 28)

plt.xlim((-0.03, 1))
plt.xticks(np.linspace(0, 1, 5), fontsize = 28)
plt.yticks(np.linspace(0, 30, 6), fontsize = 28)

plt.savefig("look-pos-throughput.png")
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

