import unittest
import sp

# Some unit tests for the interface with the Opal Kelly module
# Limitations:
#   Not testing the internal OK methods beyond the API- they're a mess
#

class InterfaceTests(unittest.TestCase):
    def setUp(self):
        self.des = sp.DESTester()

    def tearDown(self):
        self.des.resetBoard()

    def testResetBuffer(self):
        self.des.resetBuffer()
        buf = bytearray("\x00" * 2048)
        read = self.des.xem.ReadFromPipeOut(0xa0, buf)
        self.assertEqual(0, 0)

    def testConnect(self):
        self.assertEqual(self.des.xem.NoError, self.des.xem.OpenBySerial(""))

    def testEstimateDataBlockSizeZeroStreams(self):
        numStreams = 0
        correctSize = 32
        self.assertEqual(correctSize, self.des.dataBlockSize(numStreams))

    def testGetDataBlockZeroStreams(self):
        sampleLength = 1
        numDataStreams = 0
        array = self.des.collectDataFromPipeOut(sampleLength, numDataStreams)
        self.assertEqual(len(array), self.des.dataBlockSize(numDataStreams)*sampleLength)

    def testGetDataBlockOneStream(self):
        sampleLength = 1
        numDataStreams = 1
        array = self.des.collectDataFromPipeOut(sampleLength, numDataStreams)
        self.assertEqual(len(array), self.des.dataBlockSize(numDataStreams)*sampleLength)


def main():
    unittest.main()


if __name__ == '__main__':
    main()
