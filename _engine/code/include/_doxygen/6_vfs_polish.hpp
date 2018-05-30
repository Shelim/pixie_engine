
/**
* \~Polish @page vfs Wirtualny System Plików (VFS)
* @tableofcontents
* @section vfs_intro Wprowadzenie
* Pixie Engine (jak wiele innych silników) stosuje wirtualny system plików (VFS), czyli warstwę abstrakcji ponad fizycznym
* systemem plików. Niniejszy rozdział opisuje wszystkie cechy VFS zaimplementowanego w&nbsp;tym silniku.
* 
* VFS Pixie Engine opiera się nie tylko na&nbsp;wczytywaniu danych, ale pozwala na&nbsp;swobodne modyfikowanie zasobów
* na&nbsp;przykład do&nbsp;wykorzystania przez edytor. Zasoby są przeładowywane dynamicznie w&nbsp;trakcie działania silnika
* i&nbsp;mogą zostać zaktualizowane przez dowolną aplikację Pixie Engine bądź fizycznie przez użytkownika poza silnikiem.
*
* @section vfs_vpath Wirtualna ścieżka
* Wirtualna ścieżka (vpath) reprezentuje nazwę zasobu wewnątrz VFS. Vpath składa się z&nbsp;dwóch elementów:
* typu i&nbsp;adresu. Typ reprezentuje miejsce poszukiwania fizycznego pliku,
* podczas gdy adres przybliża nazwę tego pliku.
* 
* W&nbsp;praktyce vpath może opisywać więcej niż jeden fizyczny plik, lub nawet brak pliku. Jeżeli vpath odpowiada więcej
* niż jednemu plikowi próby tworzenia akcesora pliku wykorzystają zawsze najnowszy, spełniający warunki zasób.
* Dla wczytywania wystarczy warunek czytalności, podczas gdy do&nbsp;zapisu wymaganiem jest zarówno czytalność jak
* i&nbsp;zapisywalność. Jeżeli żaden plik nie spełnia warunku zapisywalności (np. istnieje tylko jedna kopia ulokowana
* na&nbsp;płycie CD), przy próbie tworzenia akcesora pliku zostanie stworzony nowy zasób w&nbsp;lokacji określonej przez platformę.
* 
* Spostrzegawczy użytkownik zauważy, że&nbsp;dane czytane i&nbsp;zapisywane mogą lądować w&nbsp;dwóch różnych plikach fizycznych.
* W&nbsp;praktyce jednak po&nbsp;zapisie zaktualizowana zostanie data ostatniej modyfikacji i&nbsp;tym samym ten plik przeskoczy
* na&nbsp;początek kolejki czytania (stąd wymóg czytalności od&nbsp;takiego pliku)
*
* @section vfs_input_output Wprowadzenie/Wyprowadzenie
* Wprowadzene i&nbsp;wyprowadzenie to rodzina klas dostarczająca rodzajowy sposób czytania lub zapisywania danych.
* Z&nbsp;perspektywy użytkownika nie ma znaczenia co znajduje się w&nbsp;konkretnej implementacji, polega się wyłącznie
* na&nbsp;interfejsie. Popularne implementacje obejmują: plik, strumień sieciowy, FTP, archiwum ZIP, etc.
* 
* Nie istnieje wymóg żeby każde wprowadzenie danych miało odpowiednik wyprowadzenia (np. czytanie danych
* po&nbsp;HTTP nie wymaga zapisu). Silnik ściśle wymaga natomiast żeby cały ruch danych był prowadzony przez
* te klasy ze&nbsp;względów bezpieczeństwa. Jedynym wyjątkiem jest wyjście pliku dla dziennika, ponieważ musi
* być w&nbsp;stanie zanotować błędy krytyczne związane z&nbsp;samym systemem wyprowadzania danych.
* 
* Silnik dostarcza także opakowanie dla strumieni - klasy 
* @c engine::data::input_streambuf_t dla wprowadzania i&nbsp;@c engine::data::output_streambuf_t
*  dla wyprowadzenia. Obie bazują na&nbsp;`std::streambuf` i&nbsp;pozwalają na&nbsp;tworzenie własnych strumieni C++
* 
* Istnieje także opcja stworzenia 'częściowego' wprowadzania. Jeżeli wiadomo, że&nbsp;źródłowy plik zawiera
* więcej niż jeden zasób (np. kilka różnych poziomów detali tekstury), można rozkazać obiektowi wprowadzającemu
* żeby przeczytał dane o&nbsp;określonej długości do&nbsp;bufora, a&nbsp;następnie użył go do&nbsp;stworzenia nowego obiektu wprowadzającego.
* Źródłowy strumień pozostanie nie zmodyfikowany, a&nbsp;tak stworzony obiekt wprowadzający operuje na&nbsp;kopii, więc może
* przeżyć oryginalny obiekt wprowadzający. Utworzony obiekt ma dostęp tylko do&nbsp;tej porcji danych która była
* zażądana przy jego tworzeniu.
*
* @section vfs_providers Dostawcy
* Dostawca to klasa opisująca sposób tworzenia obiektów wprowadzających/wyprowadzających dla danego zasobu. Zwyczajowy dostawcy
* są zbierani i&nbsp;używani przez wewnętrzne komponenty silnika i&nbsp;nie ma powodu manipulacja przy nich z&nbsp;poziomu aplikacji. 
* 
* Standardowo każdy dostawca właściwy posiada możliwość podania ostatniej daty modyfikacji zasobu.
* Ostatecznie klasa zbiorcza dostawy zawsze wybierze najnowszego dostawcę (do zapisu dostawca musi
* wskazywać na&nbsp;zasób zdolny do&nbsp;zarówno zapisu jak i&nbsp;odczytu).
* 
* Jeżeli żaden dostawca nie spełnia takiego warunku, nowy zostanie stworzony za&nbsp;pomocą funkcji<br>`engine::platform::create_new`
* 
* Jeżeli dany zasób jest niemożliwy do&nbsp;zapisania, można zwrócić obiekt `engine::data::output_void_t` który symuluje zapis,
* wysyłając dane w&nbsp;nicość (analogicznie do&nbsp;Linuxowego pliku `/dev/null`)
*
* @section vfs_scanners Skanery
* Skaner to niewielka klasa przeglądająca zasoby w&nbsp;regularnych odstępach czasowych (rzędu setek milisekund) aby sprawdzić
* ich datę modyfikacji. Zmiany zostają odnotowane i&nbsp;zaktualizowane w&nbsp;formie nowych dostawców. Lista dostępnych skanerów
* jest udostępniona jako ustawienia rozrusznika i&nbsp;zwykle zawiera kilka
* skanerów katalogów ze&nbsp;ścieżkami w&nbsp;jakich można się spodziewać zasobów.
*
* @section vfs_extension Rozszerzenia
* Skanery (opisane w&nbsp;poprzednim akapicie) posiadają także możliwość zarejestrowania wywołania zwrotnego po&nbsp;każdym znalezionym pliku.
* W&nbsp;obecnej wersji Pixie Engine istnieje jedno takie zaimplementowane rozszerzenie: skaner archiwum. Jeżeli skaner wykryje plik
* archiwum (np. zip) uruchomi kolejny skaner, operujący całkowicie na&nbsp;archiwum i&nbsp;czytający pliki w&nbsp;środku. Oczywiście jest to
* wciąż normalny skaner - w&nbsp;znaczeniu wciąż wykonujący zapytania zwrotne - i&nbsp;z tego powodu rekurencyjne archiwa (archiwum w&nbsp;archiwum) są jak najbardziej
* wspierane (ale niezalecane, jako że&nbsp;aby skaner mógł dostać się do&nbsp;wewnętrznego archiwum, musi je odpakować do&nbsp;pamięci).
* Skaner archiwum wygeneruje dostawców w&nbsp;formie archiwalnym które są w&nbsp;stanie stworzyć obiekt wprowadzania/wyprowadzania archiwalnego.
* 
* W&nbsp;formacie ZIP istnieje pewne zagrożenie które trzeba było uwzględnić w&nbsp;implementacji; O&nbsp;ile prawie wszyscy dostawcy korzystają z&nbsp;daty
* modyfikacji z&nbsp;dokładnością do&nbsp;jednej sekundy, archiwum zipa operuje na&nbsp;dacie modyfikacji pliku z&nbsp;dokładnością do&nbsp;dwóch sekund.
* Założeniem twórcy było dwukrotne zwiększenie zakresu czasowego 32-bitowej liczby bez znaku (patrz: problem roku 2038@cite year2038Problem),
* niestety w&nbsp;efekcie wpływa to na&nbsp;stabilność dostawcy i&nbsp;jego kolejki priorytetowej uwzględniającej datę zapisu. Dla pewności
* wszystkie operacje czasowe na&nbsp;zip są zaokrąglane w&nbsp;górę (co wymusza położenie archiwum z&nbsp;przodu kolejki, dla jednoczesnego zapisu z&nbsp;i bez archiwum)
* 
* @section vfs_example Przykład działania vfs
* Zasób na&nbsp;którym będziemy operować w&nbsp;niniejszym przykładzie ma wirtualną ścieżkę `script/orc_ai.code` o&nbsp;typie `game_asset`.
* 
* 1. Gracz wkłada płytę z&nbsp;grą do&nbsp;napędu. Dla oszczędności miejsca na&nbsp;dysku twardym zasób ten zostanie rozwiązany do&nbsp;lokacji na&nbsp;płycie (tj. `D:/script/orc_ai.code`)
* 2. Niestety, twórca odkrył iż&nbsp;popełnił błąd w&nbsp;skrypcie. Dystrybuuje zatem poprawkę zawierającą spakowany skrypt. Po&nbsp;jej instalacji istnieją już dwie lokacje zasobu, a&nbsp;gra wybiera tę nowszą (znajdującą się w&nbsp;`C:/Program Files/Pixie/patch.zip` ścieżka w&nbsp;archiwum `script/orc_ai.code`)
* 3. Gracz stwierdza iż&nbsp;nie podoba mu się zachowanie orków. Nie wyłączając gry, otwiera skrypt edytorem i&nbsp;wprowadza poprawki. Ponieważ zapis do&nbsp;`Program Files` wymaga uprawnień administracyjnych, gra tworzy trzecią kopię skryptu w&nbsp;lokacji `C:/Users/Piotr/AppData/Local/Pixie/script/orc_ai.code`
* 4. Uruchomiona w&nbsp;między czasie gra wykrywa istnienie nowego skryptu o&nbsp;nowszej dacie niż zapamiętana. Przeładowuje więc skrypt i&nbsp;aktualizuje referencje do&nbsp;niego bez przerywania rozgrywki
* 
* @section vfs_hotreload_problems Problemy z&nbsp;przeładowaniem zasobów na&nbsp;gorąco
* Jak widać po&nbsp;powyższym przykładzie pomimo zalet takiego rozwiązania, natychmiastowej informacji zwrotnej dla twórców modyfikacji,
* istnieje poważny problem z&nbsp;przeładowaniem zasobów na&nbsp;gorąco: gracz może podmienić dowolny zasób w&nbsp;trakcie działania programu.
* W&nbsp;szczególności może to wygenerować następujące problemy:
* 
* - Gracz może oszukiwać w&nbsp;grze wieloosobowej
* - Gracz może oszukiwać w&nbsp;osiągnięciach
* 
* Obie sytuacje wymagają innego podejścia do&nbsp;ich rozwiązania. 
* 
* Pierwszym krokiem jest podzielenie zasobów na&nbsp;kosmetyczne i&nbsp;kluczowe. I&nbsp;tak kosmetyczna jest np. grafika postaci (zmiana grafiki
* nie wpłynie znacząco na&nbsp;rozgrywkę), podczas gdy kluczowy jest np. rozkład poziomu (zmiana znacząco ułatwi bądź utrudni wykonanie
* zadania jakie stoi przed graczem). Podział taki należy częściowo do&nbsp;finałowej gry.
* 
* Aby uniknąć oszukiwania w&nbsp;grze wieloosobowej wprowadzono ideę odczepianych zasobów. Odczepiony zasób tworzy kopię zasobu w&nbsp;pamięci,
* kopię która przestaje śledzić fizyczny plik w&nbsp;systemie plików. W&nbsp;ten sposób serwer może przetransmitować zasób do&nbsp;klienta i&nbsp;mieć
* pewność że&nbsp;wszyscy klienci używają dokładnie tej samej wersji i&nbsp;danych zasobu. W&nbsp;takiej sytuacji zmiana pliku na&nbsp;dysku nie wpłynie
* na&nbsp;rozgrywkę do&nbsp;jej przerwania (i kontynuowania w&nbsp;trybie dla pojedynczego gracza)
* 
* Drugim problemem jest oszukiwanie w&nbsp;grze jednoosobowej, a&nbsp;więc w&nbsp;takiej która nie posiada autorytarnego serwera. Rozwiązaniem było 
* wprowadzenie opcji podpisywania zasobów. Podpis sam w&nbsp;sobie jest także zasobem, o&nbsp;nazwie identycznej z&nbsp;nazwą zasobu, ale dodatkowym rozszerzeniem.
* W&nbsp;ten sposób gra może szybko sprawdzić czy zasób pochodzi od&nbsp;twórcy oryginalnego czy też był modyfikowany przez gracza. W&nbsp;tym drugim
* przypadku gra może podjąć decyzję o&nbsp;wyłączeniu systemu osiągnięć, by&nbsp;nie generować możliwości oszukiwania.
* 
* @section vfs_assets System modyfikacji
* @subsection vfs_assets_intro Wprowadzenie
* Ostatnim elementem VFS jest system modyfikacji gry (w jego skład wchodzą zarówno modyfikacje fanowskie jak i&nbsp;dodatki udostępniane przez twórców).
* Stanowi on prostą warstwę abstrakcji ponad samym VFS i&nbsp;jest używany niemal wyłącznie przez samą grę (jako komponent całkowicie opcjonalny).
* 
* System modyfikacji opiera się o&nbsp;dwa typy modyfikacji gry:
*
* - Modyfikacje właściwe - zawierające duże rozdziały, bądź fanowskie modyfikacje pełnej konwersji
* - Podmodyfikacje - zawierające niewielkie elementy gry, bądź niewielkie fanowskie modyfikacje
* 
* @subsection vfs_assets_manifest Manifest
* Każda z&nbsp;modyfikacji zawiera manifest (w formacie XML) opisujący w&nbsp;jakich warunkach może być włączona. W&nbsp;ten sposób twórca jest
* w&nbsp;stanie łatwo dodać informację o&nbsp;niekompatybilnych dodatkach bezpośrednio do&nbsp;modyfikacji (użytkownik zostanie o&nbsp;tym problemie
* ostrzeżony w&nbsp;trakcie uruchamiania bądź ściągania modyfikacji). Zmiana układu modyfikacji nie wymaga restartu gry. Jeżeli problem 
* podczas uruchamiania modyfikacji jest możliwy do&nbsp;rozwiązania przez silnik, użytkownik zostanie o&nbsp;nim poinformowany,
* a&nbsp;następnie - po&nbsp;jego zgodzie - zostanie podjęta próba naprawienia. Przykład: podmodyfikacja "Wątek smoka na&nbsp;bagnach"
* wymaga modyfikacji "Bagna północne", która nie jest w&nbsp;danej chwili włączona; Gra automatycznie zaproponuje jej włączenie.
*/