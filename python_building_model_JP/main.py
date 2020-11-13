import time


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

for i in range(1000):

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

    print("Aktualna temperatura:{a:3.2f} Otwarcie zaworu:{b:3.2f}% Temperatura wody powrotnej:{c:3.2f}".format(a=T_R, b=100*F_COB/F_COB_MAX, c=T_PCO))
    time.sleep(1)
