# Konfiguracja na początku maina
# instalacja potrzebnego pakietu: pip install tinydb

import threading
import socketserver
from tinydb import TinyDB, Query
from typing import Optional


def delete_db() -> None:
    with db_lock:
        db.truncate()


def log_into_db(table: str, record: dict) -> None:
    with db_lock:
        tb = db.table(table)
        tb.insert(record)


def get_from_db(table: str, timestamp_from: Optional[int] = None, timestamp_to: Optional[int] = None) -> str:
    with db_lock:
        tb = db.table(table)
        if timestamp_from is None:
            timestamp_from = 0
        if timestamp_to is None:
            timestamp_to = tb.all()[-1]["timestamp"]
        data = str()
        i = 0
        for item in tb:
            if timestamp_from <= item["timestamp"] <= timestamp_to:
                data = "{}\"{}\": {}, ".format(data, str(i), str(item).replace('\'', '\"'))
                i = i + 1

    return "{{{}}}".format(data[0:-2])


class ThreadedTCPRequestHandler(socketserver.BaseRequestHandler):
    def handle(self):
        cur_thread = threading.current_thread()
        print("\u001b[34;1m[{th}:INFO]\u001b[0m New connection:\n"
              "\u001b[34;1m[{th}] -->\u001b[0m in thread: {th}\n"
              "\u001b[34;1m[{th}] -->\u001b[0m connected from: {ip}"
              "".format(th=cur_thread.name, ip=self.client_address[0]))
        while True:
            response = str()
            try:
                data = str(self.request.recv(1024), 'utf-8').strip()

                # ----- if data is json ----- #
                if data[0] != '{':
                    raise Exception("invalid data type")
                try:
                    data = eval(data)
                except:
                    raise Exception("invalid data type")
                if type(data) is not dict:
                    raise Exception("invalid data type")
                # ----- end if ----- #

                request_type = data.pop("request")

                # ----- handling PUT request ----- #
                if request_type == "PUT":
                    var = data.pop("variable")
                    log_into_db(var, data)
                    print("\u001b[34;1m[{}:INFO]\u001b[0m "
                          "Data has been saved in \u001b[36;1m{}\u001b[0m table".format(cur_thread.name, var))
                    response = bytes("OK", 'utf-8')
                # ----- handling GET request ----- #
                elif request_type == "GET":
                    var = data.pop("variable")
                    try:
                        timestamp_1 = data.pop("time_from")
                    except KeyError:
                        timestamp_1 = None
                    try:
                        timestamp_2 = data.pop("time_to")
                    except KeyError:
                        timestamp_2 = None
                    print("\u001b[34;1m[{}:INFO]\u001b[0m "
                          "Processing GET request from \u001b[36;1m{}\u001b[0m table".format(cur_thread.name, var))
                    response = bytes(get_from_db(var, timestamp_1, timestamp_2), 'utf-8')
                # ----- handling delete command ----- #
                elif request_type == "TRUNCATE":
                    delete_db()
                    print("\u001b[34;1m[{}:INFO]\u001b[0m "
                          "Processing TRUNCATE request.".format(cur_thread.name))
                    response = bytes("Database has been deleted", 'utf-8')
                # ----- handling bad requests ----- #
                else:
                    raise Exception("invalid request type")
                # ----- ----- ----- ----- ----- #

            except (SyntaxError, Exception) as err:
                response = bytes("[Error] Bad request ({})".format(err.args[0]), 'utf-8')
                print("\u001b[34;1m[{}:\u001b[31;1mERROR\u001b[34;1m]\u001b[0m "
                      "Request has been rejected".format(cur_thread.name))

            except KeyError:
                response = bytes("[Error] Bad request (invalid data)", 'utf-8')
                print("\u001b[34;1m[{}:\u001b[31;1mERROR\u001b[34;1m]\u001b[0m "
                      "Request has been rejected".format(cur_thread.name))

            except:
                response = bytes("[Error] Bad request (unexpected error)", 'utf-8')
                print("\u001b[34;1m[{}:\u001b[31;1mERROR\u001b[34;1m]\u001b[0m "
                      "Request has been rejected".format(cur_thread.name))

            finally:
                self.request.sendall(response)
                print("\u001b[34;1m[{}:INFO]\u001b[0m "
                    "Response has been sent".format(cur_thread.name))


class ThreadedTCPServer(socketserver.ThreadingMixIn, socketserver.TCPServer):
    pass


if __name__ == "__main__":

    '''
    Konfiguracja serwera:
    db = TinyDB('db.json') - połączenie do bazy danych
    db_lock - umożliwa dostęp do bazy danych tylko przez 1 wątek w danym momencie
    HOST, PORT - adres IP serwera 
    '''

    db = TinyDB('db.json')
    db_lock = threading.Lock()
    HOST, PORT = "192.168.192.120", 55555
    # HOST, PORT = "localhost", 55555
    server = ThreadedTCPServer((HOST, PORT), ThreadedTCPRequestHandler)

    with server:
        ip, port = server.server_address
        server_thread = threading.Thread(target=server.serve_forever)
        server_thread.daemon = True
        server_thread.start()
        print("\u001b[34;1m[INFO]\u001b[0m Server is \u001b[32;1mrunning\u001b[0m:\n"
              "\u001b[34;1m-->\u001b[0m in thread: {}\n"
              "\u001b[34;1m-->\u001b[0m on address: {}:{}".format(server_thread.name, ip, port))
        try:
            while True:
                pass
        except KeyboardInterrupt:
            server.shutdown()
            print("\u001b[34;1m[INFO]\u001b[0m Server has been \u001b[31;1mstopped\u001b[0m.")
