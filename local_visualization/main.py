import socket
import sys
import keyboard
import json
from time import sleep
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

HOST = '192.168.192.120'
PORT = 55555

# HOST = "localhost"
# PORT = 55555

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, PORT))

Tab_val = [[], [], [], [], [], [], [], [], [], [], [], []]  # Tpco, Fcob tr
Tab_time = [[], [], [], [], [], [], [], [], [], [], [], []]


def pobier_dane(var_numm, od_kad, do_kad):
    liczbalewych = 0
    liczbaprawych = 0
    jaka_zmienna = "Tpco_1"
    if var_numm == 0:
        jaka_zmienna = "Tpco_1"
    if var_numm == 1:
        jaka_zmienna = "Fcob_1"
    if var_numm == 2:
        jaka_zmienna = "Tr_1"
    if var_numm == 3:
        jaka_zmienna = "Tpco_2"
    if var_numm == 4:
        jaka_zmienna = "Fcob_2"
    if var_numm == 5:
        jaka_zmienna = "Tr_2"
    if var_numm == 6:
        jaka_zmienna = "Tpco_3"
    if var_numm == 7:
        jaka_zmienna = "Fcob_3"
    if var_numm == 8:
        jaka_zmienna = "Tr_3"
    if var_numm == 9:
        jaka_zmienna = "Tpco_4"
    if var_numm == 10:
        jaka_zmienna = "Fcob_4"
    if var_numm == 11:
        jaka_zmienna = "Tr_4"
    request = b"{\"request\":\"GET\",\"variable\":\"" + jaka_zmienna.encode() + b"\",\"time_from\":" + str(od_kad).encode() + b",\"time_to\":" + str(do_kad).encode() + b"}"
    if (od_kad == 0):
        request = b"{\"request\":\"GET\",\"variable\":\"" + jaka_zmienna.encode() + b"\"}"
    s.send(request)
    data = b''
    
    while True:
        data = data + s.recv(4096)
        liczba_lewych = int(data.decode("utf-8").count("{"))
        liczba_prawych = int(data.decode("utf-8").count("}"))
        if (liczba_lewych == liczba_prawych):
            break
    return data


def zapisz_dane(var_num):
    if Tab_val[var_num]:
        dane = pobier_dane(var_num, Tab_time[var_num].index(Tab_time[var_num][-1]),Tab_time[var_num].index(Tab_time[var_num][-1]) + 5).decode("utf-8")
    else:
        dane = pobier_dane(var_num, 0, 1).decode("utf-8")
    try:
        dane.index("[Error]")
        print(dane)
    except ValueError:
        try:
            dane = dane[dane.index("{"):]
        except ValueError:
            return -1
        if len(dane) < 3:
            return -1
        json_pomoc = json.loads(dane)
        if not Tab_val[var_num]:
            Tab_val[var_num].append(float(json_pomoc["1"]["value"]))
            Tab_time[var_num].append(int(json_pomoc["1"]["timestamp"]))
        for i in json_pomoc:
            if int(json_pomoc[i]["timestamp"]) > int(Tab_time[var_num][-1]):
                Tab_time[var_num].append(int(json_pomoc[i]["timestamp"]))
                Tab_val[var_num].append(float(json_pomoc[i]["value"]))
        while len(Tab_time[var_num]) > 1000:
            Tab_time[var_num].pop(0)
            Tab_val[var_num].pop(0)


def animate(x):
    for i in range(len(Tab_time)):
        # zapisz_dane(i)
        print(i)
    if keyboard.is_pressed('Esc'):
        sys.exit(0)

    plt.subplot(3, 1, 1)
    plt.cla()
    plt.plot(Tab_time[0], Tab_val[0], 'r', label='Tpco_1')
    plt.plot(Tab_time[3], Tab_val[3], 'g', label='Tpco_2')
    plt.plot(Tab_time[6], Tab_val[6], 'b', label='Tpco_3')
    plt.plot(Tab_time[9], Tab_val[9], 'm', label='Tpco_4')
    plt.legend()
    plt.subplot(3, 1, 2)
    plt.cla()
    plt.plot(Tab_time[1], Tab_val[1], 'r', label='Fcob_1')
    plt.plot(Tab_time[4], Tab_val[4], 'g', label='Fcob_2')
    plt.plot(Tab_time[7], Tab_val[7], 'b', label='Fcob_3')
    plt.plot(Tab_time[10], Tab_val[10], 'm', label='Fcob_4')
    plt.legend()
    plt.subplot(3, 1, 3)
    plt.cla()
    plt.plot(Tab_time[2], Tab_val[2], 'r', label='Tr_1')
    plt.plot(Tab_time[5], Tab_val[5], 'g', label='Tr_2')
    plt.plot(Tab_time[8], Tab_val[8], 'b', label='Tr_3')
    plt.plot(Tab_time[11], Tab_val[11], 'm', label='Tr_4')
    plt.legend()



ani = FuncAnimation(plt.gcf(), animate, interval=5000)
plt.tight_layout()
plt.show()