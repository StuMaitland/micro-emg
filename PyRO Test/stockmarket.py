# stockmarket.py
from __future__ import print_function
import random
import threading
import time
import Pyro4
import os
import string


class StockMarket(object):
    def __init__(self, marketname, symbols):
        self.name = marketname
        self.symbolmeans = {}
        for symbol in symbols:
            self.symbolmeans[symbol] = random.uniform(20, 200)
        self.aggregators = []

    def generate(self):
        quotes = {}
        start = time.time()
        for symbol, mean in self.symbolmeans.items():
            #quotes[symbol] = round(random.normalvariate(mean, 20),0)

            quotes[symbol] = '1'*5 #0 #Generate a random 1.25Mb bytearray
        end = time.time()
        elapsed = end - start
        print("new quotes generated for {0} in {1} seconds".format(self.name, elapsed))
        for aggregator in self.aggregators:
            aggregator.quotes(self.name, quotes)

    @Pyro4.expose
    def listener(self,aggregator):
        print("market {0} adding new aggregator".format(self.name))
        self.aggregators.append(aggregator)

    @Pyro4.expose
    def symbols(self):
        return list(self.symbolmeans.keys())

    def run(self):
        def generate_symbols():
            while True:
                starttime = time.time()
                self.generate()
                time.sleep(1.0 - ((time.time() - starttime) % 1.0)) # Run every second (minus the time for execution)
        thread = threading.Thread(target=generate_symbols)
        thread.setDaemon(True)
        thread.start()


def main():
    stocknames=[]
    for i in range(64):
        stocknames.append( ''.join(random.choice(string.ascii_uppercase + string.digits) for _ in range(4)) )

    nasdaq = StockMarket("NASDAQ", stocknames)

    with Pyro4.Daemon() as daemon:
        nasdaq_uri = daemon.register(nasdaq)
        with Pyro4.locateNS() as ns:

            ns.register("example.stockmarket.nasdaq", nasdaq_uri)
        nasdaq.run()
        print("Stockmarkets running.")
        daemon.requestLoop()

if __name__ == "__main__":
    main()