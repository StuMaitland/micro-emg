# viewer.py
from __future__ import print_function
import sys
import Pyro4
from matplotlib import pyplot as plt
import numpy as np
import datetime
from bokeh.plotting import figure, show, output_server, show, Document, curdoc
from bokeh.client import push_session



if sys.version_info < (3,0):
    input = raw_input


@Pyro4.expose
class Viewer(object):

    def __init__(self):
        self.queue=[None]*1000
        self.timequeue=[None]*1000

        self.p = figure(plot_height=400,plot_width=400)
        r=self.p.line([],[], color="firebrick",line_width=2)
        self.ds=r.data_source
        session = push_session(curdoc())
        session.show()
        curdoc().add_root(self.p)



    def quote(self, market, symbol, value, timestamps):
        #signal = sp.DESTester.readDataBlock(value, 100, 2)

        #with open('{0}.txt'.format(symbol), 'a') as myfile:
        #    myfile.write("{0}: {1}.{2}: {3}".format(time.time(), market, symbol, value))

        self.queue.extend(value)

        self.timequeue.extend(timestamps)

        self.queue[:-100] = self.queue[100:]
        self.timequeue[:-100] = self.timequeue[100:]
        #self.ds.data.update(x=self.timequeue,y=self.queue)
        self.ds.data["x"]=self.timequeue
        self.ds.data["y"]=self.queue
        self.ds.trigger('data',self.ds.data,self.ds.data)

        #for item in value:
        #    self.s.write(dict(x=datetime.datetime.now(), y=item))
        #    n+=1



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