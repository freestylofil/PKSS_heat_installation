import socket
import threading
from datetime import datetime

HOST = '192.168.192.15'
#HOST = '192.168.1.3'
PORT = 8080
clients = {}
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

try:
    s.bind((HOST, PORT))
except socket.error:
    print(str(socket.error))


def client_thr(con, addr):
    global send_data
    connected = True
    try:
        con.sendall(send_data)
    except:
        pass
    while connected:
        data = con.recv(4096)
        if data:
            if addr[0] == HOST:
                send_data = data
                #print(f'Rx[{addr[0]}:{addr[1]}]{data.decode()}')
            else:
                con.sendall(send_data)
                #print(f'{data.decode()},{datetime.fromtimestamp(int(datetime.timestamp(datetime.now())-0.05))}')
                print(f'Tx[{addr[0]}:{addr[1]}]')
        # data_len = con.recv(HEADER).decode()
        # if data_len:
        #     data = con.recv(int(data_len))
        #     print(f'[{addr[0]}:{addr[1]}]{data.decode()}')
        # for c in clients:
        #     if clients[c]!=con:
        #         clients[c].sendall(data)
    con.close()


s.listen()
while 1:
    conn, addr = s.accept()
    clients[addr] = conn
    print(f"Połączenie z {addr[0]}:{addr[1]}")
    thread = threading.Thread(target=client_thr, args=(conn, addr))
    thread.start()
