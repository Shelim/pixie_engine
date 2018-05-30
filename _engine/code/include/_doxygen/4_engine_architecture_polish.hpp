
/**
* \~Polish @page engine_architecture Architektura silnika
* @tableofcontents
* @section engine_architecture_intro Wprowadzenie
* W&nbsp;trakcie wstępnych prac nad Pixie Engine rozważano i&nbsp;sprawdzano wiele różnych modeli architektury silnika; W&nbsp;szczególności poszukiwano rozwiązania które:
* 
* 1. będą łatwo testowalne (także automatycznie)
* 2. będą łatwo rozszerzalne
* 3. będą możliwie wydajne
* 
* Pierwszym rozważanym modelem było spojrzenie z&nbsp;góry (ang. top-down). Model ten opiera się na&nbsp;serii zarządców, którzy realizują konkretne funkcje gry.
* Zarządca posiada autorytarną kontrolę własnego modułu i&nbsp;odpowiada za&nbsp;jego działanie. Przykładem jest zarządca dźwięku który sprawuje wyłączną kontrolę
* nad kartą dźwiękową i&nbsp;który umożliwia kolejkowanie próbek do&nbsp;odtworzenia.
* 
* Takie rozwiązanie ma jednak podstawowe wady:@cite managerBad1 @cite managerBad2
*  - Jest bardzo trudno sprawdzić mechanicznie zarządcę bez znajomości jego wnętrza (złamanie (1)).
*  - Ukrywa powiązania między zarządcami (nie jest oczywiste czy np. zarządca grafiki wymaga dostępu do&nbsp;zarządcy zasobów)
*  - Problematyczne w&nbsp;programowaniu wielowątkowym (złamanie (3)).
*  - Zmiany logiki zarządcy wymagają wiedzy o&nbsp;całym zarządcy (złamanie (2))
* 
* Rozwiązanie to zostało odrzucone na&nbsp;bardzo wczesnym etapie.
* 
* Kolejnym rozważanym modelem było odwrócenie kontroli@cite ioc (tzw. pryncypał Hollywood @a "Nie dzwoń do&nbsp;nas - to my zadzwonimy do&nbsp;ciebie" @cite ioc2 ) i&nbsp;wstrzykiwanie zależności.@cite di
* Ten model posiada wiele autonomicznych wariantów, na&nbsp;początek rozważano model komponentowy:
* 
* Komponent to autonomiczna jednostka kodu realizująca pojedynczą funkcjonalność silnika. Np. komponentem jest odtwarzacz konkretnej próbki dźwięku,
* drugim komponentem jest układ kolejki próbek, a&nbsp;trzecim priorytetyzator tych kolejek. W&nbsp;ten sposób każdy komponent ma jasno określone przeznaczenie 
* i&nbsp;sposób działania, a&nbsp;brakującą funkcjonalność otrzymuje z&nbsp;innych komponentów.
* 
* W&nbsp;takim podejściu pojawia się jednak pewien problem - co oznacza pojedyncza funkcjonalność? Czy przyjęcie komunikatu dziennika i&nbsp;jego wyświetlenie to
* jedna czy dwie funkcjonalności? Co robić z&nbsp;komponentami które mogą posiadać kilka wariantów działania?
* 
* W&nbsp;ten sposób narodziła się idea dostawców@cite designPattern : niewielkich klas dostarczających dane bądź implementacje komponentom które mogły do&nbsp;tej pory posiadać kilka wariantów.
* Dla przykładu komponent dziennik przyjmujący komunikaty posiada dostawców wyjścia: plik, konsola, standardowy strumień wyjścia. Wybór aktywnych
* dostawców (niekiedy może być więcej niż jeden dostawca aktywny, w&nbsp;innej sytuacji może nie mieć to sensu) należy do&nbsp;implementacji danej platformy.
* Na&nbsp;przykład komponent system plików posiada obecnie dwóch dostawców: rodzajowy (używający wyłącznie biblioteki standardowej) i&nbsp;Windows
* (używający funkcji WinAPI, zoptymalizowany na&nbsp;tej platformie). Implementacja wybiera rodzaj dostawcy na&nbsp;etapie kompilacji, aby nie generować
* narzutu na&nbsp;wykonanie (punkt (3)).
* 
* Konkretna gra to nie tylko pojedyncza aplikacja zawierająca samą rozgrywkę, ale także zestaw narzędzi, uruchamiacz, biblioteki współdzielone i&nbsp;aktualizator. 
* W&nbsp;oryginalnym pomyśle każda z&nbsp;tych aplikacji posiadałaby niezależny plik wykonywalny kompilowany z&nbsp;osobnego katalogu z&nbsp;kodem, przy jednoczesnym utrzymaniu
* wspólnych elementów w&nbsp;jednym folderze.
* 
* W&nbsp;praktyce okazało się jednak że&nbsp;takie podejście ma kilka poważnych wad: po&nbsp;pierwsze, koordynowanie stanu uruchomienia kilku programów o&nbsp;kilku
* różnych plikach wykonywalnych nie jest łatwe; Wymaga gniazd, potoków bądź systemowych blokad wykluczających. Po&nbsp;drugie ponieważ każda z&nbsp;tych
* aplikacji działa niezależnie, rywalizują w&nbsp;dostępie do&nbsp;zasobów (generując dodatkowe wydarzenia monitorów, zobacz @ref engine_architecture_component_kind_monitor "monitory").
* Po&nbsp;trzecie system Windows traktuje takie aplikacje jako niezależne, tym samym nie pozwala np. na&nbsp;przenoszenie metodą przeciągnij-i-upuść kontrolek 
* i&nbsp;zasobów między nimi (ograniczałoby to możliwości edytora uruchomionego jednocześnie z&nbsp;grą).
* 
* W&nbsp;związku z&nbsp;tym zdecydowano się na&nbsp;zmianę podejścia: od&nbsp;tamtej chwili każda uruchomiona aplikacja była utrzymywana w&nbsp;kontenerze w&nbsp;tym samym procesie.
* W&nbsp;ten sposób istniała tylko jedna instancja silnika a&nbsp;wszystkie aplikacje uruchomione były po&nbsp;prostu w&nbsp;osobnych wątkach. W&nbsp;ten sposób komunikacja
* między aplikacyjna stała się trywialna, zasoby były odczytywane tylko raz - niezależnie od&nbsp;ilości uruchomionych aplikacji - a&nbsp;rozszerzenia eksploratora
* Windows działały bez zarzutu. Takie podejście wymusiło też wprowadzenie ostrego podziału na&nbsp;program i&nbsp;aplikację. Program stanowił po&nbsp;prostu kontener
* podczas gdy aplikacja to konkretna instancja wątku, posiadająca własną niezależną funkcję `main` i&nbsp;uruchomiona wewnątrz programu.
* 
* Oczywiście kolejne uruchomienie programu (w znaczeniu kontenera) sprawdza systemową blokadą wykluczającą czy inna instancja kontenera już działa,
* a&nbsp;jeżeli tak to przesyłała do&nbsp;niej informację (za pomocą nazwanego strumienia) o&nbsp;konieczności uruchomienia nowej aplikacji, po&nbsp;czym kończyła wykonanie.
* W&nbsp;ten sposób natywne dla Windowsa dwukliknięcie na&nbsp;program w&nbsp;eksploratorze Windows uruchamiało nową aplikację w&nbsp;obecnej instancji,
* zamiast generować nową instancję.
* 
* @section engine_architecture_utilities Narzędzia pomocnicze
* Poza systemem komponentów znajduje się seria tzw. narzędzi pomocniczych. Jest to zbiór funkcji i&nbsp;klas które nie wchodzą do&nbsp;głównej części silnika
* i&nbsp;stanowią raczej bazę i&nbsp;bibliotekę standardową z&nbsp;której korzystają wszystkie wyższe elementy Pixie Engine. W&nbsp;szczególności narzędzia pomocnicze 
* nie znają budowy samego silnika, ustawień ani komponentów i&nbsp;mogą być przeniesione na&nbsp;dowolny inny projekt bez potrzeby dublowania jakiejkolwiek
* cechy samego silnika.
* 
* Narzędzia pomocnicze obejmują obecnie następujące grupy:
* @subsection engine_architecture_utilities_concurrention Wielowątkowość
* @subsubsection engine_architecture_utilities_concurrention_signal Sygnał
* To proste narzędzie blokujące wykonanie danego wątku w&nbsp;sposób nie obciążający procesora do&nbsp;nadejścia sygnału zwolnienia blokady z&nbsp;innego wątku.
* @subsection engine_architecture_utilities_container Kontener
* @subsubsection engine_architecture_utilities_container_compile_time_map Słownik czasu kompilacji
* Jest to implementacja prostego słownika par klucz-wartość dowolnych typów oparta całkowicie o&nbsp;szablony
* i&nbsp;rozwiązująca pytania o&nbsp;elementy w&nbsp;czasie kompilacji. Z&nbsp;powodu zaawansowanego meta-programowania słownik czasu kompilacji nie może posiadać
* klucza opartego o&nbsp;wartość tekstową; Jest to bezpośrednie ograniczenie możliwości szablonów w&nbsp;C++.
* @subsubsection engine_architecture_utilities_container_concurrent_queue Równoległa kolejka
* Jest to prosta implementacja równoległej kolejki w&nbsp;modelu producent-konsument@cite producerconsumer. Kolejka blokuje wykonanie wątku konsumenta (w sposób nie obciążający procesora)
* aż&nbsp;do momentu nadejścia nowego obiektu w&nbsp;wątku producenta. Kolejka realizuje model jeden-konsument wielu-producentów i&nbsp;nie wspiera dodatkowego mechanizmu
* przerywającego oczekiwanie (zwyczajowo można po&nbsp;prostu wysłać pusty wskaźnik bądź specjalnie oznaczony obiekt jeżeli potrzebne jest przerwanie)
* @subsubsection engine_architecture_utilities_container_sync Synchroniczna kolejka i&nbsp;synchroniczna mapa
* To kolejka i&nbsp;mapa ze&nbsp;standardowej biblioteki C++ opakowana w&nbsp;blokady równoległe do&nbsp;używania w&nbsp;silnie wielowątkowym środowisku.
* @subsection engine_architecture_utilities_debug Debugger
* @subsubsection engine_architecture_utilities_debug_callstack Stos wywołań
* To prosty kontener przechowujący informacje o&nbsp;stosie wywołań w&nbsp;sposób wieloplatformowy. Wspiera możliwość uzupełnienia kolekcji o&nbsp;informacje debuggera,
* także na&nbsp;etapie post-mortem.
* @subsubsection engine_architecture_utilities_debug_remotery Remotery
* Otoczka C++ do&nbsp;biblioteki remotery
* @subsection engine_architecture_utilities_pattern Wzorce programistyczne
* @subsubsection engine_architecture_utilities_pattern_class_settings Ustawienia klasy
* To sposób konfigurowania klas w&nbsp;czasie kompilacji oparty o&nbsp;szablony i&nbsp;makra. Bazowo twórca danej klasy tworzy zestawy ustawień
* (np. "na Windows","wariant przenośny") a&nbsp;implementacja danej platformy może wybrać który z&nbsp;tych zestawów użyć w&nbsp;praktyce.
* @subsubsection engine_architecture_utilities_pattern_enum Enumeracje
* Proste makro do&nbsp;tworzenia własnych silnie typowanych enumeracji które automatycznie przeciążają funkcje engine::to_string() i&nbsp;engine::from_string()
* dla własnego typu.
* @subsubsection engine_architecture_utilities_pattern_factory Fabryka
* Poprawiona implementacja fabryki abstrakcyjnej@cite designpatterns z&nbsp;Boost Dependency Injection. Zobacz rozdział @ref engine_problems_di oraz @ref dependency_di
* @subsubsection engine_architecture_utilities_pattern_flags Flagi
* Szablonowa klasa (podstawą szablonu jest silnie typowana enumeracja) tworząca flagi ustawień (tj. serię przełączników mogących być w&nbsp;danej chwili
* włączonych bądź wyłączonych). Flagi są zoptymalizowane przestrzennie i&nbsp;zawierają podstawowy zestaw metod ustawiających je i&nbsp;zdejmujących.
* @subsubsection engine_architecture_utilities_pattern_fourcc Identyfikator
* To implementacja idei czteroliterowych (32 bit) identyfikatorów@cite FourCCWikipedia używanych np. w&nbsp;koderach-dekoderach plików wideo.@cite FourCC
* Implementacja Pixie Engine oparta jest o&nbsp;szablony i&nbsp;uwzględnia kodowanie, różną kolejność bajtów na&nbsp;różnych platformach oraz potrafi wygenerować
* identyfikator jako `constexpr` (co oznacza że&nbsp;może być np. wykorzystywany jako etykieta `case` w&nbsp;instrukcji `switch`).
* @subsubsection engine_architecture_utilities_pattern_provider Dostawca
* Zbiór podstawowych metod do&nbsp;tworzenia dostawców komponentów (zobacz poniżej).
* @subsubsection engine_architecture_utilities_pattern_writer Writer
* Klasa umożliwiająca transakcyjne operacje ("Wszystko albo żaden" oraz "Nie rozdzielaj w&nbsp;trakcie") na&nbsp;dowolnym strumieniu zdolnym do&nbsp;zapisywania sekwencji obiektów
* @subsection engine_architecture_utilities_platform Platformowe
* @subsubsection engine_architecture_utilities_platform_args Linia komend
* Klasa reprezentująca wieloplatformowy stan linii komend. Pozwala określić jakie przełączniki i&nbsp;argumenty są aktualnie ustawione,
* potrafi stworzyć swoją instancję z&nbsp;pojedynczego tekstu jak i&nbsp;argumentów `argc` i&nbsp;`argv` funkcji `main`. Pozwala także na&nbsp;manipulację tymi argumentami.
* @subsubsection engine_architecture_utilities_platform_debug Debug
* To zbiór (obecnie) trzech pomocniczych funkcji debuggera posiadających osobne implementacje na&nbsp;każdej platformie i&nbsp;wymagających globalnego dostępu
* (dlatego nie tworzą obiektów):
*  - engine::platform::trigger_breakpoint()
*  - engine::platform::canonize_debug_filename()
*  - engine::platform::dump_callstack()
* @subsubsection engine_architecture_utilities_platform_version Wersja
* To prosta klasa opisująca wersję danego obiektu/komponentu/aplikacji/programu. Posiada cztery człony (główna, pomniejsza, rewizja i&nbsp;kompilacja),
* i&nbsp;umożliwia porównywanie na&nbsp;różnych polach, a&nbsp;także konwersje z&nbsp;i do&nbsp;łańcucha znaków.
* @subsection engine_architecture_utilities_renderer Renderer
* @subsubsection engine_architecture_utilities_renderer_color Kolor
* Wieloplatformowa klasa zdolna przechować kolor w&nbsp;formacie RGB. 
* @subsection engine_architecture_utilities_security Bezpieczeństwo
* @subsubsection engine_architecture_utilities_security_crc32 CRC32
* Funkcje wyliczające skrót CRC32 dowolnych danych
* @subsection engine_architecture_utilities_test Testowe
* @subsubsection engine_architecture_utilities_test_print_to Drukuj do
* Pomocnicze funkcje do&nbsp;drukowania silnikowych @ref engine::ustring_t "łańcuchów znaków" na&nbsp;potrzeby Google Test Framework
* @subsection engine_architecture_utilities_text Tekstowe
* @subsubsection engine_architecture_utilities_text_difference Różnica
* Oblicza odległość Levenshteina dwóch silnikowych łańcuchów znaków
* @subsubsection engine_architecture_utilities_text_expand Rozwinięcie
* Konwertuje serię dowolnych argumentów w&nbsp;tablicę łańcuchów znaków typu `ustring_t`. Wykorzystywane m.in. do&nbsp;formatowania tekstu.
* @subsubsection engine_architecture_utilities_text_parser Parser
* Zbiór narzędzi do&nbsp;wykorzystania w&nbsp;parserze Spirit (część biblioteki boost)
* @subsubsection engine_architecture_utilities_text_ustring Łańcuch znaków
* Własna implementacja łańcucha znaków wspierająca w&nbsp;pełni kodowanie UTF-8 (natywne rozwiązanie C++ nie jest dostępne na&nbsp;wielu platformach).
* 
* @section engine_architecture_core Rdzeń
* Oczywiście nie całą funkcjonalność można zamknąć w&nbsp;komponentach które mogą być włączone bądź wyłączone. Istnieje pewna część kodu która należy
* bezpośrednio do&nbsp;silnika i&nbsp;musi być zawsze dostępna. W&nbsp;ten sposób w&nbsp;naturalny sposób wykrystalizował się rdzeń Pixie Engine.
* 
* Przy decydowaniu która funkcjonalność należy do&nbsp;rdzenia a&nbsp;która do&nbsp;komponentów podjęto decyzję że&nbsp;w&nbsp; przypadku wątpliwości będzie wybierany zawsze komponent.
* Rdzeń miał zawierać tylko absolutne minimum kodu, niezbędne do&nbsp;działania logiki silnika.
* 
* Po&nbsp;rozważaniach nad podziałem kodu na&nbsp;globalny (dotyczący wszystkich uruchomionych aplikacji) i&nbsp;lokalny (dotyczący konkretnej aplikacji) w&nbsp;sposób
* naturalny pojawił się także podział rdzenia. W&nbsp;ten sposób istnieje rdzeń dotyczący całego programu i&nbsp;osobne instancje rdzenia każdej aplikacji.
* 
* W&nbsp;chwili obecnej w&nbsp;skład rdzenia wchodzą wymienione w&nbsp;poniższych podrozdziałach elementy.
* @subsection engine_architecture_core_global Globalne
* @subsubsection engine_architecture_core_global_accountable Księgowi
* Jest to rodzajowy zbiór klas służących do&nbsp;notowania ilości i&nbsp;adresów zasobów silnika, z&nbsp;pominięciem plików danych (których byłoby zbyt dużo
* żeby implementacja była efektywna). W&nbsp;tej chwili rejestrowane są dwie rzeczy: wątki (wraz z&nbsp;zagospodarowaniem procesora) i&nbsp;same aplikacje
* (wraz z&nbsp;zajętością zasobów). Księgowi odnotowują akt stworzenia i&nbsp;zniszczenia zasobu i&nbsp;są w&nbsp;stanie przedstawić migawkę zasobów istniejących
* w&nbsp;aktualnej chwili programu.
* 
* @subsubsection engine_architecture_core_global_app Aplikacje
* Ten element rdzenia odpowiada za&nbsp;stworzenie nowych instancji aplikacji, uruchomienie wątków i&nbsp;obsługę głównej funkcji (`main`) każdej z&nbsp;aplikacji.
* O&nbsp;ile oryginalnie nie wchodziło to w&nbsp;skład tego elementu, dodano do&nbsp;niego także - inspirowany systemem operacyjnym Unix - system przerwań (odpowiednik sygnałów).
* Przerwanie aplikacji to np. informacja o&nbsp;tym że&nbsp;użytkownik poprosił aplikację o&nbsp;wyłączenie, albo nowa instancja właśnie próbuje być uruchomiona.
* Niektóre aplikacje mogą nie chcieć wspierać więcej niż jednej instancji, aczkolwiek ten prosty scenariusz może być obsłużony w&nbsp;inny sposób - te bardziej
* rozbudowane wymagają przerwania. Ten element rdzenia zawiera także bootstrappera odpowiedzialnego za&nbsp;rozruch danej aplikacji
* (zobacz @ref engine_startup_app "Bootstrapper")
* w&nbsp;danej chwili programu.
* 
* @subsubsection engine_architecture_core_global_console Konsola
* Jednym z&nbsp;najstarszych konceptów silnika gier jest pojęcie konsoli, zaproponowane jeszcze w&nbsp;latach 80 i&nbsp;będące standardem branży od&nbsp;lat 90.
* Konsole były tworzone na&nbsp;wzór terminali systemów operacyjnych i&nbsp;w przeciwieństwie do&nbsp;dzienników umożliwiały interakcję użytkownika z&nbsp;danymi tekstowymi,
* tym samym dając narzędzie do&nbsp;szybkiego zarządzania grą dla administratorów serwerów i&nbsp;bardziej technicznych graczy.
* 
* W&nbsp;Pixie Engine wewnętrznie konsola oparta jest o&nbsp;pętlę wiadomości, które posiadają możliwość konwersji do&nbsp;tekstu celem wyświetlenia ich użytkownikowi.
* W&nbsp;przeciwieństwie jednak do&nbsp;innych gier, konsola Pixie Engine trzyma wewnętrznie obiekty a&nbsp;nie tekst. Dzięki takiemu podejściu można np. przeformatować
* dane w&nbsp;zależności od&nbsp;potrzeb, lub uzyskać dodatkowe informacje na&nbsp;temat każdej linijki.
* 
* Jeżeli chodzi o&nbsp;parsowanie wejścia, wykorzystana jest biblioteka @ref ustring_format_spirit "Boost Spirit" z&nbsp;autorską implementacją systemu na&nbsp;wzór intellisense
* poprawiającą błędy wejścia i&nbsp;posiadająca funkcjonalność autouzupełniania poleceń@cite spirit
* 
* @subsubsection engine_architecture_core_global_data Dane
* To zbiór klas stanowiący bazę wirtualnego systemu plików (zobacz @ref vfs ). W&nbsp;tej chwili w&nbsp;skład rdzenia należą: strumienie wejścia/wyjścia,
* dostawcy strumieni, skanery i&nbsp;archiwa.
* 
* @subsubsection engine_architecture_core_global_manifest Manifest
* Manifest obejmuje metadane platformy na&nbsp;której uruchomiony jest Pixie Engine jak i&nbsp;samego silnika. W&nbsp;chwili obecnej obejmuje on:
*  - Informacje o&nbsp;systemie operacyjnym
*  - Manifest silnika (zobacz rozdział @ref manifest "manifest silnika")
*  - Wersję silnika
*
* @subsubsection engine_architecture_core_global_messenger Komunikator
* Jednym z&nbsp;największych wyzwań stojących przed budową komponentową (w znaczeniu rozproszoną) była konieczność synchronizacji transferu informacji między
* poszczególnymi komponentami. W&nbsp;pierwszej fazie implementacji rozważano po&nbsp;prostu przekazywanie wskaźników na&nbsp;komponenty do&nbsp;konstruktorów
* (za pomocą wstrzykiwania zależności) co wygenerowało bardzo poważny problem: cykliczne zależności. Takie grafy obiektów były niemożliwe do
* stworzenia i&nbsp;generowały zawieszenie się aplikacji podczas startu - do&nbsp;tego były to zawieszenia bardzo trudne do&nbsp;analizy
* (długie szablonowe stosy wywołań). Jedynym rozwiązaniem było znalezienie sposobu na&nbsp;rozdzielenie komunikacji od&nbsp;faktycznych komponentów.
* Wzorzec komunikatora@cite messagingPattern jest tutaj idealnym rozwiązaniem.
*
* Komunikator jest w&nbsp;tym wypadku szablonową klasą opartą o&nbsp;kolejkę wielu-producentów jeden-konsument i&nbsp;posiada cztery warianty implementacji
* (synchroniczne albo asynchroniczne wysyłanie i&nbsp;odbieranie komunikatów). Warianty synchroniczne generują najmniejsze opóźnienia ale nie są przeznaczone
* do&nbsp;przesyłania dużych ilości wiadomości (gdyż sam akt wysyłania generuje blokadę wątkową). Implementacja danego typu wiadomości może zdecydować
* który wariant kolejki zostanie do&nbsp;niej przypisany (za pomocą wyrażeń typu `constexpr`).
*
* @subsubsection engine_architecture_core_global_platform Platformowe
* Ta sekcja rdzenia zawiera elementy które - jak ktoś mógłby zauważyć - należą bardziej do&nbsp;narzędzi pomocniczych niż klas średniego poziomu.
* W&nbsp;praktyce jednak okazało się że&nbsp;ich implementacja wymaga dostępu do&nbsp;SDLa - który włączony jest do&nbsp;projektu właśnie na&nbsp;poziomie rdzenia.
* Dlatego ostatecznie wylądowały w&nbsp;tej części silnika. W&nbsp;chwili obecnej są to tylko dwa narzędzia:
*  - Detektor cech CPU
*  - Zestaw zaawansowanych danych o&nbsp;platformie (szerokość linii cache L1, wersja systemu, etc.)
* 
* @subsubsection engine_architecture_core_global_policy Polityki
* O&nbsp;ile sam silnik nie nakłada zbyt wielu ograniczeń na&nbsp;twórcę gier, o&nbsp;tyle niektóre platformy posiadają silniejsze obostrzenia. Dla przykładu
* niemal wszystkie implementacje OpenGL pozwalają na&nbsp;wywołania graficznego interfejsu programistycznego tylko z&nbsp;jednego wątku. W&nbsp;przypadku
* jednoczesnego prowadzenia wielu aplikacji w&nbsp;jednym kontenerze stanowiłoby to kłopot;  Kłopot którego jedynym rozwiązaniem jest stworzenie
* komponentu globalnego na&nbsp;wszelkie wywołania renderera, z&nbsp;wszystkich aplikacji. Takie podejście jest jednak jeszcze bardziej ograniczone
* na&nbsp;niektórych platformach mobilnych: tam renderer może być wywołany tylko z&nbsp;jednego konkretnego, zadanego przez system wątku. W&nbsp;ten sposób
* narodziły się dwa warianty polityki Wątek Renderera: odczepiony i&nbsp;wołany przez platformę. Analogiczne ograniczenia dotyczą liczby instancji
* aplikacji i&nbsp;programu które stworzyły kolejne dwie polityki (pojedyncze, pojedyncze zadanego typu, wielokrotne).
*
* @subsubsection engine_architecture_core_global_process Procesy
* Procesy to zestaw wielowątkowych narzędzi na&nbsp;których bazuje praktycznie cała implementacja wyższego poziomu. W&nbsp;początkowej wersji rozważano
* po&nbsp;prostu współbieżność w&nbsp;oparciu o&nbsp;wątki, ale takie podejście bardzo utrudniało rzeczywiste zastosowanie, np. w&nbsp;ładowaniu zasobów. Proces
* ładowania wymaga koordynacji między wątkami i&nbsp;przeskakiwania między wątkiem czytającym z&nbsp;dysku a&nbsp;wątkiem renderera. Sytuację rozwiązało skupienie
* się na&nbsp;zadaniach i&nbsp;gońcach. Zadanie, to pojedyncze wyzwanie przed systemem współbieżnym. Istnieje podział na&nbsp;zadania proste (jak np. wspomniane
* wczytanie zasobu) jak i&nbsp;zadania długotrwałe (nazwane właśnie procesami). Goniec to aktywny element wykonujący zadania. Każde zadanie składa się z&nbsp;elementów
* z&nbsp;których każdy wykonywany jest w&nbsp;ramach jednego gońca. Elementy można kolejkować i&nbsp;uzależniać od&nbsp;siebie (np. wysłanie obrazu do&nbsp;karty graficznej nastąpi
* po&nbsp;jego wczytaniu z&nbsp;dysku). W&nbsp;chwili obecnej istnieją cztery typy gońców:
*  - Główny wątek; W&nbsp;tym momencie jedynie renderer. Zadania przydzielone temu gońcowi zostaną zakolejkowane na&nbsp;zadanym wątku (kolejką priorytetową)
*  - Wątek oddzielony; Każde zadanie dostanie nowy, dedykowany wątek który zostanie zniszczony po&nbsp;jego wykonaniu. Nie jest to zalecane dla zadań które 
*    często się powtarzają (koszt stworzenia wątku jest dość znaczący)
*  - Synchroniczny; Zadanie zostanie wykonane w&nbsp;wątku wywołującym, bez zrównoleglania
*  - Pula wątków (ang. thread pool); Określona ilość wątków (domyślnie tyle ile rdzeni w&nbsp;komputerze) kolejkujące zadania i&nbsp;wykonujące je na&nbsp;pierwszym wolnym wątku.
* 
* @subsubsection engine_architecture_core_global_program Program (kontener)
* Program to zestaw trzech narzędzi: rozrusznik, odpowiedzialny za&nbsp;start całej aplikacji, uruchamiacz testów stanowiący rodzaj rozrusznika,
* ale tylko na&nbsp;potrzeby Google Test Framework oraz referencja, czyli obiekt przechowujący instancję programu (prosząc o&nbsp;niego w&nbsp;konstruktorze
* gra dostaje dostęp do&nbsp;wszystkich globalnych komponentów)
*
* @subsubsection engine_architecture_core_global_thread Wątki
* Wątki to baza dla modułu procesów. Ten fragment rdzenia odpowiada za&nbsp;tworzenie i&nbsp;zliczanie istniejących wątków z&nbsp;uwzględnieniem statystyk
* ich wykonania. Standardowo stosuje się do&nbsp;tego bibliotekę standardową C++ 11 (z opcją rozszerzenia o&nbsp;dodatkowe informacje dostarczane przez platformę)
* 
* @subsubsection engine_architecture_core_global_vfs Wirtualny System Plików
* Ostatni element globalnego rdzenia to po&nbsp;prostu klasa opakowująca wirtualną ścieżkę. Więcej informacji znajduje się w&nbsp;rozdziale @ref vfs "wirtualny system plików"
* @subsection engine_architecture_core_local Lokalne
* @subsubsection engine_architecture_core_local_manifest Manifest aplikacji
* To jedyna jak na&nbsp;razie klasa rdzenia lokalnego. Zawiera informacje o:
*  - Typie aktualnej aplikacji
*  - Numerze jej instancji
*  - Kontekście wykonania
*
* @section engine_architecture_component Komponenty
* Komponent, jak już zostało powiedziane w&nbsp;poprzednich rozdziałach, to podstawowy budulec silnika Pixie Engine. Komponent jest (zwykle) małą klasą o&nbsp;pojedynczej
* funkcjonalności i&nbsp;odpowiedzialności, która niezbędne informacje (w ramach swojego pola działania) zdobywa od&nbsp;innych komponentów lub (poza swoim polem działania)
* od&nbsp;komunikatora z&nbsp;rdzenia silnika. W&nbsp;szczególności komponent jest całkowicie nieświadomy istnienia innych komponentów poza swoim polem działania.
* 
* Jak już zostało wspomniane każdy komponent posiada trzy implementacje:
*  - Normalną, zawierającą pełną wersję implementacji
*  - Makietową, opakowaną w&nbsp;makra Google Mockup
*  - Pustą, zawierającą puste metody
* @subsection engine_architecture_component_kind Rodzaje komponentów
* Nie było to zamysłem twórcy, ale dość szybko na&nbsp;etapie implementacji konkretnych komponentów wykrystalizowało sie kilka modeli w&nbsp;które komponenty się wpasowują.
* O&nbsp;ile opisanie wszystkich powstałych komponentów byłoby trudne (w tej chwili 27 globalnych i&nbsp;2 lokalne) można je z&nbsp;grubsza podzielić na&nbsp;następujące kategorie:
* @subsubsection engine_architecture_component_kind_monitor Monitory
* To komponenty operujące tylko na&nbsp;komunikatorach i&nbsp;zwykle nie posiadające żadnej publicznej metody ani własności. Ich celem jest skanowanie – na
* zadany wyzwalacz (np. czasowy) – sprawdzają czy dany element środowiska się nie zmienił. Najbardziej modelowym przykładem jest skaner zasobów sprawdzający
* czy nie trzeba ich przeładować na&nbsp;gorąco (zobacz rozdział @ref vfs_assets).
* @subsubsection engine_architecture_component_kind_service Usługi
* To mniejsze komponenty pracujące w&nbsp;wydzielonych wątkach, których celem jest dokonywanie aktywnych obliczeń. Najlepszym przykładem jest
* tu na&nbsp;przykład usługa profilera (w tym celu wykorzystane jest remotery)
* @subsubsection engine_architecture_component_kind_facade Fasady
* Nie mylić ze&nbsp;wzorcem projektowym fasady. Fasady w&nbsp;Pixie Engine to komponenty przyjmujące polecenia z&nbsp;reszty silnika za&nbsp;pomocą komunikatorów i&nbsp;dystrybuujące je do&nbsp;innych komponentów w&nbsp;ramach tego samego obszaru.
* @subsubsection engine_architecture_component_kind_standard Standardy
* Czyli najbardziej typowe komponenty posiadające jasno zadeklarowaną funkcjonalność i&nbsp;publiczne API w&nbsp;formie metod wirtualnych.
*
* @section engine_architecture_provider Dostawcy
* Ostatnim elementem architektury silnika jest koncept dostawcy. Ponieważ wiele komponentów posiada osobne warianty implementacji
* bądź może produkować (zdobywać) zasoby na&nbsp;kilka sposobów pojawił się problem kombinatoryczny rosnącej wykładniczo ich liczby.
* Aby ominąć tę pułapkę wprowadzono ideę dostawców kodu.
* 
* Dostawca to niesamodzielny element kodu dostarczający funkcjonalność komponentowi. Przykładem dostawcy jest dostawca wyjścia
* dziennika: konsola, plik, strumień. Istnieją też dostawcy funkcjonalności z&nbsp;których tylko jeden może być aktywny w&nbsp;danej chwili
* (np. dostawca danych o&nbsp;wątkach: rodzajowy, korzystający wyłącznie z&nbsp;biblioteki standardowej/boost i&nbsp;Windowsowy, korzystający
* z&nbsp;WinAPI aby zdobyć pełne informacje)
* 
* Dostawcy muszą być zarejestrowani na&nbsp;etapie kompilacji i&nbsp;za ich rejestrację odpowiada rozrusznik lub bootstrapper. 
* 
*/