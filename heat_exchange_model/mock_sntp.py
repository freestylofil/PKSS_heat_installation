import process_model as process
import client as client
from server_multithread import MultiServer
from sntp_client import Sntp
from itertools import chain
from time import sleep, ctime


sntp_port = 8123 #mock sntp port
sntp = Sntp(sntp_port)
sntp.listen_for_timestamps()

while True:
    if sntp.recv_flag:
        print(sntp.current_delta)
        sntp.recv_flag = False