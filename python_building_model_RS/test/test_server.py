from server import ServerThread
from unittest import TestCase
import logging
import socket
import json


logging.basicConfig(level=logging.DEBUG)


class TestServer(TestCase):
    @classmethod
    def setUpClass(cls):
        cls.data = {'a': 1, 'b': 2}
        cls.server = ServerThread(cls.data, 'localhost', 50009)
        cls.server.start()

    @classmethod
    def tearDownClass(cls):
        cls.server.join()

    def test_server_response(self):
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
            sock.connect(('localhost', 50009))
            sock.sendall(b'Can I have a dictionary please?')
            response = json.loads(sock.recv(1024).decode('utf-8'))
            print(f'received {response}')
            self.assertTrue(response['a'] == 1)
            self.assertTrue(response['b'] == 2)
        self.data['a'] = 2
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
            sock.connect(('localhost', 50009))
            sock.sendall(b'Can I have a dictionary please?')
            response = json.loads(sock.recv(1024).decode('utf-8'))
            print(f'received {response}')
            self.assertTrue(response['a'] == 2)
            self.assertTrue(response['b'] == 2)

    def test_server_stability(self):
        requests = 100
        for request in range(requests):
            with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
                try:
                    sock.connect(('localhost', 50009))
                    sock.sendall(b'Can I have a dictionary please?')
                    response = json.loads(sock.recv(1024).decode('utf-8'))
                    self.assertTrue(isinstance(response, dict))
                except Exception as err:
                    self.fail(f'exception {err} raised')
