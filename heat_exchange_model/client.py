import socket
from json import loads


def run_client_controller(host, port):
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((host, port))
        s.sendall(b'Data request')
        data = loads(s.recv(1024))

    return data


def run_client_energy(host, port, mock=False):
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((host, port))
        s.sendall(b'Data request')
        data = loads(s.recv(1024))
    if mock:
        print(f'Received {repr(data)}')
    else:
        return data
