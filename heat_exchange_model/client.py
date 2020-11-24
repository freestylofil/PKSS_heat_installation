import socket
from json import loads, dumps


class Client:
    def __init__(self, host, port, addressee):
        self.host = host
        self.port = port
        self.addressee = addressee
        if addressee not in ['controller', 'energy', 'mock']:
            raise ValueError
        self.state = []
        self.soc = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.soc.connect((self.host, self.port))

    def _run_client_controller(self):
        self.soc.sendall(b'Data request')
        data = loads(self.soc.recv(1024).decode('utf-8'))
        return data

    def _run_client_energy(self):
        self.soc.sendall(b'Data request')
        data = loads(self.soc.recv(1024).decode('utf-8'))
        return data

    def _run_client_mock(self):
        self.soc.sendall(b'Data request')
        data = loads(self.soc.recv(1024).decode('utf-8'))
        return data

    def run_client(self):
        if self.addressee == 'controller':
            return self._run_client_controller()
        elif self.addressee == 'energy':
            return self._run_client_energy()
        elif self.addressee == 'mock':
            return self._run_client_mock()


class LogClient:
    def __init__(self, host, port, state, variables):
        self.host = host
        self.port = port
        self.state = state
        self.variables = variables
        self.soc = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.soc.connect((self.host, self.port))

    def send_data(self):
        for i in self.variables:
            dumper = {"request": "PUT", "variable": i, "timestamp": self.state['Time'],
                   "value": self.state[i]}
            self.soc.sendall(dumps(dumper).encode('utf-8'))
            print(f'Received from logger: {self.soc.recv(1024).decode("utf-8")}')




