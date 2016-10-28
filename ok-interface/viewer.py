# viewer.py
from __future__ import print_function
import sys
import Pyro4
from matplotlib import pyplot as plt
import numpy as np
import plotly.plotly as py
import plotly.tools as tls
import plotly.graph_objs as go
import datetime


if sys.version_info < (3,0):
    input = raw_input






@Pyro4.expose
class Viewer(object):

    def __init__(self):
        self.queue=[None]*1000
        self.timequeue=[None]*1000
        stream_ids= tls.get_credentials_file()['stream_ids']
        stream_1 = dict(token=stream_ids[0], maxpoints=500)
        trace1 = go.Scatter(
            x=[],
            y=[],
            mode='lines+markers',
            stream=stream_1
        )

        data = go.Data([trace1])

        layout = go.Layout(title='Time Series')

        fig = go.Figure(data=data, layout=layout)

        py.plot(fig, filename='python-streaming')

        self.s = py.Stream(stream_ids[0])

        self.s.open()

    def quote(self, market, symbol, value):
        #signal = sp.DESTester.readDataBlock(value, 100, 2)
        #print(signal.amplifier[0][16])

        #with open('{0}.txt'.format(symbol), 'a') as myfile:
        #    myfile.write("{0}: {1}.{2}: {3}".format(time.time(), market, symbol, value))

        timestamps = [datetime.datetime.now() + datetime.timedelta(seconds=sample / 20000) for sample in range(0, len(value))]
        #n = 0
        self.queue.extend(value)
        self.timequeue.extend(timestamps)

        self.queue[:-100] = self.queue[100:]
        self.timequeue[:-100] = self.timequeue[100:]
        self.s.write(dict(x=self.timequeue, y=self.queue))
        #for item in value:
        #    self.s.write(dict(x=datetime.datetime.now(), y=item))
        #    n+=1



        # Create the timestamps for each sample



    def realTimePlot(self):
        #self.ln.set_xdata(range(0,1000))
        #self.ln.set_ydata(self.queue[-1000:])
        plt.scatter(xrange(0,1000),self.queue[-1000:])
        #plt.draw()
        plt.pause(0.05)
        print("drawn")



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