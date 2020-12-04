import socket
import sys
import keyboard
import json
from time import sleep
from random import randrange
from itertools import count
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

HOST = '192.168.192.120'
PORT = 55555


# HOST = "localhost"
# PORT = 55555

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, PORT))

Tab_val = [[],[],[],[],[],[],[],[]] # Tzwu Tpwu Tzco Tpco
Tab_time = [[],[],[],[],[],[],[],[]]




def pobier_dane(var_numm, od_kad, do_kad):
    liczbalewych = 0
    liczbaprawych = 0
    jaka_zmienna = "Tpco_2"
    if var_numm == 0:
        jaka_zmienna = "Tpco_1"
    if var_numm == 1:
        jaka_zmienna = "Fcob_1"
    if var_numm == 2:
        jaka_zmienna = "Tr_1"
    # if var_numm == 3:
    #     jaka_zmienna = "aaa"
    # if var_numm == 4:
    #     jaka_zmienna = "aaaa"
    # if var_numm == 5:
    #     jaka_zmienna = "aaa"
    # if var_numm == 6:
    #     jaka_zmienna = "aaa"
    # if var_numm == 7:
    #     jaka_zmienna = "aaa"
    # request = b"{\"request\":\"GET\",\"variable\":\"" + jaka_zmienna.encode() + b"\",\"time_from\":\"" + bytes(0) + b"\",\"time_to\":\"" + bytes(5) + b"\"}"
    request = b"{\"request\":\"GET\",\"variable\":\"" + jaka_zmienna.encode() + b"\",\"time_from\":" + str(od_kad).encode() + b",\"time_to\":" + str(do_kad).encode() + b"}"
    if(od_kad == 0):
        request = b"{\"request\":\"GET\",\"variable\":\""+jaka_zmienna.encode() +b"\"}"
    # print(jaka_zmienna)
    s.send(request)
    data = b''
    while True:
        data = data + s.recv(4096)
        liczba_lewych = int(data.decode("utf-8").count("{"))
        liczba_prawych = int(data.decode("utf-8").count("}"))
        # print(liczba_lewych)
        if(liczba_lewych == liczba_prawych):
            break
    print(jaka_zmienna)
    print(data[0:100])
    return data


licznik = 1

def zapisz_dane(var_num):
    global licznik
    # dane = "{\"1\": {\"timestamp\": " + str(licznik) + ", \"value\": " + str(
    #     randrange(10)) + "}, \"2\": {\"timestamp\": " + str(licznik + 1) + ", \"value\": " + str(
    #     randrange(10)) + "}, \"3\": {\"timestamp\": " + str(licznik + 2) + ", \"value\": " + str(
    #     randrange(10)) + "}, \"4\": {\"timestamp\": " + str(licznik + 3) + ", \"value\": " + str(
    #     randrange(10)) + "}, \"5\": {\"timestamp\": " + str(licznik + 4) + ", \"value\": " + str(randrange(10)) + "}}"

    if Tab_val[var_num] != []:
        dane = pobier_dane(var_num, Tab_time[var_num].index(Tab_time[var_num][-1]), Tab_time[var_num].index(Tab_time[var_num][-1]) + 5).decode("utf-8")
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
        if len(dane)<3:
            return -1
        json_pomoc = json.loads(dane)
        if Tab_val[var_num] == []:
            Tab_val[var_num].append(float(json_pomoc["1"]["value"]))
            # Tab_time[var_num].append(licznik)
            Tab_time[var_num].append(int(json_pomoc["1"]["timestamp"]))
        for i in json_pomoc:
            if int(json_pomoc[i]["timestamp"]) > int(Tab_time[var_num][-1]):
                Tab_time[var_num].append(int(json_pomoc[i]["timestamp"]))
                # Tab_time[var_num].append(licznik)
                Tab_val[var_num].append(float(json_pomoc[i]["value"]))
            # licznik = licznik +1
        while len(Tab_time[var_num])>5000:
            Tab_time[var_num].pop(0)
            Tab_val[var_num].pop(0)

def animate(i):
    global licznik
    global Tab_time
    global Tab_val
    # Tab_val = [[], [], [], [],[], [], [], []]
    # Tab_time = [[], [], [], [],[], [], [], []]
    for i in range(len(Tab_time)):
        zapisz_dane(i)
    licznik = 1
    if keyboard.is_pressed('Esc'):
        sys.exit(0)

    plt.subplot(4, 2, 1)
    plt.cla()
    plt.plot(Tab_time[0],Tab_val[0], 'r', label='Tpco_1')
    plt.legend()
    plt.subplot(4, 2, 3)
    plt.cla()
    plt.plot(Tab_time[1],Tab_val[1], 'b', label='Fcob_1')
    plt.legend()
    plt.subplot(4, 2, 5)
    plt.cla()
    plt.plot(Tab_time[2],Tab_val[2], 'g', label='Tr_1')
    plt.legend()
    plt.subplot(4, 2, 7)
    plt.cla()
    plt.plot(Tab_time[3],Tab_val[3], 'm', label='Tpco')
    plt.legend()



    plt.subplot(4, 2, 2)
    plt.cla()
    plt.plot(Tab_time[4],Tab_val[4], 'r', label='Tpco_2')
    plt.legend()
    plt.subplot(4, 2, 4)
    plt.cla()
    plt.plot(Tab_time[5],Tab_val[5], 'b', label='Fcob_2')
    plt.legend()
    plt.subplot(4,2,6)
    plt.cla()
    plt.plot(Tab_time[6],Tab_val[6], 'g', label='Tr_2')
    plt.legend()
    plt.subplot(4, 2, 8)
    plt.cla()
    plt.plot(Tab_time[7],Tab_val[7], 'm', label='Tpco_2')
    plt.legend()


# sleep(1)
ani = FuncAnimation(plt.gcf(), animate, interval=5000)
plt.tight_layout()
plt.show()