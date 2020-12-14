
from ntplib import NTPClient, NTPException
from datetime import datetime
import matplotlib.pyplot as plt
import numpy as np


# host = '127.0.0.1'
# port = 52345
# host='192.168.192.12'
# server = functions.MyServer(port, host)

PID_P = 1/2000
PID_I = 1/400000
PID_Sum = 0

PID_U = 20
PID_E = 0
PID_Y = 0

dt = 10 # czas serwera
T_O = 5  # temp otoczenia - zewnetrzne
T_ZCO = 70  # temp wody z wymiennika - zewnetrzne

T_R = 15  # temp pomieszczen
T_PCO = 0  # temp wody powrotnej
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

time_v = []
tpco_v = []
tr_v = []
fcob_v = []
pid_e = []


ntp = NTPClient()
t = ntp.request('europe.pool.ntp.org').tx_time

print(t)
dt_object = datetime.fromtimestamp(t)

print("dt_object =", dt_object)
print("type(dt_object) =", type(dt_object))
year = dt_object.strftime("%Y")
month = dt_object.strftime("%m")
day = dt_object.strftime("%d")
time = dt_object.strftime("%H")

today = datetime.date(dt_object)
weekday = today.weekday()

state = {'Tpco': T_PCO, 'Fcob': F_COB, "Tr": T_R, "timestamp": t}

n=0
dt=10
while n<60480:
# while True:
    resp = True
    while n<60480:
        n = n + 1
        t = t + 10
        dt_object = datetime.fromtimestamp(t)
        year = dt_object.strftime("%Y")
        month = dt_object.strftime("%m")
        day = dt_object.strftime("%d")
        time = dt_object.strftime("%H")
        today = datetime.date(dt_object)
        weekday = today.weekday()

        if int(time) > 7 and int(time) < 22:
            if weekday == 5 or weekday == 6:
                T_O = 2
                PID_U = 15
            else:
                PID_U = 20
        else:
            T_O = 2
            PID_U = 15

        try:

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

            print(state)
            state = {'Tpco': T_PCO, 'Fcob': F_COB, "Tr": T_R, "timestamp": t}

            tt=tt+dt
            time_v.append(tt)
            tpco_v.append(T_PCO)
            tr_v.append(T_R)
            fcob_v.append(F_COB*3600)
            pid_e.append(PID_E)

        except(NTPException):
            pass

# lebels = ['0','1', '2', '3', '4', '5', '6', '7', '8', '9', '10']
# plt.title('Uchyb dla temperatury pomieszczeń')
# plt.xticks(np.arange(11)*max(time_v)/10, lebels)
# plt.ylabel('temperatura [°C]')
# plt.xlabel('czas [h]')
# plt.grid()
# plt.plot(time_v, pid_e)
# plt.show()
# #
# plt.title('Temperatura pomieszczeń Tr')
# plt.xticks(np.arange(11)*max(time_v)/10, lebels)
# plt.ylabel('temperatura [°C]')
# plt.xlabel('czas [h]')
# plt.grid()
# plt.plot(time_v,tr_v)
# plt.show()
# #
# plt.title('Pobór wody w budynku Fcob')
# plt.xticks(np.arange(11)*max(time_v)/10, lebels)
# plt.ylabel('przepływ [m3/h]')
# plt.xlabel('czas [h]')
# plt.grid()
# plt.plot(time_v,fcob_v)
# plt.show()
# #
# plt.title('Temperatura wody powrotnej Tpco')
# plt.xticks(np.arange(11)*max(time_v)/10, lebels)
# plt.ylabel('temperatura [°C]')
# plt.xlabel('czas [h]')
# plt.grid()
# plt.plot(time_v, tpco_v)
# plt.show()



# lebels = ['Nd','Pon', 'Wt', 'Śr', 'Czw', 'Pt', 'Sb', 'Nd','Pon', 'Wt', 'Śr', 'Czw', 'Pt', 'Sb', 'Nd','Pon', 'Wt', 'Śr', 'Czw', 'Pt', 'Sb', 'Nd','Pon', 'Wt', 'Śr', 'Czw', 'Pt', 'Sb', 'Nd','Pon', 'Wt', 'Śr', 'Czw', 'Pt', 'Sb', 'Nd']
# lebels = ['Pon', 'Pt', 'Pon', 'Pt','Pon', 'Pt','Pon', 'Pt','Pon', 'Pt']
# plt.title('Uchyb dla temperatury pomieszczeń')
# plt.xticks(np.arange(36)*max(time_v)/35, lebels)
# plt.ylabel('temperatura [°C]')
# plt.xlabel('czas')
# plt.grid()
# plt.plot(time_v[1000:], pid_e[1000:])
# plt.show()
# #
# plt.title('Temperatura pomieszczeń Tr')
# plt.xticks(np.arange(36)*max(time_v)/35, lebels)
# plt.ylabel('temperatura [°C]')
# plt.xlabel('czas')
# plt.grid()
# plt.plot(time_v[1000:],tr_v[1000:])
# plt.show()
# #
# plt.title('Pobór wody w budynku Fcob')
# plt.xticks(np.arange(36)*max(time_v)/35, lebels)
# plt.ylabel('przepływ [m3/h]')
# plt.xlabel('czas')
# plt.grid()
# plt.plot(time_v[1000:],fcob_v[1000:])
# plt.show()
# #
# plt.title('Temperatura wody powrotnej Tpco')
# plt.xticks(np.arange(36)*max(time_v)/35, lebels)
# plt.ylabel('temperatura [°C]')
# plt.xlabel('czas')
# plt.grid()
# plt.plot(time_v[1000:], tpco_v[1000:])
# plt.show()



lebels = ['Nd','Pon', 'Wt', 'Śr', 'Czw', 'Pt', 'Sb', 'Nd']
plt.title('Uchyb dla temperatury pomieszczeń')
plt.xticks(np.arange(8)*max(time_v)/7, lebels)
plt.ylabel('temperatura [°C]')
plt.xlabel('czas')
plt.grid()
plt.plot(time_v[1000:], pid_e[1000:])
plt.show()
#
plt.title('Temperatura pomieszczeń Tr')
plt.xticks(np.arange(8)*max(time_v)/7, lebels)
plt.ylabel('temperatura [°C]')
plt.xlabel('czas')
plt.grid()
plt.plot(time_v[1000:],tr_v[1000:])
plt.show()
#
plt.title('Pobór wody w budynku Fcob')
plt.xticks(np.arange(8)*max(time_v)/7, lebels)
plt.ylabel('przepływ [m3/h]')
plt.xlabel('czas')
plt.grid()
plt.plot(time_v[1000:],fcob_v[1000:])
plt.show()
#
plt.title('Temperatura wody powrotnej Tpco')
plt.xticks(np.arange(8)*max(time_v)/7, lebels)
plt.ylabel('temperatura [°C]')
plt.xlabel('czas')
plt.grid()
plt.plot(time_v[1000:], tpco_v[1000:])
plt.show()
