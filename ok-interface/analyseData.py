import pickle
import matplotlib.pyplot as plt
import numpy as np

fig,ax=plt.subplots()
plt.ylim([0,40000])

line,=ax.plot(np.zeros(1000))
queue=[None]*1000
sampleLength = 200

while 1:
    archive = pickle.load(open('test.lol', 'rb'))
    print()
    voltData = archive[1][5][:-sampleLength]

    queue[:-sampleLength] = queue[sampleLength:]
    queue[-sampleLength:] = voltData
    line.set_ydata(queue)
    fig.canvas.draw()



for graph in range(0,32):
    ax=plt.subplot(8,4,graph)
    ax.plot(archive[graph])

plt.show()

print("")