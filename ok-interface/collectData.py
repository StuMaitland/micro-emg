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
des.setDataSource(0,0)
numDataStreams+=1
des.enableDataStream(1)
des.setDataSource(1,1)
numDataStreams+=1


des.setContinuousRunMode(False)
#Set sample frequency to 20KS/s
des.setSampleFrequency(28,25)
des.setCableDelay("PortA",0)
des.setCableDelay("PortB",0)

queue=[None]*1000
#Note than the number of channels is per stream
numChannels=32

dataSize = des.dataBlockSize(numDataStreams) * sampleLength
archive=[[None] for i in range(numChannels)]

bufferStore=[]

for t in xrange(0,50):
    #Collect data from the USB Buffer- supply size in bytes and duration of sample in time interval
    buffer = des.collectDataFromPipeOut(sampleLength,numDataStreams)
    bufferStore.append(buffer)
    des.resetBuffer()


for item in bufferStore:
    signal = des.readDataBlock(buffer, sampleLength, numDataStreams)
    for i in xrange(0,numChannels):
        archive[i].extend(signal.amplifier[0][i])
        #archive[i].extend(des.bytesToVolts(signal.amplifier[0][i]))

#pickle.dump(archive,open("test.pickle","wb"))


fig,ax=plt.subplots()
plt.ylim([0,40000])
plt.ylim([0,500])


print("")

ax.plot(archive[1])

line,=ax.plot(np.zeros(1000))
for graph in range(0,32):
    with open("{}.csv".format(graph),'w') as f:
        f.write(str(archive[graph][1:])[1:-1])
    ax=plt.subplot(8,4,graph)
    ax.plot(archive[graph])
    ax.set_title(graph)

plt.show()

print("ready for input")
print("")

