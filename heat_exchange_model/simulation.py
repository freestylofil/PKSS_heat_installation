from socket import timeout
import process_model as process
from ntplib import NTPException
import client
from server_multithread import MultiServer
from sntp_client_new import SntpNew
from itertools import chain
from time import sleep

if __name__ == "__main__":
    #mock data
    host = '192.168.192.123'
    port = 43516
    state = {'Tpm': 95,
            'Tzco': 90,
            'Time': 0}
    model = process.ProcessModel([state['Tpm'], state['Tzco']])
    server = MultiServer(host, port, state)
    print("running")

    is_energy = False
    is_controller = False
    is_logger = False
    controller_host = '192.168.192.100'
    controller_port = 4790
    energy_host = '192.168.192.15'
    energy_port = 8080
    if is_controller:
        controller = client.Client(controller_host, controller_port, 'controller')
    if is_energy:
        energy = client.Client(energy_host, energy_port, 'energy')
    print("running")
    sntp_port = 8123
    sntp_host = '192.168.192.253'
    time_getter = SntpNew(sntp_host, sntp_port)

    logger_host = '192.168.192.120'
    logger_port = 55555
    if is_logger:
        log_client = client.LogClient(logger_host, logger_port, state, ['Tpm', 'Tzco'])
    server.listen()
    sleep(0.1)
    delta = 1
    while True:
        print("running")
        sleep(delta)
        try:
            time_getter.get_time()
            time_dict = {'Time': time_getter.current_time}
        except NTPException:
            try:
                print('Connection with time lost. Trying to reestablish...')
                time_getter = SntpNew(sntp_host, sntp_port)
                time_getter.get_time()
                time_dict = {'Time': time_getter.current_time}
            except timeout:
                continue
        except NameError:
            print('Previous time object not present, remaking...')
            time_getter = SntpNew(sntp_host, sntp_port)
            time_getter.get_time()
            time_dict = {'Time': time_getter.current_time}

        if time_getter.delta == 0:
            continue
        if is_controller:
            data_controller = controller.run_client()
            print(data_controller)
        else:
            data_controller = {'Fzm': 40, 'Fzco': 30, 'Tpco': 87}
        # print('running1')
        if is_energy:
            data_energy = energy.run_client()
            print(data_energy)
        else:
            data_energy = {'Tzm': 95}
        # print('running2')
        model.simulate((data_energy['Tzm'], data_controller['Fzm'], data_controller['Fzco'],
                        data_controller['Tpco']))
        print(model.x0) # for testing
        state = dict(chain.from_iterable(d.items() for d in (model.x0, time_dict)))
        server.current_state = state
        if is_logger:
            log_client.state = state
            log_client.send_data()
        if not server.working:
            print("Application closing")
            break
        # if sntp.recv_flag and sntp.prev_timestamp:
        #     data_controller = client.run_client_controller('192.168.192.100', 4790)
        #     # data_energy = client.run_client_energy('192.168.192.15', 8080)
        #     data_energy = {'Tzm': 100}
        #     model.simulate((data_energy['Tzm'], data_controller['Fzm'], data_controller['Fzco'],
        #                     data_controller['Tpco']), delta=sntp.current_delta)
        #     # print(model.x0) # for testing
        #     server.current_state = \
        #         dict(chain.from_iterable(d.items() for d in (model.x0, ctime(sntp.current_timestamp))))
        #     sntp.recv_flag = False