import matplotlib.pyplot as plt
import SignalProcessor
import numpy as np
import pickle
import time
import matplotlib.pyplot as plt


# User-modified variables
sampleLength = 100
numDataStreams = 0

# Main code
print("------ DES Encrypt/Decrypt Tester in Python ------")
des = SignalProcessor.DESTester()

completed = des.initializeDevice()
if completed == 'False':
    exit

des.enableDataStream(0)
numDataStreams+=1
des.setDataSource(0,0)
des.setContinuousRunMode(False)

queue=[None]*1000
dataSize = des.dataBlockSize(numDataStreams) * sampleLength
archive=[[None] for i in range(32)]

bufferStore=[]


#plt.figure()
#ln,=plt.plot([])
#plt.ion()
#plt.show()
#plt.ylim([7500,9000])
#plt.xlim([0,len(queue)])

for t in xrange(0,50):
    #Collect data from the USB Buffer- supply size in bytes and duration of sample in time interval
    buffer = des.collectDataFromPipeOut(sampleLength,numDataStreams)
    bufferStore.append(buffer)


for item in bufferStore:
    signal = des.readDataBlock(buffer, sampleLength, numDataStreams)
    for i in xrange(0,32):
       archive[i].extend(des.bytesToVolts(signal.amplifier[0][i]))

pickle.dump(archive,open("test.pickle","wb"))


fig,ax=plt.subplots()
plt.ylim([0,40000])

line,=ax.plot(np.zeros(1000))

for graph in range(0,32):
    with open("{}.csv".format(graph),'w') as f:
        f.write(str(archive[graph][1:]))
    ax=plt.subplot(8,4,graph)
    ax.plot(archive[graph])
    ax.set_title(graph)

plt.show()

print("ready for input")
print("")

