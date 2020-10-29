import process_model as process
import client as client
from server_multithread import MultiServer
from time import sleep

if __name__ == "__main__":
    #mock data
    host = '127.0.0.1'
    port = 65432
    state = {'Tpm': 95,
           'Tzco': 90}
    model = process.ProcessModel([95, 90])
    server = MultiServer(host, port, state)
    # starting to listen
    server.listen()
    delta = 5
    while True:
        sleep(delta) #mock_for_time
        data_controller = client.run_client_controller('127.0.0.1', 65000)
        data_energy = client.run_client_energy('127.0.0.1', 65001)
        model.simulate((data_energy['Tzm'], data_controller['Fzm'], data_controller['Fzco'],
                        data_controller['Tpco']))
        # print(model.x0) # for testing
        server.current_state = model.x0
        if not server.working:
            print("Application closing")
            break