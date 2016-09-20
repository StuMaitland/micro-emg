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
des.setCableDelay("PortA",5)

numChannels=32*numDataStreams
queue = [None] * 1000
dataSize = des.dataBlockSize(numDataStreams) * sampleLength
archive = [[None] for i in range(numChannels)]

bufferStore = []

plt.figure()
ln,=plt.plot([])
plt.ion()
plt.show()
plt.ylim([8000,8200])
plt.xlim([0,len(queue)])

for t in xrange(0, 5000):
    # Collect data from the USB Buffer- supply size in bytes and duration of sample in time interval
    buffer = des.collectDataFromPipeOut(sampleLength, numDataStreams)

    signal = des.readDataBlock(buffer, sampleLength, numDataStreams)

    voltData=des.bytesToVolts(signal.amplifier[0][2][:])
    des.resetBuffer()
    queue[:-sampleLength] = queue[sampleLength:]
    queue[-sampleLength:] = voltData

    plt.pause(0.001)
    ln.set_xdata(range(len(queue)))
    ln.set_ydata(queue)
    plt.draw()
    plt.show()



print("ready for input")
print("")
