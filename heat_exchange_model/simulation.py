import process_model as process
import client as client
from server_multithread import MultiServer
from sntp_client import Sntp
from itertools import chain
from time import sleep, ctime

if __name__ == "__main__":
    #mock data
    host = '127.0.0.1'
    port = 65432
    state = {'Tpm': 95,
            'Tzco': 90,
            'Time': 0}
    model = process.ProcessModel([state['Tpm'], state['Tzco']])
    server = MultiServer(host, port, state)
    sntp_port = 12345 #mock sntp port
    sntp = Sntp(sntp_port)

    sntp.listen_for_timestamps()
    server.listen()
    sleep(0.2)
    # delta = 5
    while True:
        # sleep(delta) #mock_for_time
        # data_controller = client.run_client_controller('127.0.0.1', 65000)
        # data_energy = client.run_client_energy('127.0.0.1', 65001)
        # model.simulate((data_energy['Tzm'], data_controller['Fzm'], data_controller['Fzco'],
        #                 data_controller['Tpco']))
        # # print(model.x0) # for testing
        # server.current_state = model.x0
        # if not server.working:
        #     print("Application closing")
        #     break
        if not server.working:
            print("Application closing")
            break
        if sntp.recv_flag and sntp.prev_timestamp:
            data_controller = client.run_client_controller('127.0.0.1', 65000)
            data_energy = client.run_client_energy('127.0.0.1', 65001)
            model.simulate((data_energy['Tzm'], data_controller['Fzm'], data_controller['Fzco'],
                            data_controller['Tpco']), delta=sntp.current_delta)
            # print(model.x0) # for testing
            server.current_state = \
                dict(chain.from_iterable(d.items() for d in (model.x0, ctime(sntp.current_timestamp))))
            sntp.recv_flag = False