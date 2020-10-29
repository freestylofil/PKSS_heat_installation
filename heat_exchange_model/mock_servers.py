from server_multithread import MultiServer

if __name__ == "__main__":
    #mock data
    host1 = '127.0.0.1'
    port1 = 65000
    state1 = {'Fzm': 55,
             'Fzco': 150,
             'Tpco': 105}

    host2 = '127.0.0.1'
    port2 = 65001
    state2 = {'Tzm': 55,
              'To': 20}
    server1 = MultiServer(host1, port1, state1)
    server2 = MultiServer(host2, port2, state2)
    server1.listen()
    server2.listen()