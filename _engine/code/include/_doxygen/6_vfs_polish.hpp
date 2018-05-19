
/**
* \~Polish @page vfs Wirtualny System Plików (VFS)
* @tableofcontents
* @section vfs_intro Wprowadzenie
* Pixie Engine (jak wiele innych silników) stosuje wirtualny system plików (VFS), czyli warstwę abstrakcji ponad fizycznym
* systemem plików. Niniejszy rozdział opisuje wszystkie cechy VFS zaimplementowanego w tym silniku.
* 
* VFS Pixie Engine opiera się nie tylko na wczytywaniu danych, ale pozwala na swobodne modyfikowanie zasobów
* na przykład do wykorzystaniu przez edytor. Zasoby są przeładowywane dynamicznie w trakcie wykonania silnika
* i mogą zostać zaktualizowane przez dowolną aplikację Pixie Engine bądź fizycznie przez użytkownika poza silnikiem.
*
* @section vfs_vpath Wirtualna ścieżka
* Wirtualna ścieżka (vpath) reprezentuje nazwę zasobu wewnątrz VFS. Vpath składa się z dwóch elementów:
* @ref engine::virtual_path_t::type_t "typu" i adresu. Typ reprezentuje miejsce poszukiwania fizycznego pliku,
* podczas gdy adres przybliża nazwę tego pliku.
* 
* W praktyce vpath może opisywać więcej niż jeden fizyczny plik, lub nawet brak pliku. Jeżeli vpath odpowiada więcej
* niż jednemu plikowi próby tworzenia akcesora pliku wykorzystają zawsze najnowszy, spełniający warunki zasób.
* Dla wczytywania wystarczy warunek czytalności, podczas gdy do zapisu wymaganiem jest zarówno czytalność jak
* i zapisywalność. Jeżeli żaden plik nie spełnia warunku zapisywalności (np. istnieje tylko jedna kopia ulokowana
* na płycie CD), przy próbie tworzenia akcesora pliku zostanie stworzony nowy zasób w lokacji określonej przez platformę.
* 
* Spostrzegawczy użytkownik zauważy że dane czytane i zapisywane mogą lądować w dwóch różnych plikach fizycznych.
* W praktyce jednak po zapisie zaktualizowana zostanie data ostatniej modyfikacji, i tym samym ten plik przeskoczy
* na początek kolejki czytania (stąd wymóg czytalności od takiego pliku)
* @see engine::virtual_path_t
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
* @ref engine::data::input_streambuf_t dla wprowadzania i @ref engine::data::output_streambuf_t
*  dla wyprowadzenia. Obie bazują na std::streambuf i pozwalają na tworzenie własnych strumieni C++
* 
* Istnieje także opcja stworzenia 'częściowego' wprowadzania. Jeżeli wiadomo że źródłowy plik zawiera
* więcej niż jeden zasób (np. kilka różnych poziomów detali tekstury), można rozkazać wprowadzaczowi
* żeby przeczytał dane o określonej długości do bufora, a następnie użył go do stworzenia nowego wprowadzacza.
* Źródłowy strumień pozostanie nie zmodyfikowany, a tak stworzony wprowadzacz operuje na kopii, więc może
* przeżyć oryginalnego wprowadzacza. Utworzony wprowadzacz ma dostęp tylko do tej porcji danych która była
* zażądana przy jego tworzeniu. Zobacz metodę @ref engine::data::input_t::spawn_partial.
* @see engine::data::input_t, engine::data::output_t
*
* @section vfs_providers Dostawcy
* Dostawca to klasa opisująca sposób tworzenia wprowadzacza/wyprowadzacza dla danego zasobu. Zwyczajowy dostawcy
* są zbierani i używani przez wewnętrzne komponenty silnika i nie ma powodu manipulacja przy nich z poziomu aplikacji. 
* @warning Wprowadzacz i Wyprowadzacz mogą, w niektórych przypadkach, wskazywać na różne lokacje, nawet jeżeli vpath obu lokacji jest taki sam!
* Dla przykładu można czytać dane z pliku CD i zapisywć do Lokalnych Danych Aplikacji.
* 
* Standardowo każdy dostawca właściwy posiada możliwość podania ostatniej daty modyfikacji zasobu.
* Ostatecznie klasa zbiorcza dostawy zawsze wybierze najnowszego dostawcę (do zapisu dostawca musi
* wskazywać na zasób zdolny do zarówno zapisu jak i odczytu).
* 
* Jeżeli żaden dostawca nie spełnia takiego warunku, nowy zostanie stworzony za pomocą funkcji @ref engine::platform::create_new
* 
* Jeżeli dany zasób jest niemożliwy do zapisania, można zwrócic obiekt @ref engine::data::output_void_t który symuluje zapis,
* wysyłając dane w nicość (analogicznie do Linuxowego pliku `/dev/null`)
* @see engine::data::provider_t, engine::data::provider_actual_t
*
* @section vfs_scanners Skanery
* Skaner to niewielka klasa przeglądająca zasoby w regularnych odstępach czasowych (rzędu setek milisekund) aby sprawdzić
* ich datę modyfikacji. Zmiany zostają odnotowane i zaktualizowane w formie nowych dostawców. Lista dostępnych skanerów
* jest udostępniona jako ustawienia @ref engine_startup_platform "Rozrusznika" i zwykle zawiera kilka
* @ref engine::data::scanner_directory_t "skanerów katalogów" ze ścieżkami w jakich można się spodziewać zasobów.
*
* @see engine::data::scanner_t, engine::data::scanners_t
*
*/