import unittest
from SignalProcessor import DESTester

class InterfaceTests(unittest.TestCase):

    def setUp(self):
        self.des=DESTester()

    def testInitialise(self):
        self.assertTrue(self.des.initializeDevice())

def main():
    unittest.main()

if __name__ == '__main__':
    main()