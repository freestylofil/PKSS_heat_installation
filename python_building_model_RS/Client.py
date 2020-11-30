import socket
import json
from typing import Union
import logging


class Client:

    def __init__(self, address: str, port: int, variable_name: Union[str, None] = None,
                 to_send: Union[dict, None] = None):
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.variable_name = variable_name
        self.to_send = to_send
        self.address = address
        self.port = port

    def connect(self):
        connected = False
        while not connected:
            try:
                self.sock.connect((self.address, self.port))
                connected = False
            except TimeoutError:
                pass

    def receive(self):
        self.sock.sendall(b'request')
        try:
            value = json.loads(self.sock.recv(1024).decode('utf-8'))[self.variable_name]
        except TimeoutError:
            value = 'Timeout'
        return value

    def send(self):
        if self.to_send:
            for key, value in self.to_send.items():
                if key == 'timestamp':
                    continue
                formatted = {'request': 'PUT', 'variable': f'{key}_1',
                             'timestamp': self.to_send['timestamp'], 'value': value}
                # print(json.dumps(formatted).encode('utf-8'))
                self.sock.sendall(json.dumps(formatted).encode('utf-8'))
                logging.log(logging.INFO, f'logging said: {self.sock.recv(1024).decode("utf-8")}')

    def close(self):
        self.sock.close()

    def __del__(self):
        self.close()
