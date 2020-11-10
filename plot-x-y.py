import numpy as np
import matplotlib.pyplot as plt

data_set = np.loadtxt(
    fname="tmp2",
    dtype="float",
    delimiter=",",
)

x = []
y = []

for data in data_set:
    #plt.scatter(data[0], data[1], c='black')
    x.append(data[0])
    y.append(data[1])
    
plt.plot(x, y)
    
#plt.title("correlation")
#plt.xlabel("Average Temperature of SAITAMA")
#plt.ylabel("Average Temperature of IWATE")

plt.show()
