from matplotlib import pyplot as plt
import numpy as np

x_memory = np.arange(20 , 180, 20)
y_memory = [7662.06, 7751.39, 7417.98, 7976.60, 5327.34, 5219.60, 3892.88, 4212.04]
fig = plt.figure()
plt.plot(x_memory, y_memory, 'co-')
plt.xlabel("Size of Memory(GB)")
plt.ylabel("Average bandwidth(MB/s)")
plt.savefig("memory.png")

plt.clf()

x_loop = np.arange(0, 10)
y1_loop = [2048.77, 8227.81, 8202.35, 8229.10, 8225.38, 
        8196.66, 8274.67, 8236.80, 8298.02, 8281.12]
x = np.arange(10)
y2_loop = [2174.11, 8342.34, 8346.19, 8359.67, 8402.64,
        8369.74, 8398.45, 8419.97, 8361.28, 8339.54]
y3_loop = [2154.68, 3010.30, 8663.53, 8633.24, 8604.70, 
        8643.40, 8574.57, 8665.32, 8582.89, 8647.25]
y4_loop = [2207.72, 8628.90, 8610.43, 8596.81, 8601.40,
        8606.44, 8624.96, 8644.67, 8621.63, 8623.06]
y5_loop = [2160.84, 2916.82, 8258.31, 7598.48, 1667.91,
        5781.89, 3758.16, 7610.42, 7846.33, 5674.25]
y6_loop = [2004.68, 5945.92, 6065.54, 7797.95, 6950.90,
        1844.50, 6074.44, 8076.74, 3014.26, 4421.14]
y7_loop = [2032.07, 4495.53, 2880.27, 5358.49, 3998.58,
        5208.52, 4090.89, 3154.74, 5089.08, 2620.68]
y8_loop = [1853.60, 5616.32, 3390.88, 4713.20, 4990.80,
        6946.96, 4557.91, 3453.64, 3205.83, 3391.32]
plt.plot(x_loop, y1_loop)
plt.plot(x, y2_loop)
plt.plot(x, y3_loop)
plt.plot(x, y4_loop)
plt.plot(x, y5_loop)
plt.plot(x, y6_loop)
plt.plot(x, y7_loop)
plt.plot(x, y8_loop)
plt.legend(['20GB', '40GB', '60GB', '80GB', '100GB', '120GBB', '140GB', '160GB'])
plt.xlabel("The number of Loops")
plt.ylabel("Bandwidth(MB/s)")
plt.savefig("loops.png")
