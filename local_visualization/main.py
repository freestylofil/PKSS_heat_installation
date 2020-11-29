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


# HOST = '192.168.4.1'
# PORT = 80

Tab_val = [[],[],[],[]] # Tzwu Tpwu Tzco Tpco
Tab_time = [[],[],[],[]]


def pobier_dane(var_numm, od_kad, do_kad):
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((HOST, PORT))
        if var_numm == 0:
            jaka_zmienna = "Tzwu"
        if var_numm == 1:
            jaka_zmienna = "Tpwu"
        if var_numm == 2:
            jaka_zmienna = "Tzco"
        if var_numm == 3:
            jaka_zmienna = "Tpco"
        request = b"{\"request\":\"GET\",\"variable\":\""+jaka_zmienna+"\",\"time_from\":"+ str(od_kad) + ",\"time_to\":" + str(do_kad) +"\"}"
        # request = b"GET /abc1 HTTP/1.1\n\n\n"
        s.send(request)
        data = s.recv(1024)
        return data


licznik = 1

def zapisz_dane(var_num):
    # global licznik
    # dane = "{\"1\": {\"timestamp\": " + str(licznik) + ", \"value\": " + str(
    #     randrange(10)) + "}, \"2\": {\"timestamp\": " + str(licznik + 1) + ", \"value\": " + str(
    #     randrange(10)) + "}, \"3\": {\"timestamp\": " + str(licznik + 2) + ", \"value\": " + str(
    #     randrange(10)) + "}, \"4\": {\"timestamp\": " + str(licznik + 3) + ", \"value\": " + str(
    #     randrange(10)) + "}, \"5\": {\"timestamp\": " + str(licznik + 4) + ", \"value\": " + str(randrange(10)) + "}}"

    if Tab_val[var_num] != []:
        dane = pobier_dane(var_num, Tab_time[var_num].index(Tab_time[var_num][-1]), Tab_time[var_num].index(Tab_time[var_num][-1] + 10)).decode("utf-8")
    else:
        dane = pobier_dane(var_num, 0, 5).decode("utf-8")
    try:
        dane.index("[Error] Bad request ()")
        print("[Error] Bad request ()")
    except ValueError:
        try:
            dane = dane[dane.index("{"):]
        except ValueError:
            return -1
        json_pomoc = json.loads(dane)
        if Tab_val[var_num] == []:
            Tab_val[var_num].append(int(json_pomoc["1"]["value"])+var_num)
            Tab_time[var_num].append(int(json_pomoc["1"]["timestamp"]))
        for i in json_pomoc:
            if int(json_pomoc[i]["timestamp"]) > int(Tab_time[var_num][-1]):
                Tab_time[var_num].append(int(json_pomoc[i]["timestamp"]))
                Tab_val[var_num].append(int(json_pomoc[i]["value"])+var_num)
        while len(Tab_time[var_num])>500:
            Tab_time[var_num].pop(0)
            Tab_val[var_num].pop(0)

def animate(i):
    # global licznik
    for i in range(4):
        zapisz_dane(i)
    # licznik = licznik + 5
    if keyboard.is_pressed('Esc'):
        sys.exit(0)


    plt.subplot(2,1,1)
    plt.cla()
    plt.plot(Tab_time[0],Tab_val[0], 'r', label='Tzwu')
    plt.plot(Tab_time[1],Tab_val[1], 'b', label='Tpwu')
    plt.legend()
    plt.subplot(2,1,2)
    plt.cla()
    plt.plot(Tab_time[2],Tab_val[2], 'g', label='Tzco')
    plt.plot(Tab_time[3],Tab_val[3], 'm', label='Tpco')
    plt.legend()


# sleep(1)
ani = FuncAnimation(plt.gcf(), animate, interval=5000)
plt.tight_layout()
plt.show()