from matplotlib import pyplot as plt

x = []
y = []
idx = 0

with open('./data.txt') as file:
    for line in file.readlines():
        x.append(int(line.split()[0]))
        y.append(float(line.split()[1]))
        idx += 1

x_axis = range(len(x))
plt.plot(x_axis, y, 'co-')
plt.xticks(x_axis, x)
plt.xlabel("Number Of Pages")
plt.ylabel("Time Per Access (ns)")

plt.title("TLB Size Measurement")
plt.savefig('Graph.png')
