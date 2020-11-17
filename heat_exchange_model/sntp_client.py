import socket
from threading import Thread
import struct
from copy import deepcopy
import sys

TIME1970 = 2208988800


class Sntp:

    def __init__(self, port):
        self.port = port
        self.current_timestamp = 0
        self.current_delta = 0
        self.prev_timestamp = 0
        self.recv_flag = False
        self.soc = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
        self.soc.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEPORT, 1)
        self.soc.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
        self.soc.bind(("", self.port)) # specific port of udp server

    def listen_for_timestamps(self):
        Thread(target=self.listening).start()

    def listening(self):
        while True:
            data, address = self.soc.recvfrom(1024)
            if data:
                    t = struct.unpack('!12I', data)[10]
                    t -= TIME1970
                    if not self.current_timestamp:
                        self.current_timestamp = t
                    else:
                        self.current_delta = t - self.current_timestamp
                        self.prev_timestamp = deepcopy(self.current_timestamp)
                        self.current_timestamp = t
                        self.recv_flag = True
