
/**
* \~Polish @page vfs Wirtualny System Plików (VFS)
* @tableofcontents
* @section vfs_intro Wprowadzenie
* Pixie Engine (jak wiele innych silników) stosuje wirtualny system plików (VFS), czyli warstwę abstrakcji ponad fizycznym
* systemem plików. Niniejszy rozdział opisuje wszystkie cechy VFS zaimplementowanego w tym silniku.
* 
* VFS Pixie Engine opiera się nie tylko na wczytywaniu danych, ale pozwala na swobodne modyfikowanie zasobów
* na przykład do wykorzystania przez edytor. Zasoby są przeładowywane dynamicznie w trakcie działania silnika
* i mogą zostać zaktualizowane przez dowolną aplikację Pixie Engine bądź fizycznie przez użytkownika poza silnikiem.
*
* @section vfs_vpath Wirtualna ścieżka
* Wirtualna ścieżka (vpath) reprezentuje nazwę zasobu wewnątrz VFS. Vpath składa się z dwóch elementów:
* typu i adresu. Typ reprezentuje miejsce poszukiwania fizycznego pliku,
* podczas gdy adres przybliża nazwę tego pliku.
* 
* W praktyce vpath może opisywać więcej niż jeden fizyczny plik, lub nawet brak pliku. Jeżeli vpath odpowiada więcej
* niż jednemu plikowi próby tworzenia akcesora pliku wykorzystają zawsze najnowszy, spełniający warunki zasób.
* Dla wczytywania wystarczy warunek czytalności, podczas gdy do zapisu wymaganiem jest zarówno czytalność jak
* i zapisywalność. Jeżeli żaden plik nie spełnia warunku zapisywalności (np. istnieje tylko jedna kopia ulokowana
* na płycie CD), przy próbie tworzenia akcesora pliku zostanie stworzony nowy zasób w lokacji określonej przez platformę.
* 
* Spostrzegawczy użytkownik zauważy, że dane czytane i zapisywane mogą lądować w dwóch różnych plikach fizycznych.
* W praktyce jednak po zapisie zaktualizowana zostanie data ostatniej modyfikacji i tym samym ten plik przeskoczy
* na początek kolejki czytania (stąd wymóg czytalności od takiego pliku)
*
* @section vfs_input_output Wprowadzenie/Wyprowadzenie
* Wprowadzene i wyprowadzenie to rodzina klas dostarczająca rodzajowy sposób czytania lub zapisywania danych.
* Z perspektywy użytkownika nie ma znaczenia co znajduje się w konkretnej implementacji, polega się wyłącznie
* na interfejsie. Popularne implementacje obejmują: plik, strumień sieciowy, FTP, archiwum ZIP, etc.
* 
* Nie istnieje wymóg żeby każde wprowadzenie danych miało odpowiednik wyprowadzenia (np. czytanie danych
* po HTTP nie wymaga zapisu). Silnik ściśle wymaga natomiast żeby cały ruch danych był prowadzony przez
* te klasy ze względów bezpieczeństwa. Jedynym wyjątkiem jest wyjście pliku dla dziennika, ponieważ musi
* być w stanie zanotować błędy krytyczne związane z samym systemem wyprowadzania danych.
* 
* Silnik dostarcza także opakowanie dla strumieni - klasy 
* @c engine::data::input_streambuf_t dla wprowadzania i @c engine::data::output_streambuf_t
*  dla wyprowadzenia. Obie bazują na `std::streambuf` i pozwalają na tworzenie własnych strumieni C++
* 
* Istnieje także opcja stworzenia 'częściowego' wprowadzania. Jeżeli wiadomo, że źródłowy plik zawiera
* więcej niż jeden zasób (np. kilka różnych poziomów detali tekstury), można rozkazać obiektowi wprowadzającemu
* żeby przeczytał dane o określonej długości do bufora, a następnie użył go do stworzenia nowy obiekt wprowadzający.
* Źródłowy strumień pozostanie nie zmodyfikowany, a tak stworzony obiekt wprowadzający operuje na kopii, więc może
* przeżyć oryginalny obiekt wprowadzający. Utworzony obiekt ma dostęp tylko do tej porcji danych która była
* zażądana przy jego tworzeniu.
*
* @section vfs_providers Dostawcy
* Dostawca to klasa opisująca sposób tworzenia obiektów wprowadzających/wyprowadzających dla danego zasobu. Zwyczajowy dostawcy
* są zbierani i używani przez wewnętrzne komponenty silnika i nie ma powodu manipulacja przy nich z poziomu aplikacji. 
* 
* Standardowo każdy dostawca właściwy posiada możliwość podania ostatniej daty modyfikacji zasobu.
* Ostatecznie klasa zbiorcza dostawy zawsze wybierze najnowszego dostawcę (do zapisu dostawca musi
* wskazywać na zasób zdolny do zarówno zapisu jak i odczytu).
* 
* Jeżeli żaden dostawca nie spełnia takiego warunku, nowy zostanie stworzony za pomocą funkcji @c engine::platform::create_new
* 
* Jeżeli dany zasób jest niemożliwy do zapisania, można zwrócić obiekt @c engine::data::output_void_t który symuluje zapis,
* wysyłając dane w nicość (analogicznie do Linuxowego pliku `/dev/null`)
*
* @section vfs_scanners Skanery
* Skaner to niewielka klasa przeglądająca zasoby w regularnych odstępach czasowych (rzędu setek milisekund) aby sprawdzić
* ich datę modyfikacji. Zmiany zostają odnotowane i zaktualizowane w formie nowych dostawców. Lista dostępnych skanerów
* jest udostępniona jako ustawienia rozrusznika i zwykle zawiera kilka
* skanerów katalogów ze ścieżkami w jakich można się spodziewać zasobów.
*
* @section vfs_extension Rozszerzenia
* Skanery (opisane w poprzednim akapicie) posiadają także możliwość zarejestrowania wywołania zwrotnego po każdym znalezionym pliku.
* W obecnej wersji Pixie Engine istnieje jedno takie zaimplementowane rozszerzenie: skaner archiwum. Jeżeli skaner wykryje plik
* archiwum (np. zip) uruchomi kolejny skaner, operujący całkowicie na archiwum i czytający pliki w środku. Oczywiście jest to
* wciąż normalny skaner - w znaczeniu wciąż wykonujący zapytania zwrotne - i z tego powodu rekurencyjne archiwa (archiwum w archiwum) są jak najbardziej
* wspierane (ale niezalecane, jako że aby skaner mógł dostać się do wewnętrznego archiwum, musi je odpakować do pamięci).
* Skaner archiwum wygeneruje dostawców w formie archiwalnym które są w stanie stworzyć obiekt wprowadzania/wyprowadzania archiwalnego.
* 
* W formacie ZIP istnieje pewne zagrożenie które trzeba było uwzględnić w implementacji; O ile prawie wszyscy dostawcy korzystają z daty
* modyfikacji z dokładnością do jednej sekundy, archiwum zipa operuje na dacie modyfikacji pliku z dokładnością do dwóch sekund.
* Założeniem twórcy było dwukrotne zwiększenie zakresu czasowego 32-bitowej liczby bez znaku (patrz: problem roku 2038@cite year2038Problem),
* niestety w efekcie wpływa to na stabilność dostawcy i jego kolejki priorytetowej uwzględniającej datę zapisu. Dla pewności
* wszystkie operacje czasowe na zip są zaokrąglane w górę (co wymusza położenie archiwum z przodu kolejki, dla jednoczesnego zapisu z i bez archiwum)
* 
* @section vfs_example Przykład działania vfs
* Zasób na którym będziemy operować w niniejszym przykładzie ma wirtualną ścieżkę `script/orc_ai.code` o typie `game_asset`.
* 
* 1. Gracz wkłada płytę z grą do napędu. Dla oszczędności miejsca na dysku twardym zasób ten zostanie rozwiązany do lokacji na płycie (tj. `D:/script/orc_ai.code`)
* 2. Niestety, twórca odkrył iż popełnił błąd w skrypcie. Dystrybuuje zatem poprawkę zawierającą spakowany skrypt. Po jej instalacji istnieją już dwie lokacje zasobu, a gra wybiera tę nowszą (znajdującą się w `C:/Program Files/Pixie/patch.zip` ścieżka w archiwum `script/orc_ai.code`)
* 3. Gracz stwierdza iż nie podoba mu się zachowanie orków. Nie wyłączając gry, otwiera skrypt edytorem i wprowadza poprawki. Ponieważ zapis do `Program Files` wymaga uprawnień administracyjnych, gra tworzy trzecią kopię skryptu w lokacji `C:/Users/Piotr/AppData/Local/Pixie/script/orc_ai.code`
* 4. Uruchomiona w między czasie gra wykrywa istnienie nowego skryptu o nowszej dacie niż zapamiętana. Przeładowuje więc skrypt i aktualizuje referencje do niego bez przerywania rozgrywki
* 
* @section vfs_hotreload_problems Problemy z przeładowaniem zasobów na gorąco
* Jak widać po powyższym przykładzie pomimo zalet takiego rozwiązania, natychmiastowej informacji zwrotnej dla twórców modyfikacji,
* istnieje poważny problem z przeładowaniem zasobów na gorąco: gracz może podmienić dowolny zasób w trakcie działania programu.
* W szczególności może to wygenerować następujące problemy:
* 
* - Gracz może oszukiwać w grze wieloosobowej
* - Gracz może oszukiwać w osiągnięciach
* 
* Obie sytuacje wymagają innego podejścia do ich rozwiązania. 
* 
* Pierwszym krokiem jest podzielenie zasobów na kosmetyczne i kluczowe. I tak kosmetyczna jest np. grafika postaci (zmiana grafiki
* nie wpłynie znacząco na rozgrywkę), podczas gdy kluczowy jest np. rozkład poziomu (zmiana znacząco ułatwi bądź utrudni wykonanie
* zadania jakie stoi przed graczem). Podział taki należy częściowo do finałowej gry.
* 
* Aby uniknąć oszukiwania w grze wieloosobowej wprowadzono ideę odczepianych zasobów. Odczepiony zasób tworzy kopię zasobu w pamięci,
* kopię która przestaje śledzić fizyczny plik w systemie plików. W ten sposób serwer może przetransmitować zasób do klienta i mieć
* pewność że wszyscy klienci używają dokładnie tej samej wersji i danych zasobu. W takiej sytuacji zmiana pliku na dysku nie wpłynie
* na rozgrywkę do jej przerwania (i kontynuowania w trybie dla pojedynczego gracza)
* 
* Drugim problemem jest oszukiwanie w grze jednoosobowej, a więc w takiej która nie posiada autorytarnego serwera. Rozwiązaniem było 
* wprowadzenie opcji podpisywania zasobów. Podpis sam w sobie jest także zasobem, o nazwie identycznej z nazwą zasobu, ale dodatkowym rozszerzeniem.
* W ten sposób gra może szybko sprawdzić czy zasób pochodzi od twórcy oryginalnego czy też był modyfikowany przez gracza. W tym drugim
* przypadku gra może podjąć decyzję o wyłączeniu systemu osiągnięć, by nie generować możliwości oszukiwania.
* 
* @section vfs_assets System modyfikacji
* @subsection vfs_assets_intro Wprowadzenie
* Ostatnim elementem VFS jest system modyfikacji gry (w jego skład wchodzą zarówno modyfikacje fanowskie jak i dodatki udostępniane przez twórców).
* Stanowi on prostą warstwę abstrakcji ponad samym VFS i jest używany niemal wyłącznie przez samą grę (jako komponent całkowicie opcjonalny).
* 
* System modyfikacji opiera się o dwa typy modyfikacji gry:
*
* - Modyfikacje właściwe - zawierające duże rozdziały, bądź fanowskie modyfikacje pełnej konwersji
* - Podmodyfikacje - zawierające niewielkie elementy gry, bądź niewielkie fanowskie modyfikacje
* 
* @subsection vfs_assets_manifest Manifest
* Każda z modyfikacji zawiera manifest (w formacie XML) opisujący w jakich warunkach może być włączona. W ten sposób twórca jest
* w stanie łatwo dodać informację o niekompatybilnych dodatkach bezpośrednio do modyfikacji (użytkownik zostanie o tym problemie
* ostrzeżony w trakcie uruchamiania bądź ściągania modyfikacji). Zmiana układu modyfikacji nie wymaga restartu gry. Jeżeli problem 
* podczas uruchamiania modyfikacji jest możliwy do rozwiązania przez silnik, użytkownik zostanie o nim poinformowany,
* a następnie - po jego zgodzie - zostanie podjęta próba naprawienia. Przykład: podmodyfikacja "Wątek smoka na bagnach"
* wymaga modyfikacji "Bagna północne", która nie jest w danej chwili włączona; Gra automatycznie zaproponuje jej włączenie.
*/