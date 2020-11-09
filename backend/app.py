from flask import Flask, session, copy_current_request_context
from flask_cors import CORS, cross_origin
from flask_socketio import SocketIO, emit, disconnect
import requests
import sys
from random import random, randint
from datetime import datetime
from threading import Thread, Event
from time import sleep
from math import exp, log

from gevent.pywsgi import WSGIServer
from geventwebsocket.handler import WebSocketHandler

app = Flask(__name__)
app.config['SECRET_KEY'] = 'secret!'
cors = CORS(app)
app.config["CORS_HEADERS"] = 'Content-Type'
socketio = SocketIO(app, cors_allowed_origins="*")



class DataThread(Thread):
    """
    DataThread is a class that allows data operations and communication with the logging station while the server is running.
    It inherit from the Thread class, so it is basically a Thread itself.
    It is started when the first connection is established and it kills itself after a timeout, provided there are no active connections.
    Thread's 'run' method is used to constatly repeat our programLoop method (we can say that it looks somehow like this:
    while True:
        programLoop())
    So in order to change application behavior we should manipulate programLoop method only.
    """

    def __init__(self):
        self.delay = 1
        super(DataThread, self).__init__()
        self.thread_stop_event = False
        self.data = {
                    "plot1": 0,
                    "plot2": 0,
                    "plot3": 0,
                    "plot4": 0,
                    "plot5": 0
                }

    def programLoop(self):
        """
        Base method of the application. It is constatly repeated by a Thread independent from the server.
        There should be all communications and data processing feature implemented.
        """
        print("Initialising")
        try:
            while not self.thread_stop_event:
                # here should be implemented the process of communicating with logging station and updating the data 
                self.data = {key: value + 1  for key, value in self.data.items()}
                # this is responsible for broadcasting the data to all clients connected to the socket
                socketio.emit('responseMessage', {"data": {key: value for key, value in self.data.items()}}, broadcast=True)
                sleep(self.delay)
        # handling keyboard interrupt exception. Do not matter really matter for the app
        except KeyboardInterrupt:
            self.thread_stop_event = True
            print("Terminating the process.")
            sleep(3 * self.delay)
            print("Process terminated")
            sys.exit()

    def run(self):
        self.programLoop()


@socketio.on('connect')
def heater_connect():
    """
    When new connection is established, this method checks whether the thread exists and is working at the moment.
    If the thread isn't working, it creates it / starts it.
    """
    print('Client connected to websocket')
    emit('response', {'data': "Connected"})
    global thread
    if thread is None:
        thread = DataThread()
    if not thread.is_alive():
        print("Starting Thread")
        thread.start()


@socketio.on('disconnect')
def heater_disconnect():
    print('Client disconnected')

@socketio.on_error_default
def default_error_handler(e):
    print('An error occured:')
    print(e)



if __name__ == "__main__":
    # initializing global thread with None in the __main__ part of the program is used for assuring that there exist only one instantion of DataThread class.
    # No more instantions are needed, because data is broadcasted to every client connected to the socket.
    # By design, clients do not have any possibility to interact with the way data is being fetched and processed, so this solution is plaussible. 
    global thread
    thread = None
    http_server = WSGIServer(('',5000), app, handler_class=WebSocketHandler)
    http_server.serve_forever()
        
