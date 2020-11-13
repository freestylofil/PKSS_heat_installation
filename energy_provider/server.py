import socket
import threading
from typing import Dict, Any

HOST = '192.168.1.2'
PORT = 8080
BUFFER = 1024
HEADER = 64
clients = {}
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

try:
    s.bind((HOST, PORT))
except socket.error:
    print(str(socket.error))


def client_thr(con, addr):
    connected = True
    while connected:
        data_len = con.recv(HEADER).decode()
        if data_len:
            data = con.recv(int(data_len))
        print(f'[{addr[0]}:{addr[1]}]{data.decode()}')
        for c in clients:
            if clients[c]!=con:
                clients[c].sendall(data)
    con.close()


s.listen()
while 1:
    conn, addr = s.accept()
    clients[addr] = conn
    print(f"Połączenie z {addr[0]}:{addr[1]}")
    thread = threading.Thread(target=client_thr, args=(conn, addr))
    thread.start()
    print(threading.active_count() - 1)
