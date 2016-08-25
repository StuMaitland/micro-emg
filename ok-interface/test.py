import ok
import sys
import string
import time
import numpy


class DESTester:
    def __init__(self):
        return

    def initializeDevice(self):
        # Open the first device we find.
        self.xem = ok.okCFrontPanel()
        if (self.xem.NoError != self.xem.OpenBySerial("")):
            print("A device could not be opened.  Is one connected?")
            return (False)

        # Get some general information about the device.
        self.devInfo = ok.okTDeviceInfo()
        if (self.xem.NoError != self.xem.GetDeviceInfo(self.devInfo)):
            print("Unable to retrieve device information.")
            return (False)
        print("         Product: " + self.devInfo.productName)
        print("Firmware version: %d.%d" % (self.devInfo.deviceMajorVersion, self.devInfo.deviceMinorVersion))
        print("   Serial Number: %s" % self.devInfo.serialNumber)
        print("       Device ID: %s" % self.devInfo.deviceID)

        self.xem.LoadDefaultPLLConfiguration()

        # Download the configuration file.
        if (self.xem.NoError != self.xem.ConfigureFPGA("main.bit")):
            print("FPGA configuration failed.")
            return (False)

        # Check for FrontPanel support in the FPGA configuration.
        if (False == self.xem.IsFrontPanelEnabled()):
            print("FrontPanel support is not available.")
            return (False)

        print("FrontPanel support is available.")
        return (True)

    def setKey(self, key):
        for i in range(4):
            self.xem.SetWireInValue((0x0b - i), key[i], 0xffff)
        self.xem.UpdateWireIns()

    def resetDES(self):
        self.xem.SetWireInValue(0x10, 0xff, 0x01)
        self.xem.UpdateWireIns()
        self.xem.SetWireInValue(0x10, 0x00, 0x01)
        self.xem.UpdateWireIns()

    def encryptDecrypt(self, infile, outfile):
        fileIn = open(infile, "rb")
        fileOut = open(outfile, "wb")

        # Reset the RAM address pointer.
        self.xem.ActivateTriggerIn(0x41, 0)

        while fileIn:
            buf = bytearray(fileIn.read(2048))

            got = len(buf)
            if (got == 0):
                break

            if (got < 2048):
                buf += b"\x00" * (2048 - got)

            # Write a block of data.
            self.xem.ActivateTriggerIn(0x41, 0)
            self.xem.WriteToPipeIn(0x80, buf)

            # Perform DES on the block.
            self.xem.ActivateTriggerIn(0x40, 0)

            # Wait for the TriggerOut indicating DONE.
            for i in range(100):
                self.xem.UpdateTriggerOuts()
                if (self.xem.IsTriggered(0x60, 1)):
                    break

            self.xem.ReadFromPipeOut(0xa0, buf)
            fileOut.write(buf)

        fileIn.close()
        fileOut.close()

    def encrypt(self, infile, outfile):
        print("Encrypting %s ----> %s" % (infile, outfile))
        # Set the encrypt Wire In.
        self.xem.SetWireInValue(0x0010, 0x0000, 0x0010)
        self.xem.UpdateWireIns()
        self.encryptDecrypt(infile, outfile)

    def decrypt(self, infile, outfile):
        print("Decrypting %s ---> %s" % (infile, outfile))
        # Set the decrypt Wire In.
        self.xem.SetWireInValue(0x0010, 0x00ff, 0x0010)
        self.xem.UpdateWireIns()
        self.encryptDecrypt(infile, outfile)

    def lightPulse(self,i=1):
        #Cascades lights on OK machine
        while 1:
            if i > 255:
                i = 1
            self.xem.SetWireInValue(0x11, i)
            self.xem.UpdateWireIns()
            time.sleep(0.1)
            i *= 2

    def startRecording(self,RecordingState):
        #Switches the board to a listening state
        if RecordingState:
            self.xem.ActivateTriggerIn(0x41, 0)
        else:
            self.xem.ActivateTriggerIn(0x41, 1)

    def isRunning(self):
        #Checks whether the board is currently evaluating a data block
        return self.xem.GetWireOutValue(0x22)

    def setContinuousRunMode(self,continuousMode):
        #Set the continuous recording mode i.e. run whether the computer is ready for data or not
        if continuousMode:
            self.xem.SetWireInValue(0x00,0x02,0x02)
        else:
            self.xem.SetWireInValue(0x00,0x00,0x02)
        self.xem.UpdateWireIns()

    def setMaxTimeStep(self,maxTimeStep):
        #Set the amount of time the board should listen for data over

        # I have no idea why you need to do this least/most sig bit but its in the source so
        maxTimeStepLsb = maxTimeStep & 0x0000ffff
        maxTimeStepMsb = maxTimeStep & 0xffff0000

        # These are just the magic addresses for switching on the timers
        self.xem.SetWireInValue(0x01,maxTimeStepLsb)
        self.xem.SetWireInValue(0x02,maxTimeStepMsb >> 16)
        self.xem.UpdateWireIns()


    def collectDataFromPipeOut(self, length, sampleLength):
        #Starts the timer, waits until an amount of data is available on the USB buffer, then collects it
        self.setMaxTimeStep(sampleLength)
        buffer = bytearray("\x00" * length)
        self.startRecording(1)
        while self.isRunning():
            pass
        # Read from the Data pipe out- note that buffer passed is overwritten with the new data source in doing this.
        self.xem.ReadFromPipeOut(0xa0,buffer)
        return buffer

    def convertUSBWord(self,integers):
        #Takes bytes and concatenates them into a new word
        multiple=len(integers)
        concatItems=0
        #Reverse the array for little endian format
        for item in integers[::-1]:
            concatItems=concatItems + (item << (multiple * 8))
            multiple-= 1

        #Trim the final byte caused by initialising with 0
        concatItems=bin(concatItems)[:-8]
        concatItems=int(str(concatItems),2)

        return concatItems


    def readDataBlock(buffer, samplesPerDataBlock, numDataStreams):

        #First we initialise some variables
        timeStamp = []
        auxiliaryData = numpy.zeros((1, 1, 1,))
        amplifierData = numpy.zeros((1, 1, 1,))
        boardAdcData = numpy.zeros((1, 1))
        ttlIn = []
        ttlOut = []
        index = 0

        # The data is arranged in the following format on the board:
            ## 8 byte header
            ## 4 byte timestamp
            ## n byte auxiliary data
            ## n byte amplifier data (the one we want)
            ## n byte filler word
            ## 128 byte ADC5662 data
            ## 4 byte ttlIn + ttOut data

        for sample in range(0, samplesPerDataBlock):
            # Check the first 8 bytes to match the RHD2000 magic header word
            if des.convertUSBWord(buffer[index:index + 8]) != 0xc691199927021942:
                raise SyntaxError("Error in readDataBlock: Incorrect header")
            index += 8

            # Read the timestamp
            timeStamp.append(des.convertUSBWord(buffer[index:index + 4]))
            index += 4

            # Read auxiliary input
            for channel in range(0, 3):
                for stream in range(0, numDataStreams):
                    auxiliaryData[stream][channel][sample] == des.convertUSBWord(buffer[index:index + 1])
                    index += 2
                    # Each sample is represented by two bytes

            # Read amplifier channels- this is the one we really want
            for channel in range(0, 32):
                for stream in range(0, numDataStreams):
                    amplifierData[stream][channel][sample] = des.convertUSBWord(buffer[index:index + 1])
                    index+=2
            # Skip 36th filler word in each data stream
            index += 2 * numDataStreams

            # Read from AD5662 ADCs
            for i in range(0, 8):
                boardAdcData[i][t] = des.convertUSBWord(buffer[index:index + 1])
                index += 2

            # Read TTL input and output values
            ttlIn[sample] = des.convertUSBWord(buffer[index:index + 1])
            index += 2
            ttlOut[sample] = des.convertUSBWord(buffer[index:index + 1])
            index += 2




#User-modified variables
samplesPerDataBlock=60
numDataStreams= 5

# Main code
print("------ DES Encrypt/Decrypt Tester in Python ------")
des = DESTester()

completed = des.initializeDevice()
if completed == 'False':
    exit

#des.lightPulse()


des.setContinuousRunMode(False)

buffer=des.collectDataFromPipeOut(2048, samplesPerDataBlock)
des.readDataBlock(buffer,samplesPerDataBlock,numDataStreams)


print("ready for input")
print("")
