# Moduł wizualizacji http stanu symulowanej sieci ciepłowniczej

Moduł ten jest odpowiedzialny za utrzymanie serwera http umożliwiającego dostęp do wizualizacji stanu obiektów składających
się na symulowaną sieć ciepłowniczą z poziomu wybranej przeglądarki. Moduł umożliwia przeglądanie wykresów różnych wartości charakteryzujących poszczególne komponenty sieci w czasie niemal rzeczywistym.

Moduł ten składa się z dwóch części:
* Frontendu
Za generowanie odpowiednich wykresów i wyświetlanie ich w przeglądarce odpowiedzialny jest wyspecyfikowany moduł zawarty wewnątz katalogu "visualization". Moduł ten został zbudowany w oparciu o bibliotekę React oraz framework Node.js.
* Backend
Zadaniem backendu jest z kolei łączenie się ze stacją logowania danych, pobieranie stamtąd stanu obiektów oraz rozgłaśnianie ich w czasie rzeczywistym do wszystkich instancji frontendu (okien przeglądarki). Zaimplementowany został w oparciu o framework flask oraz serwer biblioteki gevent. Do komunikacji pomiędzy backendem i frontendem wykorzystana została technologia webSocket (zwykłe sockety nie są obsługiwane przez współczesne przeglądarki) zaimplementowana w bibliotece socket.IO.

## Dokumentacja

### 1. Frontend
Cała funkcjonalność frontendowej części aplikacji zawarta jest wewnątrz katalogu "visualization".
Aby odtworzyć strukturę tego katalogu najlepiej wykorzystać funkcjonalność create-react-app frameworka node.js:
```javascript
npx create-react-app visualization
```
Aby skorzystać z tego rozwiązania należy na swoim systemie zainstalować Node >= 8.10 oraz npm >= 5.6.
Do działania nasza aplikacja potrzebuje zainstalowanych kilku dodatkowych bibliotek:
* @material-ui/core
* bootstrap
* socket.io-client
* react-bootstrap
* react-router-dom
* CanvasJSReact

Po ich instalacji możemy przejść do dodania plików z tego repozytorium do naszego utworzonego projektu.
Należy dodać / podmienić następujące pliki / katalogi:
* z katalogu public:
* index.html
* z katalogu src:
* cały folder components
* cały folder lib
* App.js
* mystyle.css

Aplikacja powinna zacząć działaćpo wywołaniu:
```
npm start
```

#### Znaczenie plików.
* public/index.html
Główny plik aplikacji. Zawarte są w nim informacje na temat niektórych zainstalowanych bibliotek, oraz wywoływany jest podstawowy komponent aplikacji.
* src/lib
W folderze tym zawarte są skrypty wykorzystywanych bibliotek.
* src/App.js
Główny komponent aplikacji. Wywołuje on i steruje zachowaniem pozostałych komponentów, takich jak wykresy czy moduł połączenia z serwerem.
* src/mystyle.css
Plik zawierający style css, służące do zmiany wyglądu wykorzystywanych komponentów.
* src/components/NavigationBar.js
Komponent poziomego paska u góry strony. Nie posiada on żadnej funkcjonalności (choć można taką dodać), służy temu aby strona nie wyglądała jak jednolita bryła.
* src/components/Sidebar.js
Podobnie jak NavigationBar jest to tylko komponent wizualny. Jest to czarny pasek znajdujący się po lewej stronie ekranu.
* src/components/socketClient.js
Moduł odpowiedzialny za nawiązanie i utrzymanie połączenia z serwerem. Przekazuje on dane uzyskane od serwera do głównego komponentu aplikacji. Implementuje on przycisk umożliwiający wstrzymanie / rozpoczęcie procesu zbierania danych.
* src/components/SPlot.js
Moduł odpowiedzialny za generowanie wykresów. Renderuje on wykres na podstawie uzyskanych danych oraz nazw osi. Jest zoptymalizowany do ciągłego aktualizowania stanu wykresu.
* src/components/SinglePlot.js
Archiwalny moduł generacji wykresów, nie jest używany w projekcie. Nie został usunięty ze względu na fakt, że może się przydać podczas dalszych prac, powinien zostać usunięty w końcowej wersji aplikacji.


### 2. Backend
Moduł backendu jest znacznie prostszy od frontendu. Cały zawiera się w jednym skrypcie języka python w katalogu backend.
Jest to skrypt backend/app.py. Do działania wymaga on następującego oprogramownia:
* Python w wersji => 3.7.4
* flask
* flask_cors
* flask_socketio
* threading
* gevent

Aplikacja implementuje klasę dziedziczącą po klasie Thread, co daje jej możliwość bycia uruchomioną w tle. Klasa ta zarządza połączeniem pomiędzy frontendem i backendem, a jednocześnie jest odpowiedzialna za pobieranie danych od stacji logującej.
Powstanie pierwszego połączenia klienta z serwerem prowadzi do utworzenia instancji tej klasy, która rozgłasza dane do wszystkich instancji frontendu podpiętych do utworzonego socketa. Może istnieć wyłącznie jedna instancja tej klasy.
W głównym wątku aplikacji działa serwer gevent'a.
