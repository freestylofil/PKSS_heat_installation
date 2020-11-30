import socket, json, threading
from time import sleep


class SockClient:

    def __init__(self, host_exch, port):
        self.port = port
        self.host = host_exch
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.connect((self.host, self.port))
        self.data = 1
        self.header = 1024
        self.format = 'utf-8'
        print('[Utworzono socket]')

    def receive(self):
        rec = self.sock.recv(self.header)
        rec_data = json.loads(rec.decode(self.format))
        return rec_data

    def send(self, data):
        self.sock.sendall(json.dumps(data).encode(self.format))


# time_client = ntplib.NTPClient()
#
#
# def get_time(host, port):
#     now = time_client.request(host, 3,port)
#     return datetime.fromtimestamp(int(now.tx_time))


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
            dumm=conn.recv(self.header)
            conn.sendall(json.dumps(self.rec).encode(self.format))
            # self.rec = json.loads(conn.recv(self.header).decode(self.format))
            # print("[DATA SEND]  T_R: {a:10.2f}  T_PCO: {b:10.2f}  F_COB: {c:10.4f}  timestamp:{d:10.10f}".format(a=self.rec['Tr'], b=self.rec['Tpco'], c=self.rec['Fcob'], d=self.rec['timestamp']))

        conn.close()

    def run(self):
        self.server.listen()
        print(f"[LISTENING] Server is listening on {self.host}")
        while True:
            print("b")
            conn, addr = self.server.accept()
            print("c")
            thread = threading.Thread(target=self.handle_client, args=(conn, addr))
            thread.start()
            print(f"[ACTIVE CONNECTIONS] {threading.activeCount() - 1}")


def get_time(host, port, time_client):
    now = time_client.request(host, 3 , port)
    return now.tx_time