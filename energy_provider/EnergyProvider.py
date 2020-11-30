import socket
from jsons import to_json, to_json_log
from time import sleep
from ActualTime import *
from AmbientT import AmbientT
from WaterTemperature import WaterTemperature
from datetime import *

ip = '192.168.192.15'
time_host = '192.168.192.253'
#time_host = 'pl.pool.ntp.org'
time_port = '8123'
HEADER = 64
failure_duration = timedelta(hours=5)
failure_time = datetime.fromtimestamp(0)
failure = False

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect(('192.168.192.15', 8080))
try:
    log = socket.socket(socket .AF_INET, socket.SOCK_STREAM)
    log.connect(('192.168.192.120',55555))
except:
    print('Cannot connect logs')

time = ActualTime(datetime.fromtimestamp(int(datetime.timestamp(datetime.now())))) #ActualTime(get_time(time_host, time_port)) #ActualTime(datetime.fromtimestamp(int(datetime.timestamp(datetime.now()))))   # get_time(time_host))
Tout = AmbientT()
Tzm = WaterTemperature()

def send_data(msg):
    message = msg.encode()
    msg_length = len(message)
    send_length = str(msg_length).encode()
    send_length += b' ' * (HEADER - len(send_length))
    s.send(message)

def send_log(msg):
    message = msg.encode()
    msg_length = len(message)
    send_length = str(msg_length).encode()
    send_length += b' ' * (HEADER - len(send_length))
    log.send(message)

To = Tout.temperature
while 1:
    try:
        time.date =  datetime.fromtimestamp(int(datetime.timestamp(datetime.now()))) #get_time(time_host, time_port)"""  # get_time(time_host)
        if time.check_period():
            if failure == False:
                Tout.temperature = time.date.hour
                To = Tout.temperature
                Tzm.temperature = To
                failure = Tzm.failure()
                failure_time = time.date
            else:
                failure = False if time.date-failure_time > failure_duration else True
        send_data(to_json(ip, time.get_timestamp(), To, Tzm.temperature))
        #send_log(to_json_log('To', time.get_timestamp(), To))
        #send_log(to_json_log('Tzm', time.get_timestamp(), Tzm.temperature))
        sleep(1)
    except:
        pass
s.close()
