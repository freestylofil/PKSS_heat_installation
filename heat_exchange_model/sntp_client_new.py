from ntplib import NTPClient
from copy import deepcopy
import socket

class SntpNew:
    def __init__(self, host, port):
        self.host = host
        self.port = port
        self.delta = 0
        self.current_time = 0
        self.previous_time = 0
        self.client = NTPClient()

    def get_time(self):
        try:
            timestamp = self.client.request(self.host, 3, self.port)
            if self.current_time == 0:
                self.current_time = timestamp.tx_time
                return
            self.delta = timestamp.tx_time - self.current_time
            self.previous_time = deepcopy(self.current_time)
            self.current_time = timestamp.tx_time
        except socket.timeout:
            return
