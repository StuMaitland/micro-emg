# viewer.py
from __future__ import print_function
import sys
import Pyro4
import time
import sp
from matplotlib import pyplot as plt

if sys.version_info < (3,0):
    input = raw_input

#Pyro4.config.HOST = "mHealthfulsMini"
#host = "mHealthfulsMini"

@Pyro4.expose
class Viewer(object):

    def __init__(self):
        self.queue=[None]*1000
        plt.figure()
        self.ln, = plt.plot([])
        plt.ion()
        plt.show()
        plt.ylim([8000, 8200])
        plt.xlim([0, len(self.queue)])

    def quote(self, market, symbol, value):
        #signal = sp.DESTester.readDataBlock(value, 100, 2)
        #print(signal.amplifier[0][16])
        print("quote received: {}".format(value))

        #with open('{0}.txt'.format(symbol), 'a') as myfile:
        #    myfile.write("{0}: {1}.{2}: {3}".format(time.time(), market, symbol, value))

        self.queue.append(value)
        #self.queue[:-100] = self.queue[100:]

    def realTimePlot(self):
        self.ln.set_xdata(range(len(self.queue)))
        self.ln.set_ydata(self.queue)
        plt.draw()
        plt.show()



def main():
    viewer = Viewer()
    with Pyro4.Daemon() as daemon:
        daemon.register(viewer)
        aggregator = Pyro4.Proxy("PYRONAME:example.stockquote.aggregator")
        print("Available stock symbols:", aggregator.available_symbols())
        symbols = input("Enter symbols you want to view (comma separated):")
        symbols = [symbol.strip() for symbol in symbols.split(",")]
        symbols = [int(symbol) for symbol in symbols]
        aggregator.view(viewer, symbols)
        print("Viewer listening on symbols", symbols)
        daemon.requestLoop()


if __name__ == "__main__":
    main()