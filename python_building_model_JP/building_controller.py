from ntplib import NTPClient
from time import sleep
import json
import functions, threading

host = '127.0.0.1'
port = 52344
# host='192.168.192.12'
print("abcd213123")
server = functions.MyServer(port, host)
print("abcd")
#server.start_sock()
server.start()




print("a")
PID_P= 1 / 1000
PID_I= 1 / 50000
PID_Sum=0

PID_U=25
PID_E=0
PID_Y=0

dt = 10 # czas serwera
T_O = 5  # temp otoczenia - zewnetrzne
T_ZCO = 0  # temp wody z wymiennika - zewnetrzne

T_R = 0  # temp pomieszczen
T_PCO = 80  # temp wody powrotnej
T_R_Prim = 0
T_PCO_Prim = 0

rho = 1000  # gestosc wody
c_W = 4200  # cieplo wlasciwe wody

F_COB = 15 / 3600  # aktualny pobór wody w budynku, max 40m3/h
F_COB_MAX = 40 / 3600
k_h = 12000  # współczynnik przenikania ciepła kaloryfer-pokój
m_h = 3000  # masy zastępcze wszystkich kaloryferów [kg]
c_h = 2700  # zastępcze ciepło właściwe kaloryferów
m_b = 20000  # masa powietrza w budynku
c_b = 1000  # ciepło właściwe powietrza
k_ext = 15000  # współczynnik utraty ciepła

host = '192.168.192.12'
port = 52344

state = {'Tpco': T_PCO, 'Fcob': F_COB, "Tr": T_R, "timestamp": t}
host_k = '192.168.192.100' #Kamil
port_k = 4790
host_e = '192.168.192.123' #Michał
port_e = 43516
host_l = '192.168.192.120' #Przemek
port_l = 55555
host_o = '192.168.192.15' #Adam
port_o = 8080
host_t = '192.168.192.253'
port_t = 8123
host = '192.168.192.12'
port = 52345
host = '127.0.0.1'
port = 52344


# sock_exch = functions.SockClient(host_e, port_e)
print('tk5')
# sock_outdoor = functions.SockClient(host_o, port_o)
print('tk1')
# sock_log = functions.SockClient(host_l, port_l)
print('tk2')
# sock_con = functions.SockClient(host_k, port_k)
print('tk3')

sock_send = functions.SockClient(host, port)
# sock_other = functions.SockClient(host, 52344)

resp = True

time_client = NTPClient()   #serwer
t = functions.get_time(host_t, port_t, time_client)
ntp = NTPClient()
t = ntp.request('europe.pool.ntp.org').tx_time

state = {'Tpco': T_PCO, 'Fcob': F_COB, "Tr": T_R, "timestamp": t}


while True:

    while resp:

        try:

            # t_prim = functions.get_time(host_t, port_t, time_client)
            # dt = t_prim-t
            # t = t_prim

            # rec_zco = sock_exch.receive()
            # rec_o = sock_outdoor.receive()
            # T_O = rec_o[To]
            # T_ZCO = rec_zco[Tzco]
            T_ZCO = 80

            testt = sock_send.receive()
            print(testt)

            PID_Sum = PID_Sum + PID_E
            PID_E = PID_U - T_R
            PID_Y = PID_P * PID_E + PID_I * PID_Sum

            if PID_Y>F_COB_MAX:
                PID_Y=F_COB_MAX

            if PID_Y<0:
                PID_Y=0

            F_COB=PID_Y

            T_PCO_Prim = dt * (F_COB * rho * c_W * (T_ZCO - T_PCO) - k_h * (T_PCO - T_R)) / (m_h * c_h)
            T_R_Prim = dt * (k_h * (T_PCO - T_R) - k_ext * (T_R - T_O)) / (m_b * c_b)

            T_R = T_R + T_R_Prim
            T_PCO = T_PCO + T_PCO_Prim

            print("T_R: {a:10.2f}  T_PCO: {b:10.2f}  F_COB: {c:10.4f}  T_PCO_Prim:{d:10.2f} pid_E:{e:10.10f}  pid_Esum:{f:10.10f}".format(a=T_R, b=T_PCO, c=F_COB, d=T_PCO_Prim, e=PID_E, f=PID_Sum))

            state = {'Tpco': T_PCO, 'Fcob': F_COB, "Tr": T_R, "timestamp": t}

            log = {"request": "PUT", "variable": "Tcob", "timestamp": t, "value": F_COB }
            server.rec = state
            # sock_log.sock.send(log)
            # sock_con.send(state)
            # sock_exch.send(state)

        except(ConnectionResetError):
            sock_send.close()
            # sock_out.close()
            break
