import time
import functions
from ntplib import NTPClient

host = '192.168.192.82'
port = 50621

server = functions.MyServer(port, host)
server.start()

m_h = 3000  # kg– masy zastępcze wszystkich kaloryferów
c_h = 2700  # J/kg/K– zastępcze ciepło właściwe kaloryferów
k_h = 12000   # J/K/s– współczynnik przenikania ciepła kaloryfer-pokój;

F_COB = 0  # L/s aktualny pobór wody w czasie sekundy
F_COB_MAX = 40 / 3600  # L/h maksymalny pobór wody w czasie godziny

m_b = 20000  # kg - masa powietrza w budynku
c_b = 1000  # J/kg/K - ciepło właściwe powietrza
cw = 4200  # J/kg/K– ciepło właściwe wody
dzeta = 1000  # kg/m3 - gęstość wody
k_ext = 15000   # J/K/s – współczynnik utraty ciepła

dT_PCO = 0  # Zmiana temperatury wody powrotnej
T_PCO = 0  # Temperatura wody powrotnej

dT_R = 0  # Zmiana temperatury wewnątrz budynku
T_R = 0  # Temperatura wewnątrz budynku

T_ZCO = 80  # Temperatura wody doprowadzonej do budynku
T_O = 0  # Temperatura otoczenia

dt = 60  # Przyrost czasu

pid_U = 25
pid_E = 0
pid_Esum = 0
pid_Y = 0
pid_P = 0.001
pid_I = 0.000015



t=0
state = {'Tpco': T_PCO, 'Fcob': F_COB, "Tr": T_R, "timestamp": t}
addres_kamil = '192.168.192.100' #Kamil
port_kamil = 4790
address_michal = '192.168.192.123' #Michał
port_michal = 43516
address_przemek = '192.168.192.120' #Przemek
port_przemek = 55555
address_adam = '192.168.192.15' #Adam
port_adam = 8080
host_t = '192.168.192.253'
port_t = 8123
host = '192.168.192.12'
port = 52345
host = '127.0.0.1'
port = 52344

time_client = NTPClient()   #serwer
t = functions.get_time(host_t, port_t, time_client)
ntp = NTPClient()
#t = ntp.request('europe.pool.ntp.org').tx_time
#sock_michal = functions.SockClient(address_michal, port_michal)
#sock_przemek = functions.SockClient(address_przemek, port_przemek)
#sock_adam = functions.SockClient(address_adam, port_adam)
print("a")


last_t=t
while True:
    t = functions.get_time(host_t, port_t, time_client)
    dt = t - last_t


    dT_PCO = dt * (F_COB * dzeta * cw * (T_ZCO - T_PCO) - k_h * (T_PCO - T_R)) / (m_h * c_h)
    dT_R = dt * (k_h * (T_PCO - T_R) - k_ext * (T_R - T_O)) / (m_b * c_b)

    T_PCO = T_PCO + dT_PCO
    T_R = T_R + dT_R

    pid_E = pid_U - T_R
    pid_Esum = pid_Esum + pid_E
    pid_Y = pid_P * pid_E + pid_I * pid_Esum
    F_COB = pid_Y

    if F_COB < 0:
        F_COB = 0
    if F_COB > F_COB_MAX:
        F_COB = F_COB_MAX

    state = {'Tpco': T_PCO, 'Fcob': F_COB, "Tr": T_R, "timestamp": t}
    #print("Aktualna temperatura:{a:3.2f} Otwarcie zaworu:{b:3.2f}% Temperatura wody powrotnej:{c:3.2f}".format(a=T_R, b=100*F_COB/F_COB_MAX, c=T_PCO))
    print(state)

    #sock_michal.send(state)
    #recieve_michal= sock_michal.receive()
    #T_ZCO=recieve_michal["Tzco"]

    #sock_przemek.send(state)
    #recieve_przemek = sock_przemek.receive()
    #sock_adam.send(state)
    #recieve_adam = sock_adam.receive()
    #server.rec = state
    last_t=t
    server.rec = state
    time.sleep(1)


