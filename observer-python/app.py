from observer import Observer
#from observable import Observable
import sys

import Pyro4
import Pyro4.util

sys.excepthook = Pyro4.util.excepthook

# Connect to the observable (host) using this name server
#observable = Observable()
observable = Pyro4.Proxy("PYRO:example.observable@localhost:55646")


# Remote procedure call to Observable
observer1 = Observer(observable)
observer2 = Observer(observable)

observable.notify_observers('test')