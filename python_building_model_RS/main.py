from ntplib import NTPClient, NTPException
from server import ServerThread
import matplotlib.pyplot as plt
from time import sleep
import logging
from threading import Lock
import socket
import json
import datetime


def request_data(address: str, port: int, variable_name: str):
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
        sock.connect((address, port))
        sock.sendall(b'request')
        response = sock.recv(1024)
        response = json.loads(response.decode('urf-8'))
        return response[variable_name]


logging.basicConfig(level=logging.DEBUG)

# Other adresses
heat_exchanger = ('0.0.0.0', 999999)
energy_provider = ('0.0.0.0', 999999)

# Model parameters
mb = 20000
cb = 1000
kext = 15000
mh = 3000
ch = 2700
kh = 12000
dzeta = 1000
cw = 4200
max_flow = 140

# Regulator parameters
P = 1
I = 1
D = 1

# Mocked data from other nodes
to = 5
tzco = 80

# Initial values for data calculated by the model
tr = 20
tpco = 75

# Initial flow
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
last_error = 0
error_integral = 0
while True:
    try:
        to = request_data(energy_provider[0], energy_provider[1], 'To')
        tzco = request_data(heat_exchanger[0], heat_exchanger[1], 'Tzco')
        new_t = ntp.request('1.pl.pool.ntp.org').tx_time
        time_delta = new_t - t
        t = new_t
        time_info = datetime.datetime.fromtimestamp(t)
        day = time_info.weekday()
        hour = time_info.hour
        target_temp = 20 if day <= 5 and 7 <= hour <= 22 else 15
        error = target_temp - tr
        error_derivative = (error - last_error)/time_delta
        error_integral += error*time_delta
        fcob = min(max_flow, P*error + I*error_integral + D*error_derivative)
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
