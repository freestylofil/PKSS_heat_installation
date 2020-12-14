import socket, json, threading
from time import sleep


#tworzenie obiektu klient socket
class SockClient:

    def __init__(self, host_exch, port):
        self.port = port
        self.host = host_exch
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.connect((self.host, self.port))
        self.data = 1
        self.header = 1024
        self.format = 'utf-8'
        print('[Creating new socket]')

    def receive(self):
        rec = self.sock.recv(self.header)
        rec_data = json.loads(rec.decode(self.format))
        return rec_data

    def send(self, data):
        self.sock.sendall(json.dumps(data).encode(self.format))


#wielowątkowy serwer budynku
class MyServer(threading.Thread):
    def __init__(self, port, host, *args, **kwargs):
        self.header = 1024
        self.port = port
        self.host = host
        self.format = 'utf-8'
        self.server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.server.bind((self.host, self.port))
        self.rec = 0
        print("[STARTING] server is starting...")
        super().__init__(*args, **kwargs)

    def handle_client(self, conn, addr):
        print(f"[NEW CONNECTION] {addr} connected.")

        while conn:
            sleep(1)
            conn.sendall(json.dumps(self.rec).encode(self.format))

    def run(self):
        self.server.listen()
        print(f"[LISTENING] Server is listening on {self.host}")
        while True:
            conn, addr = self.server.accept()
            thread = threading.Thread(target=self.handle_client, args=(conn, addr))
            thread.start()
            print(f"[ACTIVE CONNECTIONS] {threading.activeCount() - 1}")


#pobranie czasu z serwera
def get_time(host, port, time_client):
    time_sample = time_client.request(host, 3, port)
    return time_sample.tx_time

