from sntp_client_new import SntpNew
from time import sleep, ctime

sntp_port = 8123
sntp_host = '192.168.192.253'
time_getter = SntpNew(sntp_host, sntp_port)

while True:
    sleep(1)
    time_getter.get_time()
    print(time_getter.current_time)