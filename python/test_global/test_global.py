#!/usr/bin/env python

import threading
import time
import tornado.ioloop

connections = {}

def add_c(id, info={}):
    global connections
    connections[id] = info

def get_connections():
    global connections
    return connections
    

class vThread1(threading.Thread):
    def __init__(self):
        threading.Thread.__init__(self)

    def run(self):
        t2 = vThread3()
        t2.setDaemon(True)
        t2.start()

        for i in range(10):
            c = {"c":i}
            add_c(i, c)
            time.sleep(2)
            print "1: ",get_connections()



class vThread3(threading.Thread):
    def __init__(self):
        threading.Thread.__init__(self)

    def run(self):
        while True:
            print "3: ",get_connections()
            time.sleep(2)


class vThread2(threading.Thread):
    def __init__(self):
        threading.Thread.__init__(self)

    def run(self):
        while True:
            print "2: ",get_connections()
            time.sleep(2)



def check_connections():
    print "check connection"
    t1 = vThread1()
    t1.setDaemon(True)
    t1.start()

    #t1.join()


if __name__ == "__main__":
    t2 = vThread2()
    t2.setDaemon(True)
    t2.start()

    tornado.ioloop.PeriodicCallback(check_connections, 20 * 1000).start()
    tornado.ioloop.IOLoop.instance().start()
    
    t2.join()
