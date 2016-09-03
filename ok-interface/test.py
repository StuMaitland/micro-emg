import ok
import sys
import string
import time
import numpy
import struct
import matplotlib.pyplot as plt
import collections


class DESTester:
    def __init__(self):
        return

    def initializeDevice(self):
        # Open the first device we find.
        self.xem = ok.okCFrontPanel()
        if (self.xem.NoError != self.xem.OpenBySerial("")):
            raise RuntimeError("A device could not be opened.  Is one connected, or in use by another process?")
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
        self.resetBoard()
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

    def lightPulse(self, i=1):
        # Cascades lights on OK machine
        while 1:
            if i > 255:
                i = 1
            self.xem.SetWireInValue(0x11, i)
            self.xem.UpdateWireIns()
            time.sleep(0.1)
            i *= 2

    def startRecording(self, RecordingState):
        # Switches the board to a listening state
        if RecordingState:
            self.xem.ActivateTriggerIn(0x41, 0)
        else:
            self.xem.ActivateTriggerIn(0x41, 1)

    def isRunning(self):
        # Checks whether the board is currently evaluating a data block
        return self.xem.GetWireOutValue(0x22)

    def setContinuousRunMode(self, continuousMode):
        # Set the continuous recording mode i.e. run whether the computer is ready for data or not
        if continuousMode:
            self.xem.SetWireInValue(0x00, 0x02, 0x02)
        else:
            self.xem.SetWireInValue(0x00, 0x00, 0x02)
        self.xem.UpdateWireIns()

    def setMaxTimeStep(self, maxTimeStep):
        # Set the amount of time the board should listen for data over
        # timesteps are in intervals of 10, so lets multiply it by that amount
        maxTimeStep*=10
        # I have no idea why you need to do this least/most sig bit but its in the source so
        maxTimeStepLsb = maxTimeStep & 0x0000ffff
        maxTimeStepMsb = maxTimeStep & 0xffff0000


        # These are just the magic addresses for switching on the timers
        self.xem.SetWireInValue(0x01, maxTimeStepLsb)
        self.xem.SetWireInValue(0x02, maxTimeStepMsb >> 16)
        self.xem.UpdateWireIns()

    def collectDataFromPipeOut(self, length, sampleLength):
        # Starts the timer, waits until an amount of data is available on the USB buffer, then collects it
        self.setMaxTimeStep(sampleLength)
        buffer = bytearray("\x00" * length)
        self.startRecording(1)
        while self.isRunning():
            pass
        # Read from the Data pipe out- note that buffer passed is overwritten with the new data source in doing this.
        self.xem.ReadFromPipeOut(0xa0, buffer)
        return buffer

    def enableDataStream(self, streamIndex, enable):
        # Enables or disables streams
        return None

    def dataBlockSize(self,numDataStreams):
        """
        Calculates the expected size of a single sample of data
        :param numDataStreams: number of data streams
        :return: int
        """
        return(8+4+(numDataStreams*3*2)+(numDataStreams*32*2)+(numDataStreams*2)+(8*2)+2+2)

    def setSampleFrequency(self, multiply, divide):
        # Assuming a 100 MHz reference clock is provided to the FPGA, the programmable FPGA clock frequency
        # is given by:
        #
        #       FPGA internal clock frequency = 100 MHz * (M/D) / 2
        #
        # M and D are "multiply" and "divide" integers used in the FPGA's digital clock manager (DCM) phase-
        # locked loop (PLL) frequency synthesizer, and are subject to the following restrictions:
        #
        #                M must have a value in the range of 2 - 256
        #                D must have a value in the range of 1 - 256
        #                M/D must fall in the range of 0.05 - 3.33
        #
        # (See pages 85-86 of Xilinx document UG382 "Spartan-6 FPGA Clocking Resources" for more details.)
        #
        # This variable-frequency clock drives the state machine that controls all SPI communication
        # with the RHD2000 chips.  A complete SPI cycle (consisting of one CS pulse and 16 SCLK pulses)
        # takes 80 clock cycles.  The SCLK period is 4 clock cycles; the CS pulse is high for 14 clock
        # cycles between commands.
        #
        # Rhythm samples all 32 channels and then executes 3 "auxiliary" commands that can be used to read
        # and write from other registers on the chip, or to sample from the temperature sensor or auxiliary ADC
        # inputs, for example.  Therefore, a complete cycle that samples from each amplifier channel takes
        # 80 * (32 + 3) = 80 * 35 = 2800 clock cycles.
        #
        # So the per-channel sampling rate of each amplifier is 2800 times slower than the clock frequency.
        #
        # Based on these design choices, we can use the following values of M and D to generate the following
        # useful amplifier sampling rates for electrophsyiological applications:
        #
        #   M    D     clkout frequency    per-channel sample rate     per-channel sample period
        #  ---  ---    ----------------    -----------------------     -------------------------
        #    7  125          2.80 MHz               1.00 kS/s                 1000.0 usec = 1.0 msec
        #    7  100          3.50 MHz               1.25 kS/s                  800.0 usec
        #   21  250          4.20 MHz               1.50 kS/s                  666.7 usec
        #   14  125          5.60 MHz               2.00 kS/s                  500.0 usec
        #   35  250          7.00 MHz               2.50 kS/s                  400.0 usec
        #   21  125          8.40 MHz               3.00 kS/s                  333.3 usec
        #   14   75          9.33 MHz               3.33 kS/s                  300.0 usec
        #   28  125         11.20 MHz               4.00 kS/s                  250.0 usec
        #    7   25         14.00 MHz               5.00 kS/s                  200.0 usec
        #    7   20         17.50 MHz               6.25 kS/s                  160.0 usec
        #  112  250         22.40 MHz               8.00 kS/s                  125.0 usec
        #   14   25         28.00 MHz              10.00 kS/s                  100.0 usec
        #    7   10         35.00 MHz              12.50 kS/s                   80.0 usec
        #   21   25         42.00 MHz              15.00 kS/s                   66.7 usec
        #   28   25         56.00 MHz              20.00 kS/s                   50.0 usec
        #   35   25         70.00 MHz              25.00 kS/s                   40.0 usec
        #   42   25         84.00 MHz              30.00 kS/s                   33.3 usec
        #
        # To set a new clock frequency, assert new values for M and D (e.g., using okWireIn modules) and
        # pulse DCM_prog_trigger high (e.g., using an okTriggerIn module).  If this module is reset, it
        # reverts to a per-channel sampling rate of 30.0 kS/s.

        self.xem.SetWireInValue(0x03,(256*multiply + divide))
        self.xem.UpdateWireIns()
        self.xem.ActivateTriggerIn(0x40,0)

    def resetBoard(self):
        self.xem.SetWireInValue(0x00,0x01,0x01)
        self.xem.UpdateWireIns()
        self.xem.SetWireInValue(0x00,0x00,0x01)
        self.xem.UpdateWireIns()

    def readDataBlock(self, buffer, samplesPerDataBlock, numDataStreams):
        """
        :param buffer: byte array object, the size of what you want to get
        :param samplesPerDataBlock: number of samples expected
        :param numDataStreams: int number of data streams
        :return:
        """

        # Takes raw data from the USB buffer on the board and converts it into something usable.

        # First we initialise some local variables
        timeStamp = []
        numChannels=32
        auxiliaryData = numpy.zeros((100, 100, 100,))
        amplifierData = numpy.zeros((numDataStreams, numChannels, samplesPerDataBlock))
        boardAdcData = numpy.zeros((100, 100))
        ttlIn = []
        ttlOut = []
        index = 0

        #   The data is arranged in the following format on the USB buffer:
        #       8 byte header
        #       4 byte timestamp
        #       n byte auxiliary data
        #       n byte amplifier data (the one we want)
        #       n byte filler word
        #       128 byte ADC5662 data
        #       4 byte ttlIn + ttOut data

        for sample in range(0, samplesPerDataBlock):
            print("Sample: {}. Index: {}".format(sample,index))

            # OK, there's a lot going on on this line.
            # Unpack the byte array, coerced into a bytes object
            # < means little endian, Q indicates unsigned long long
            # By default returns a tuple, so just get the first bit.
            # Reference: https://docs.python.org/3/library/struct.html
            header = struct.unpack("<Q", bytes(buffer[index:index + 8]))[0]
            # Check the first 8 bytes to match the RHD2000 magic header word
            if header != 0xc691199927021942:
                raise SyntaxError("Error in readDataBlock: Incorrect header: {0}".format(header))
            index += 8

            # Read the timestamp
            # This time it's L- unsigned long
            timeStamp.append(struct.unpack("<L", buffer[index:index + 4])[0])
            print("TimeStamp: {0}".format(timeStamp[sample]))
            index += 4

            # Read auxiliary input
            for channel in range(0, 3):
                for stream in range(0, numDataStreams):
                    # The rest are H, unsigned short
                    auxiliaryData[stream][channel][sample] = struct.unpack("<H", buffer[index:index + 2])[0]
                    index += 2
                    # Each sample is represented by two bytes

            # Read amplifier channels- this is the one we really want
            for channel in range(0, 32):
                for stream in range(0, numDataStreams):
                    amplifierData[stream][channel][sample] = struct.unpack("<H", buffer[index:index + 2])[0]
                    index += 2
            # Skip 36th filler word in each data stream
            index += 2 * numDataStreams

            # Read from AD5662 ADCs
            for i in range(0, 8):
                boardAdcData[i][sample] = struct.unpack("<H", buffer[index:index + 2])[0]
                index += 2

            # Read TTL input and output values
            ttlIn.append(struct.unpack("<H", buffer[index:index + 2])[0])
            index += 2
            ttlOut.append(struct.unpack("<H", buffer[index:index + 2])[0])
            index += 2

        return amplifierData

# User-modified variables
samplesPerDataBlock = 50
numDataStreams = 4

# Main code
print("------ DES Encrypt/Decrypt Tester in Python ------")
des = DESTester()

completed = des.initializeDevice()
if completed == 'False':
    exit

# des.lightPulse()

des.setContinuousRunMode(False)



#Set sample frequency to 20KS/s
#des.setSampleFrequency(42,45)
dataToPlot= [None]*100

plt.axis([0,1000,8000,20000])
plt.ion()

print("")

while True:
    try:
        #Get the bytes from the buffer
        buffer = des.collectDataFromPipeOut(des.dataBlockSize(numDataStreams)*samplesPerDataBlock, samplesPerDataBlock)
        # Process into a data block
        amplifierData= des.readDataBlock(buffer, samplesPerDataBlock, numDataStreams)

        dataToPlot=dataToPlot[5:]
        dataToPlot.extend(amplifierData[0][1][:])
        #Plot the data
        #dataToPlot.append(amplifierData[0][1][:])

        plt.plot(dataToPlot)
        plt.waitforbuttonpress()
    except KeyboardInterrupt:
        break

print("ready for input")
print("")
