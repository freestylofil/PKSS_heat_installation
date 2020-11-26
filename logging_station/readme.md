Stacja logująca napisana w języku Python.

Wymagania:
- instalacja pakietu tinydb (pip install tinydb
- interpreter Pythona w wersji 3.8

Przetestowano program:
- lokalnie: serwer i klient w sieci 127.0.0.1
- lokalnie: host <-> maszyna wirtualna, gdzie serwer znajdował się na maszynie wirtualnej

Baza danych stworzona w oparciu o pakiet **TinyDB**.
Zaimplementowane zostały trzy funkcje:
- log_into_db - zapisuje pojedyńczy rekord do bazy danych
- get_from_db - odczytuje z bazy danych wpisy  z konkretnego przedziału czasowego
- delete_db - do usunięcia bazy danych (w celach diagnostycznych)

Komunikacja z serwerem:
- protokół:     TCP/IP
- kodowanie:    utf-8
- adres IP:     192.168.192.120
- numer portu:  55555

Konfiguracja bazy danych:
- ścieżka do bazy danych: "./db.json"

----------------------------------------------------------------------------------------------------------------

Specyfikacja funkcji log_into_db:

     {"request": "PUT", "variable": str, "timestamp": numeric, "value": numeric}

 Przyjmuje format json. Wymagane pola:
 - "request": "PUT" - informacja dla serwera, że chcemy zapisać wpis w bazie danych
 - "variable": str - nazwa zmiennej procesu, musi być unikalna dla różnych zmiennych
 - "timestamp": str - czas, w którym zmierzono wartość procesu
 - "value": int - wartość procesu
 

 
 Odpowiedź serwera:
 
 "OK" - w przypadku poprawnego zapytania
 
 "[Error] Bad request (<message>)" - w przypadku błędnego zapytania, gdzie <message> to komunikat błędu
 
 ----------------------------------------------------------------------------------------------------------------
 
Specyfikacja funkcji get_from_db:

    {"request": "GET", "variable": str, "time_from": numeric, "time_to": numeric}

 Przyjmuje format json. Wymagane pola:
 - "request": "GET" - informacja dla serwera, że chcemy odczytać dane z bazy danych
 - "variable": str - nazwa zmiennej procesu, musi być unikalna dla różnych zmiennych
 Opcjonalne pola:
 - "time_from": numeric - informuje serwer, od której chwili interesują nas logi, jeśli w bazie danych nie istnieje wpis o wartości identycznej wartość ta zostanie ustalona jako pierwsza w bazie danych
 - "time_to": numeric - informuje serwer, do której chwili interesują nas logi, jeśli w bazie danych nie istnieje wpis o wartości identycznej wartość ta zostanie ustalona jako ostatnia w bazie danych
 
 Odpowiedź serwera:
 
 json - w przypadku poprawnego zapytania
 Struktura jsona wygląda następująco:

     {
     "1": {"timestamp": "...", "value": ...},
     "2": {"timestamp": "...", "value": ...},
     "3": {...},
     ...
     }

 "[Error] Bad request (<message>)" - w przypadku błędnego zapytania, gdzie <message> to komunikat błędu
 
  ----------------------------------------------------------------------------------------------------------------
 
Specyfikacja funkcji delete_db:

Umożliwia zdalne usunięcie bazy danych. Nie używać w trakcie symulacji.

