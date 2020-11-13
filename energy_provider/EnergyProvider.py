import socket
from jsons import to_json
from time import sleep
from ActualTime import *
from AmbientTemperature import AmbientTemperature
from WaterTemperature import WaterTemperature
from datetime import *

location = 'Temperatura.xlsx'
time_host = 'pl.pool.ntp.org'
ip = '192.168.1.2'
HEADER = 64

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect(('192.168.1.2', 8080))

time = ActualTime(datetime.fromtimestamp(int(datetime.timestamp(datetime.now()))))   # get_time(time_host))
print(time.date)
Tout = AmbientTemperature(location)
Tzm = WaterTemperature()


def send(msg):
    message = msg.encode()
    msg_length = len(message)
    send_length = str(msg_length).encode()
    send_length += b' ' * (HEADER - len(send_length))
    s.send(send_length)
    s.send(message)


while 1:
    time.date = datetime.fromtimestamp(int(datetime.timestamp(datetime.now())-0.05))  # get_time(time_host)
    print(time.date)
    if time.check_period():
        To = Tout.temperature
        Tzm.temperature = To
    send(to_json(ip, time.get_timestamp(), To, Tzm.temperature))
    sleep(1)
s.close()
