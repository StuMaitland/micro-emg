# Authors: Stuart Maitland
# This is the basic server for the observer design pattern
# Observers can be registered onto the list, and notified of new data
# Modified from Python Remote Calls (Pyro4) http://pythonhosted.org/Pyro4/tutorials.html

import Pyro4
import observer

@Pyro4.expose
class Observable:
    def __init__(self):
        self.__observers = []

    def register_observer(self, observer):
        # Append a new observer onto the list
        self.__observers.append(observer)

    def notify_observers(self, *args):
        # Notify all listed observers of changes
        for observer in self.__observers:
            observer.notify(self, *args)


# Start Pyro daemon server
def main():
    Pyro4.Daemon.serveSimple(
            {
                Observable: "example.observable"
            },
            ns = False)

if __name__=="__main__":
    main()








