import socket
from threading import Thread
from json import dumps
from itertools import chain


class MultiServer:
    def __init__(self, host, port, state):
        self.host = host
        self.IP = {'IP': self.host}
        self.port = port
        self.current_state = state
        self.working = True
        self.soc = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.soc.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.soc.bind((self.host, self.port))

    def listen(self):
        Thread(target=self.listening).start()

    def listening(self):
        print("Listening...")
        time = 30
        self.soc.settimeout(time)
        self.soc.listen(5)
        while True:
            try:
                client, address = self.soc.accept()
                client.settimeout(30)
                print(f'Connection established with {address}')
                Thread(target=self.client_service, args=(client, address)).start()
            except:
                print(f"No one connected after {time} seconds, closing app")
                self.soc.close()
                self.working = False
                break

    def client_service(self, client, address):
        data_size = 1024
        while True:
            try:
                data = client.recv(data_size)
                if data:
                    client.sendall(dumps(dict(chain.from_iterable(d.items() for d in (self.IP, self.current_state)))).encode('utf-8'))
                else:
                    raise OSError("Client left")
            except:
                client.close()
                return False

    def set_state(self, state):
        self.current_state = state
