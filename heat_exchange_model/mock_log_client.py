import process_model as process
import client
from server_multithread import MultiServer
from sntp_client_new import SntpNew
from itertools import chain
from time import sleep, ctime

state = {'Tpm': 95,
            'Tzco': 90,
            'Time': 0}
logger_host = '192.168.192.111'
logger_port = 51111
log_client = client.LogClient(logger_host, logger_port, state, ['Tpm', 'Tzco'])
while True:
    sleep(1)
    log_client.state = state
    log_client.send_data()

