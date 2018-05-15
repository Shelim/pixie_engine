
/**
* \~Polish @page engine_architecture Architektura silnika
* @tableofcontents
* @section engine_architecture_intro Wprowadzenie
* W trakcie wstępnych prac nad Pixie Engine rozważano i sprawdzano wiele różnych modeli architektury silnika; W szczególności poszukiwano rozwiązania które:
* 
* 1. będą łatwo testowalne (także automatycznie)
* 2. będą łatwo rozszerzalne
* 3. będą możliwie wydajne
* 
* Pierwszym rozważanym modelem było spojrzenie z góry (ang. top-down). Model ten opiera się na serii zarządców, którzy realizują konkretne funkcje gry.
* Zarządca posiada autorytarną kontrolę własnego modułu i odpowiada za jego działanie. Przykładem jest zarządca dźwięku który sprawuje wyłączną kontrolę
* nad kartą dźwiękową i który umożliwia kolejkowanie próbek do odtworzenia.
* 
* Takie rozwiązanie ma jednak podstawowe wady:
*  - Jest bardzo trudno sprawdzić mechanicznie zarządcę bez znajomości jego wnętrza (złamanie (1)).
*  - Ukrywa powiązania między zarządcami (nie jest oczywiste czy np. zarządca grafiki wymaga dostępu do zarządcy zasobów)
*  - Bardzo trudno się zrównolegla (złamanie (3)).
*  - Zmiany logiki zarządcy wymagają wiedzy o całym zarządcy (złamanie (2))
* 
* Rozwiązanie to zostało odrzucone na bardzo wczesnym etapie.
* 
* Kolejnym rozważanym modelem było odwrócenie kontroli (tzw. pryncypał Hollywood @a "Nie dzwoń do nas - to my zadzwonimy do ciebie") i wstrzykiwanie zależności.
* Ten model posiada wiele autonomicznych wariantów, na początek rozważano model komponentowy:
* 
* Komponent to autonomiczna jednostka kodu realizująca pojedynczą funkcjonalność silnika. Np. komponentem jest odtwarzacz konkretnej próbki dźwięku,
* drugim komponentem jest układ kolejki próbek, a trzecim priorytetyzator tych kolejek. W ten sposób każdy komponent ma jasno określone przeznaczenie 
* i sposób działania, a brakującą funkcjonalność otrzymuje z innych komponentów.
* 
* W takim podejściu pojawia się jednak pewien problem - co oznacza pojedyncza funkcjonalność? Czy przyjęcie komunikatu dziennika i jego wyświetlenie to
* jedna czy dwie funkcjonalności? Co robić z komponentami które mogą posiadać kilka wariantów działania?
* 
* W ten sposób narodziła się idea dostawców: niewielkich klas dostarczających dane bądź implementacje komponentom które mogły do tej pory posiadać kilka wariantów.
* Dla przykładu komponent dziennik przyjmujący komunikaty posiada dostawców wyjścia: plik, konsola, standardowy strumień wyjścia. Wybór aktywnych
* dostawców (niekiedy może być więcej niż jeden dostawca aktywny, w innej sytuacji może nie mieć to sensu) należy do implementacji danej platformy.
* Na przykład komponent system plików posiada obecnie dwóch dostawców: rodzajowy (używający wyłącznie biblioteki standardowej) i Windows
* (używający funkcji WinAPI, zoptymalizowany na tej platformie). Implementacja wybiera rodzaj dostawcy na etapie kompilacji, aby nie generować
* narzutu na wykonanie (punkt (3)).
* 
* Konkretna gra to nie tylko pojedyncza aplikacja zawierająca samą rozgrywkę, ale także zestaw narzędzi, uruchamiacz, biblioteki współdzielone i aktualizator. 
* W oryginalnym pomyśle każda z tych aplikacji posiadałaby niezależny plik wykonywalny kompilowany z osobnego katalogu z kodem, przy jednoczesnym utrzymaniu
* wspólnych elementów w jednym folderze.
* 
* W praktyce okazało się jednak że takie podejście ma kilka poważnych wad: po pierwsze, koordynowanie stanu uruchomienia kilku programów o kilku
* różnych plikach wykonywalnych nie jest łatwe; Wymaga gniazd, potoków bądź systemowych blokad wykluczających. Po drugie ponieważ każda z tych
* aplikacji działa niezależnie, rywalizują w dostępie do zasobów (generując dodatkowe wydarzenia monitorów, zobacz @ref engine_architecture_monitor "monitory").
* Po trzecie system Windows traktuje takie aplikacje jako niezależne, tym samym nie pozwala np. na przenoszenie metodą przeciągnij-i-upuść kontrolek 
* i zasobów między nimi (ograniczałoby to możliwości edytora uruchomionego jednocześnie z grą).
* 
* W związku z tym zdecydowano się na zmianę podejścia: od tamtej chwili każda uruchomiona aplikacja była utrzymywana w kontenerze w tym samym procesie.
* W ten sposób istniała tylko jedna instancja silnika a wszystkie aplikacje uruchomione były po prostu w osobnych wątkach. W ten sposób komunikacja
* między aplikacyjna stała się trywialna, zasoby były odczytywane tylko raz - niezależnie od ilości uruchomionych aplikacji - a rozszerzenia eksploratora
* Windows działały bez zarzutu. Takie podejście wymusiło też wprowadzenie ostrego podziału na program i aplikację. Program stanowił po prostu kontener
* podczas gdy aplikacja to konkretna instancja wątku, posiadająca własnego niezależnego `main` i uruchomiona wewnątrz programu.
* 
* Oczywiście kolejne uruchomienie programu (w znaczeniu kontenera) sprawdza systemową blokadą wykluczającą czy inna instancja kontenera już działa,
* a jeżeli tak to przesyłała do niej informację (za pomocą nazwanego strumienia) o konieczności uruchomienia nowej aplikacji, po czym kończyła wykonanie.
* W ten sposób natywne dla Windowsa dwukliknięcie na program w eksploratorze Windows uruchamiało nową aplikację w obecnej instancji,
* zamiast generować nową instancję.
* 
* Jako ciekawostkę można dodać że takie podejście zostało spopularyzowane przez przeglądarkę internetową Firefox około roku 2009-2010,
* a jego jedyną wadą jest fakt że w przypadku zawieszenia się jednej instancji aplikacji, awarii ulega cały kontener.
* 
* @section engine_architecture_utilities Narzędzia pomocnicze
* Poza systemem komponentów znajduje się seria tzw. narzędzi pomocniczych. Jest to zbiór funkcji i klas które nie wchodzą do głównej części silnika
* i stanowią raczej bazę i bibliotekę standardową z której korzystają wszystkie wyższe elementy Pixie Engine. W szczególności narzędzia pomocnicze 
* nie znają budowy samego silnika, ustawień ani komponentów i mogą być przeniesione na dowolny inny projekt bez potrzeby dublowania jakiejkolwiek
* cechy samego silnika.
* 
* Narzędzia pomocnicze obejmują obecnie następujące grupy:
* @subsection engine_architecture_utilities_concurrention Wielowątkowość
* @subsubsection engine_architecture_utilities_concurrention_signal Sygnał
* To proste narzędzie blokujące wykonanie danego wątku w sposób nie obciążający procesora do nadejścia sygnału zwolnienia blokady z innego wątku.
* @subsection engine_architecture_utilities_container Kontener
* @subsubsection engine_architecture_utilities_container_compile_time_map Słownik czasu kompilacji
* Jest to implementacja prostego słownika par klucz-wartość dowolnych typów (z ograniczeniami, patrz niżej) oparta całkowicie o szablony
* i rozwiązująca pytania o elementy w czasie kompilacji. Z powodu zaawansowanego meta-programowania słownik czasu kompilacji NIE może posiadać
* klucza opartego o wartość tekstową; Jest to bezpośrednie ograniczenie możliwości szablonów w C++.
* @subsubsection engine_architecture_utilities_container_concurrent_queue Równoległa kolejka
* Jest to prosta implementacja równoległej kolejki w modelu producent-konsument. Kolejka blokuje wykonanie wątku konsumenta (w sposób nie obciążający procesora)
* aż do momentu nadejścia nowego obiektu w wątku producenta. Kolejka realizuje model jeden-konsument wielu-producentów i nie wspiera dodatkowego mechanizmu
* przerywającego oczekiwanie (zwyczajowo można po prostu wysłać pusty wskaźnik bądź specjalnie oznaczony obiekt jeżeli potrzebne jest przerwanie)
* @subsubsection engine_architecture_utilities_container_sync Synchroniczna kolejka i synchroniczna mapa
* To kolejka i mapa ze standardowej biblioteki C++ opakowana w blokady równoległe do używania w silnie wielowątkowym środowisku.
* @subsection engine_architecture_utilities_debug Odpluskwiacz
* @subsubsection engine_architecture_utilities_debug_callstack Stos wywołań
* To prosty kontener przechowujący informacje o stosie wywołań w sposób wieloplatformowy. Wspiera możliwość uzupełnienia kolekcji o informacje odpluskwiacza,
* także na etapie post-mortem.
* @subsubsection engine_architecture_utilities_debug_remotery Remotery
* Otoczka C++ do biblioteki @ref dependency_remotery "remotery"
* @subsection engine_architecture_utilities_pattern Wzorce programistyczne
* @subsubsection engine_architecture_utilities_pattern_class_settings Ustawienia klasy
* To sposób konfigurowania klas w czasie kompilacji oparty o szablony i makra. Bazowo twórca danej klasy tworzy zestawy ustawień
* (np. "na Windows","wariant przenośny") a implementacja danej platformy może wybrać który z tych zestawów użyć w praktyce.
* @subsubsection engine_architecture_utilities_pattern_enum Enumeracje
* Proste makro do tworzenia własnych silnie typowanych enumeracji które automatycznie przeciążają funkcje engine::to_string() i engine::from_string()
* dla własnego typu.
* @subsubsection engine_architecture_utilities_pattern_factory Fabryka
* Poprawiona implementacja fabryki abstrakcyjnej z @ref dependency_di "Boost Dependency Injection". Zobacz rozdział
* @ref engine_problems_di "problem z wstrzykiwaniem zależności Boost"
* @subsubsection engine_architecture_utilities_pattern_flags Flagi
* Szablonowa klasa (podstawą szablonu jest silnie typowana enumeracja) tworząca flagi ustawień (tj. serię przełączników mogących być w danej chwili
* włączonych bądź wyłączonych). Flagi są zoptymalizowane przestrzennie i zawierają podstawowy zestaw metod ustawiających je i zdejmujących.
* @subsubsection engine_architecture_utilities_pattern_fourcc Identyfikator
* To implementacja idei czteroliterowych (32 bit) identyfikatorów używanych np. w koderach-dekoderach plików wideo.
* Implementacja Pixie Engine oparta jest o szablony i uwzględnia kodowanie, różną kolejność bajtów na różnych platformach oraz potrafi wygenerować
* identyfikator jako `constexpr` (co oznacza że może być np. wykorzystywany jako etykieta `case` w instrukcji `switch`).
* @subsubsection engine_architecture_utilities_pattern_provider Dostawca
* Zbiór podstawowych metod do tworzenia dostawców komponentów (zobacz poniżej).
* @subsubsection engine_architecture_utilities_pattern_writer Zapisywacz
* Klasa umożliwiająca transakcyjne operacje ("Wszystko albo żaden" oraz "Nie rozdzielaj w trakcie") na dowolnym strumieniu zdolnym do zapisywania sekwencji obiektów
* @subsection engine_architecture_utilities_platform Platformowe
* @subsubsection engine_architecture_utilities_platform_args Linia komend
* Klasa reprezentująca wieloplatformowy stan linii komend. Pozwala określić jakie przełączniki i argumenty są aktualnie ustawione,
* potrafi stworzyć swoją instancję z pojedynczego tekstu jak i argumentów `argc` i `argv` funkcji `main`. Pozwala także na manipulację tymi argumentami.
* @subsubsection engine_architecture_utilities_platform_debug Odpluskwiacze
* To zbiór (obecnie) trzech pomocniczych funkcji odpluskwiacza posiadających osobne implementacje na każdej platformie i wymagających globalnego dostępu
* (dlatego nie tworzą obiektów):
*  - engine::platform::trigger_breakpoint()
*  - engine::platform::canonize_debug_filename()
*  - engine::platform::dump_callstack()
* @subsubsection engine_architecture_utilities_platform_version Wersja
* To prosta klasa opisująca wersję danego obiektu/komponentu/aplikacji/programu. Posiada cztery człony (główna, pomniejsza, rewizja i kompilacja),
* i umożliwia porównywanie na różnych polach, a także konwersje z i do łańcucha znaków.
* @subsection engine_architecture_utilities_renderer Renderer
* @subsubsection engine_architecture_utilities_renderer_color Kolor
* Wieloplatformowa klasa zdolna przechować kolor w formacie RGB. 
* @subsection engine_architecture_utilities_security Bezpieczeństwo
* @subsubsection engine_architecture_utilities_security_crc32 CRC32
* Funkcje wyliczające skrót CRC32 dowolnych danych
* @subsection engine_architecture_utilities_test Testowe
* @subsubsection engine_architecture_utilities_test_print_to Drukuj do
* Pomocnicze funkcje do drukowania silnikowych @ref ustring_t "łańcuchów znaków" na potrzeby @ref dependency_googletest "Google Test Framework"
* @subsection engine_architecture_utilities_text Tekstowe
* @subsubsection engine_architecture_utilities_text_difference Różnica
* Oblicza @ref dependency_levenshtein "odległość Levenshteina" dwóch silnikowych @ref ustring_t "łańcuchów znaków"
* @subsubsection engine_architecture_utilities_text_expand Rozwinięcie
* Konwertuje serię dowolnych argumnetów w tablicę silnikowych @ref ustring_t "łańcuchów znaków". Wykorzystywane m.in. do formatowania tekstu
* (jako generator argumentów)
* @subsubsection engine_architecture_utilities_text_parser Parser
* Zbiór narzędzi do wykorzystania w parserze Spirit (część biblioteki @ref dependency_boost "boost")
* @subsubsection engine_architecture_utilities_text_ustring Łańcuch znaków
* Własna implementacja łańcucha znaków wspierająca w pełni kodowanie UTF-8 (natywne rozwiązanie C++ nie jest dostępne na wielu platformach).
* Zobacz dokumentację @ref engine::ustring_t
* 
* @section engine_architecture_core Rdzeń
* Oczywiście nie całą funkcjonalność można zamknąć w komponentach które mogą być włączone bądź wyłączone. Istnieje pewna część kodu która należy
* bezpośrednio do silnika i musi być zawsze dostępna. W ten sposób w naturalny sposób wykrystalizował się rdzeń Pixie Engine.
* 
* Przy decydowaniu która funkcjonalność należy do rdzenia a która do komponentów podjęto decyzję że w  przypadku wątpliwości będzie wybierany zawsze komponent.
* Rdzeń miał zawierać tylko absolutne minimum kodu, niezbędne do działania logiki silnika.
* 
* Po rozważaniach nad podziałem kodu na globalny (dotyczący wszystkich uruchomionych aplikacji) i lokalny (dotyczący konkretnej aplikacji) w sposób
* naturalny pojawił się także podział rdzenia. W ten sposób istnieje rdzeń dotyczący całego programu i osobne instancje rdzenia każdej aplikacji.
* 
* W chwili obecnej w skład rdzenia wchodzą wymienione w poniższych podrozdziałach elementy.
* @subsection engine_architecture_core_global Globalne
* @subsubsection engine_architecture_core_global_accountable Księgowi
* Jest to rodzajowy zbiór klas służących do notowania ilości i adresów zasobów silnika, z pominięciem plików danych (których byłoby zbyt dużo
* żeby implementacja była efektywna). W tej chwili rejestrowane są dwie rzeczy: wątki (wraz z zagospodarowaniem procesora) i same aplikacje
* (wraz z zajętością zasobów). Księgowi odnotowują akt stworzenia i zniszczenia zasobu i są w stanie przedstawić migawkę zasobów istniejących
* w aktualnej chwili programu.
* 
* @subsubsection engine_architecture_core_global_app Aplikacje
* Ten element rdzenia odpowiada za stworzenie nowych instancji aplikacji, uruchomienie wątków i obsługę głównej funkcji (`main`) każdej z aplikacji.
* O ile oryginalnie nie wchodziło to w skład tego elementu, dodano do niego także - inspirowany systemem operacyjnym Unix - system przerwań (odpowiednik sygnałów).
* Przerwanie aplikacji to np. informacja o tym że użytkownik poprosił aplikację o wyłączenie, albo nowa instancja właśnie próbuje być uruchomiona
* (niektóre aplikacje mogą nie chcieć wspierać więcej niż jednej instancji, aczkolwiek ten prosty scenariusz może być obsłużony w inny sposób - te bardziej
* rozbudowane wymagają przerwania). Ten element rdzenia zawiera także bootstrappera odpowiedzialnego za rozruch danej aplikacji
* (zobacz niżej @ref Igniter i Bootstrapper)
* w aktualnej chwili programu.
* 
* @subsubsection engine_architecture_core_global_console Konsola
* Jednym z najstarszych konceptów silnika gier jest pojęcie konsoli, zaproponowane jeszcze w latach 80 i będące standardem branży od lat 90.
* Konsole były tworzone na wzór terminali systemów operacyjnych i w przeciwieństwie do dzienników umożliwiały interakcję użytkownika z danymi tekstowymi,
* tym samym dając narzędzie do szybkiego zarządzania grą dla administratorów serwerów i bardziej technicznych graczy.
* 
* W Pixie Engine wewnętrznie konsola oparta jest o pętlę wiadomości, które posiadają możliwość konwersji do tekstu celem wyświetlenia ich użytkownikowi.
* W przeciwieństwie jednak do innych gier, konsola Pixie Engine trzyma wewnętrznie obiekty a nie tekst. Dzięki takiemu podejściu można np. przeformatować
* dane w zależności od potrzeb, lub uzyskać dodatkowe informacje na temat każdej linijki.
* 
* Jeżeli chodzi o parsowanie wejścia, wykorzystana jest biblioteka @ref Boost Spirit z autorską implementacją systemu na wzór intellisense
* poprawiającą błędy wejścia i posiadająca funkcjonalność autouzupełniania poleceń (zobacz https://stackoverflow.com/a/47383910/151150 )
* 
* @subsubsection engine_architecture_core_global_data Dane
* To zbiór klas stanowiący bazę @ref wirtualnego systemu plików. W tej chwili w skład rdzenia należą: strumienie wejścia/wyjścia,
* dostawcy strumieni, skanery i archiwa.
* 
* @subsubsection engine_architecture_core_global_manifest Manifest
* Manifest obejmuje metadane platformy na której uruchomiony jest Pixie Engine jak i samego silnika. W chwili obecnej obejmuje on:
*  - Informacje o systemie operacyjnym
*  - Manifest silnika (zobacz rozdział @ref manifest silnika)
*  - Wersję silnika
* @subsubsection engine_architecture_core_global_messenger Komunikator
* @todo Dokończyć sekcję rdzenia

*/