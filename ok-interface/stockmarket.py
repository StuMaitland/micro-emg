# stockmarket.py
from __future__ import print_function
import random
import threading
import time
import Pyro4
import os
import string
from sp import DESTester as sp
import numpy as np
import matplotlib.pyplot as plt


class StockMarket(object):
    def __init__(self, marketname, symbols):
        self.name = marketname
        self.symbolmeans = {}
        for symbol in symbols:
            self.symbolmeans[symbol] = random.uniform(20, 200)
        self.aggregators = []

    def generate(self, sampleLength, numDataStreams,des):
        quotes = {}
        buffer = des.collectDataFromPipeOut(sampleLength, numDataStreams)
        des.resetBuffer()

        parsed=des.readDataBlock(buffer,sampleLength,numDataStreams)
        #print(parsed)
        volts=des.bytesToVolts(parsed.amplifier)
        # Need to convert to list as Pyro doesn't like Numpy arrays for serialisation
        listed=volts.tolist()

        #Get the 64 channels, with 32 in each stream
        quotes.update(dict(zip(xrange(0,32),listed[0][0:32])))
        quotes.update(dict(zip(xrange(32, 64), listed[1][0:32])))

        #print("new quotes generated for {0} in {1} seconds: {2}".format(self.name, elapsed,buffer[0:50]))
        for aggregator in self.aggregators:
            try:
                aggregator.quotes(self.name, quotes)
            except:
                pass

    @Pyro4.expose
    def listener(self, aggregator):
        print("market {0} adding new aggregator".format(self.name))
        self.aggregators.append(aggregator)

    @Pyro4.expose
    def symbols(self):
        return list(self.symbolmeans.keys())

    def run(self, sampleLength, numDataStreams,des):
        def generate_symbols():
            #startTime=time.clock()
            #timer=0
            while 1:
                starttime = time.time()
                self.generate(sampleLength, numDataStreams,des)
                # Run every second (minus the time for execution)
                #time.sleep(0.001 - ((time.time() - starttime) % 1.0))

        thread = threading.Thread(target=generate_symbols)
        thread.setDaemon(True)
        thread.start()


def main():
    # User-modified variables
    sampleLength = 100
    numDataStreams = 0

    # Main code
    print("------ DES Encrypt/Decrypt Tester in Python ------")
    des = sp()

    completed = des.initializeDevice()
    if completed == 'False':
        exit

    des.enableDataStream(0)
    des.setDataSource(0, 0)
    numDataStreams += 1
    des.enableDataStream(1)
    des.setDataSource(1, 1)
    numDataStreams += 1

    des.setContinuousRunMode(False)
    # Set sample frequency to 20KS/s
    des.setSampleFrequency(28, 25)
    des.setCableDelay("PortA", 0)
    des.setCableDelay("PortB", 0)

    queue = [None] * 1000
    # Note than the number of channels is per stream
    numChannels = 32

    dataSize = des.dataBlockSize(numDataStreams) * sampleLength

    stocknames = []
    for i in range(numChannels*numDataStreams):
        stocknames.append(i)

    nasdaq = StockMarket("NASDAQ", stocknames)

    with Pyro4.Daemon() as daemon:
        nasdaq_uri = daemon.register(nasdaq)
        with Pyro4.locateNS() as ns:
            ns.register("example.stockmarket.nasdaq", nasdaq_uri)
        nasdaq.run(sampleLength, numDataStreams,des)
        print("Stockmarkets running.")
        daemon.requestLoop()


if __name__ == "__main__":
    main()
