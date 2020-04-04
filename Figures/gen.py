import numpy as np
import matplotlib.pyplot as plt

#w = np.linspace(0.000001, 0.5, 1000000);
w = np.array([0.0001, 0.001])
bf = 1.44 * np.log2(1.0 / w)
cf_best = ( np.log2(8 * 0.955 / w) - 1) / 0.955
cf_avg = ( np.log2(8 * 0.75 / w) - 1) / 0.75
cf_worst = ( np.log2(8 * 0.5 / w) - 1) / 0.5
vf = ( np.log2(8 * 0.97/ w) - 1) / 0.97
low_bound =  - np.log2(w)

print(bf)
print(vf)

plt.semilogx(w, bf, lw = 1.5, linestyle = "--", label = "Bloom Filter")
#plt.semilogx(w, vf, lw = 1.5, label = "Vacuum Filter")
#plt.semilogx(w, cf_best, lw = 1.5, linestyle = "-.", label = "Cuckoo Filter Best Case")
plt.semilogx(w, vf, lw = 1.5, color = "black", label = "Vacuum Filter / CF Best Case")
plt.semilogx(w, cf_avg, lw = 1.5, linestyle = "-", label = "Cuckoo Filter Average Case")
plt.semilogx(w, cf_worst, lw = 1.5, label = "Cuckoo Filter Worst Case", color = "green")
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

