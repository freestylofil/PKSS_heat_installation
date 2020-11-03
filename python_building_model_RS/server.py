from threading import Thread, Lock
from socketserver import TCPServer, BaseRequestHandler, ThreadingMixIn
from json import dumps
import logging


class SimulationDataHandler(BaseRequestHandler):

    def handle(self):
        while True:
            data = self.request.recv(1024).strip()
            if not data:
                logging.log(logging.DEBUG, f"{self.client_address[0]} closed")
                return
            logging.log(logging.DEBUG, f"{self.client_address[0]} wrote: {data}")
            self.server.lock.acquire()
            try:
                self.request.sendall(dumps(self.server.state).encode('utf-8'))
            finally:
                self.server.lock.release()


class ObjectSharingTCPServer(ThreadingMixIn, TCPServer):

    def __init__(self, state, *args, **kwargs):
        self.state = state
        self.lock = Lock()
        super().__init__(*args, **kwargs)


class ServerThread(Thread):

    def __init__(self, state, host, port, *args, **kwargs):
        self.server = ObjectSharingTCPServer(state, (host, port), SimulationDataHandler)
        super().__init__(*args, **kwargs)

    def run(self):
        self.server.serve_forever()

    def join(self, timeout=None):
        self.server.shutdown()
        super().join(timeout)
