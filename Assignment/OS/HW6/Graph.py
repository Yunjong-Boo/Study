import matplotlib.pyplot as plt
import numpy as np

cache_size = np.arange(1,5)
policies = ["FIFO", "LRU", "OPT", "RAND", "CLOCK"]
hitrate=[[50.86, 79.38, 88.35, 92.13],
        [50.86, 85.54, 90.62, 93.92],
        [50.86, 85.78, 93.03, 95.60],
        [50.86, 79.35, 87.52, 91.45],
        [50.86, 81.16, 89.10, 92.87]]

for i in range(len(policies)):
    plt.plot(cache_size, hitrate[i])

plt.legend(policies)
plt.margins(0)
plt.xticks(cache_size, cache_size)
plt.xlabel('Cache Size')
plt.ylabel('Hit Rate')
plt.savefig('Graph.png')
