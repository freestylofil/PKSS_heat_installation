from ntplib import NTPClient, NTPException
from time import sleep
import functions


#uruchumienie serwera budynku
port = 52345
host = '192.168.192.12'
server = functions.MyServer(port, host)

#parametry regulatora PI
PID_P = 1 / 2000
PID_I = 1 / 400000

PID_U = 20 #wartość zadana temp pomieszczeń
PID_E = 0
PID_Y = 0
PID_Sum = 0

T_O = 5  # temp otoczenia - dane zewnetrzne
T_ZCO = 80  # temp wody z wymiennika - dane zewnetrzne

T_R = 0  # temp pomieszczen
T_PCO = 0  # temp wody powrotnej
T_R_Prim = 0
T_PCO_Prim = 0

rho = 1000  # gestosc wody
c_W = 4200  # cieplo wlasciwe wody

F_COB = 15 / 3600  # aktualny pobór wody w budynku [m3/s]
F_COB_MAX = 40 / 3600 #maksymalny pobór wody [m3/s] (max 40m3/h)
k_h = 12000  # współczynnik przenikania ciepła kaloryfer-pokój
m_h = 3000  # masy zastępcze wszystkich kaloryferów
c_h = 2700  # zastępcze ciepło właściwe kaloryferów
m_b = 20000  # masa powietrza w budynku
c_b = 1000  # ciepło właściwe powietrza
k_ext = 15000  # współczynnik utraty ciepła

#dane dla połączenie klient
host_k = '192.168.192.100' #regulator wymiennika
port_k = 4790
host_e = '192.168.192.123' #wymiennik
port_e = 43516
host_l = '192.168.192.120' #logowanie
port_l = 55555
host_o = '192.168.192.15' #temperatura zewnętrzna
port_o = 8080
host_t = '192.168.192.253' #serwer czasu
port_t = 8123

host = '192.168.192.12'
port = 52345

#utworzenie połączeń
sock_exch = functions.SockClient(host_e, port_e)
sock_outdoor = functions.SockClient(host_o, port_o)
sock_log = functions.SockClient(host_l, port_l)
sock_con = functions.SockClient(host_k, port_k)


#połączenie z serwerem czasu
time_client = NTPClient()
t = functions.get_time(host_t, port_t, time_client)

while True:
    rec_zco = sock_exch.receive()
    T_ZCO = rec_zco['Tzco']

    resp = sock_exch & sock_outdoor

    if T_ZCO == 0:
        resp = False

    state = {'Tpco': T_PCO, 'Fcob': F_COB, "Tr": T_R, "timestamp": t}
    sock_log.sock.send(b'')
    sock_outdoor.sock.send(b'1')

    # resp = True
    while resp:
        try:
            t_prim = functions.get_time(host_t, port_t, time_client)
            sleep(1)
            dt = t_prim-t
            t = t_prim

            sock_exch.send(state)
            rec_zco = sock_exch.receive()
            T_ZCO = rec_zco['Tzco']

            sock_outdoor.sock.send(b'1')
            rec_o = sock_outdoor.receive()
            T_O = rec_o['To']

            #regulacja przepływu
            PID_Sum = PID_Sum + PID_E
            PID_E = PID_U - T_R
            PID_Y = PID_P * PID_E + PID_I * PID_Sum

            if PID_Y > F_COB_MAX:
                PID_Y = F_COB_MAX

            if PID_Y < 0:
                PID_Y = 0

            F_COB = PID_Y

            #model matematyczny kaloryfera i budynków
            T_PCO_Prim = dt * (F_COB * rho * c_W * (T_ZCO - T_PCO) - k_h * (T_PCO - T_R)) / (m_h * c_h)
            T_R_Prim = dt * (k_h * (T_PCO - T_R) - k_ext * (T_R - T_O)) / (m_b * c_b)

            T_R = T_R + T_R_Prim
            T_PCO = T_PCO + T_PCO_Prim

            #status regulacji
            state = {'Tpco': T_PCO, 'Fcob': F_COB, "Tr": T_R, "timestamp": t}
            print(state)
            server.rec = state #aktualizacja statusu

            #dane dla serwera logowania
            log1 = {"request": "PUT", "variable": "Fcob_2", "timestamp": t, "value": F_COB}
            log2 = {"request": "PUT", "variable": "Tr_2", "timestamp": t, "value": T_R}
            log3 = {"request": "PUT", "variable": "Tpco_2", "timestamp": t, "value": T_PCO}
            sock_log.send(log1)
            sock_log.sock.recv(1024)
            sock_log.send(log2)
            sock_log.sock.recv(1024)
            sock_log.send(log3)
            sock_log.sock.recv(1024)

            #dane dla regulatora wymiennika
            sock_con.send(state)

        except(NTPException):
            time_client = NTPClient()
            t_prim = functions.get_time(host_t, port_t, time_client)


