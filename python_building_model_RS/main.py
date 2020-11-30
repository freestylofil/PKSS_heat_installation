from ntplib import NTPClient, NTPException
from server import ServerThread
import matplotlib.pyplot as plt
from time import sleep
import logging
from threading import Lock
from Client import Client
import datetime


if __name__ == '__main__':
    logging.basicConfig(level=logging.INFO)

    # Other adresses
    heat_exchanger = ('192.168.192.123', 43516)
    energy_provider = ('192.168.192.15', 8080)
    logger = ('192.168.192.120', 55555)
    time_server = ('192.168.192.253', 8123)
    # Model parameters
    mb = 20000
    cb = 1000
    kext = 15000
    mh = 3000
    ch = 2700
    kh = 12000
    dzeta = 1000
    cw = 4200
    max_flow = 40/3600*1000

    # Regulator parameters
    P = 1.5
    I = 0.002
    D = 0

    # Mocked data from other nodes
    to = 5
    tzco = 80

    # Initial values for data calculated by the model
    tr = 20
    tpco = 75

    # Initial flow
    fcob = 40/3600*1000

    # Data dict we'll be modifying and sending
    data = {'Tpcob': tpco, 'Fcob': fcob, 'Tr': tr, 'timestamp': 0}
    lock = Lock()

    host = '192.168.192.111'
    port = 51111
    server = ServerThread(data, host, port)
    server.start()

    # Used for local logging and visualization, probably will be deleted later so we don't get a MemoryError because
    # these get too big. Another option would be to serialize them and save to file but who's got time for that?
    time_vector = []
    tpco_vector = []
    tr_vector = []
    error_vector = []
    fcob_vector = []

    ntp = NTPClient()
    # t = t_0 = ntp.request(time_server[0], port=time_server[1]).tx_time
    t = t_0 = datetime.datetime.now().timestamp()
    data['timestamp'] = t
    last_error = 0
    error_integral = 0
    energy_client = Client(energy_provider[0], energy_provider[1], variable_name='To')
    energy_client.connect()
    heat_client = Client(heat_exchanger[0], heat_exchanger[1], variable_name='Tzco')
    heat_client.connect()
    logging_client = Client(logger[0], logger[1], to_send=data)
    logging_client.connect()
    # new_to = 'Timeout'
    # new_tzco = 'Timeout'
    while True:
        try:
            # send_to_logging('192.168.192.120', 55555, data)
            logging_client.send()
            new_to = energy_client.receive()
            to = new_to if new_to != 'Timeout' else to
            # print(to)
            # tzco = request_data(heat_exchanger[0], heat_exchanger[1], 'Tzco')
            new_tzco = heat_client.receive()
            tzco = new_tzco if new_tzco != 'Timeout' else tzco
            # print(tzco)
            try:
                new_t = ntp.request(time_server[0], port=time_server[1]).tx_time
                # new_t = t + 6
            except NTPException:
                new_t = t
            time_delta = new_t - t
            t = new_t
            # print(t)
            data['timestamp'] = t
            time_info = datetime.datetime.fromtimestamp(t)
            day = time_info.weekday()
            hour = time_info.hour
            target_temp = 20 if day <= 5 and 7 <= hour <= 22 else 15
            error = target_temp - tr
            error_derivative = (error - last_error)/time_delta
            error_integral += error*time_delta
            fcob = max(0, min(max_flow, P*error + I*error_integral + D*error_derivative))
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
            # sleep(1)
        # Ctrl-C stops the loop and plots, this will probably go away later but for now it's useful to see that the
        # calculations look sensible and don't run away into infinity
        except KeyboardInterrupt:
            time_vector.append(t-t_0)
            tpco_vector.append(tpco)
            tr_vector.append(tr)
            error_vector.append(error)
            fcob_vector.append(fcob)
            plt.plot(time_vector, tpco_vector)
            plt.title('Tpco')
            plt.show()
            plt.plot(time_vector, tr_vector)
            plt.title('Tr')
            plt.show()
            server.join()
            plt.plot(time_vector, error_vector)
            plt.title('Error')
            plt.show()
            plt.plot(time_vector, fcob_vector)
            plt.title('Fcob')
            plt.show()
            break
        time_vector.append(t - t_0)
        tpco_vector.append(tpco)
        tr_vector.append(tr)
        error_vector.append(error)
        fcob_vector.append(fcob)
