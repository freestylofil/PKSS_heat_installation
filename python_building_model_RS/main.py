from ntplib import NTPClient, NTPException
from server import ServerThread
import matplotlib.pyplot as plt
from time import sleep
import logging
from threading import Lock


logging.basicConfig(level=logging.DEBUG)

# Model parameters
mb = 20000
cb = 1000
kext = 15000
mh = 3000
ch = 2700
kh = 12000
dzeta = 1000
cw = 4200

# Mocked data from other nodes
to = 5
tzco = 80

# Initial values for data calculated by the model
tr = 20
tpco = 75

# No regulation yet, constant water flow
fcob = 40

# Data dict we'll be modifying and sending
data = {'Tpcob': tpco, 'Fcob': fcob, 'Tr': tr}
lock = Lock()

host = 'localhost'
port = 50009
server = ServerThread(data, host, port)
server.start()

# Used for local logging and visualization, probably will be deleted later so we don't get a MemoryError because
# these get too big. Another option would be to serialize them and save to file but who's got time for that?
time_vector = []
tpco_vector = []
tr_vector = []

ntp = NTPClient()
t = t_0 = ntp.request('1.pl.pool.ntp.org').tx_time
while True:
    try:
        new_t = ntp.request('1.pl.pool.ntp.org').tx_time
        time_delta = new_t - t
        t = new_t
        d_tpco = (fcob*dzeta*(tzco-tpco) - kh*(tpco-tr))/(mh*ch)
        d_tr = (kh*(tpco-tr) - kext*(tr-to))/(mb*cb)
        tr += d_tr*time_delta
        tpco += d_tpco*time_delta
        lock.acquire()
        try:
            data['Tr'] = tr
            data['Tpcob'] = tpco
        finally:
            lock.release()
        time_vector.append(t-t_0)
        tpco_vector.append(tpco)
        tr_vector.append(tr)
        sleep(10)
    # Ctrl-C stops the loop and plots, this will probably go away later but for now it's useful to see that the
    # calculations look sensible and don't run away into infinity
    except (KeyboardInterrupt, NTPException):
        plt.plot(time_vector, tpco_vector)
        plt.show()
        plt.plot(time_vector, tr_vector)
        plt.show()
        server.join()
        break
