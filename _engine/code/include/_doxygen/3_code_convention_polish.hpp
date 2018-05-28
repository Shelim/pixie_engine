 /**
 * \~Polish @page code_convention Konwencje kodu
 * @tableofcontents
 * @section code_convention_intro Wprowadzenie
 * Jedną z pierwszych decyzji organizacyjnych jaką muszą podjąć autorzy niemal każdego przedsięwzięcia programistycznego jest
 * ustandaryzowanie procesu wytwarzania danego oprogramowania. W ramach procesu decyzyjnego należy przyjąć zgodę co do procedur takich jak:
 *      - Wspólna konwencja nazw. W ten sposób programiści dołączający do danego projektu po jego rozpoczęciu mogą łatwiej wdrożyć się w proces tworzenia
 *      - Wspólna konwencja kodu. Składa się na to całość organizacji wizualnej która nie powinna mieć wpływu na logikę aplikacji. Mowa o takich elementach jak podział wyrównania, umieszczenie nawiasów czy konwencja deklarowania typów.
 *      - Układ katalogów projektu
 *      - Procedury rozwiązywania potencjalnych problemów
 * 
 * @section code_convention_naming Konwencje nazw
 * Poniżej znajdują się przyjęte przez autora konwencje nazw Pixie Engine.
 * 
 *  1. Wszystkie nazwy są pisane małymi literami alfabetu łacińskiego z podkreślnikiem jako separatorem (np. `hello_world`)
 *  2. Podstawowe wzorce nazw:
 *      - `obiekt_właściwość`, nie na odwrót (np. `game_name`)
 *      - `akcja_obiekt_właściwość` (np. `new_window_handle`)
 *      - Więcej niż jedno słowo może być rozdzielane dodatkowym podkreślnikiem (np. `new_game_window_handle`)
 *      - Wszystkie typy mają przyrostek `_t` (np. `game_handle_t`)
 *      - Nie używa się notacji węgierskiej!
 *  3. Ustandaryzowane nazwy:
 *      - `asset` - Każda nie wpisana na stałe do kodu źródłowego dana która może być modyfikowana. Zobacz `content`
 *      - `build_number` - Numer zwiększany za każdym przebiegiem kompilacji (współdzielony przez wszystkie typy kompilacji)
 *      - `changelog` - Katalog zawierający meta-informacje o zmianach we wszystkich poprzednich kompilacjach (wykorzystywany przez automatycznego aktualizera)
 *      - `component` - Pojedynczy element aplikacji (kodu źródłowego). Komponenty dzielą się na globalne (współdzielone między aplikacjami) i lokalne (należące do konkretnej instancji aplikacji). Wszystkie komponenty mogą być włączone i wyłączonane poprzez zastąpienie ich funkcjonalności wariantem pustym (`dummy`) bądź makietą (`mockup`) - na potrzeby testów
 *      - `concurrention` - Kod dedykowany wielowątkowości
 *      - `config` - Ustawienia czasu wykonania (nie mylić z `settings`) które zwykle przechowuje się w sposób zależny od platformy
 *      - `console` - Wewnętrzna konsola wypisująca dane tekstowe i reagująca na wprowadzanie danych tekstowych
 *      - `container` - Zwykle standardowy kontener Biblioteki Szablonów C++ lub dodatkowy zdefiniowany bezpośrednio przez Pixie Engine
 *      - `content` - Zbiór wszystkich `asset`
 *      - `data` - Fragment informacji silnika. Wspólny termin na `assets`, pliki `config`, profile użytkownika, zapisane stany gry, itd.
 *      - `debug` - Konfiguracja wyjściowa która powinna być uruchamiana tylko przez developera. Także każdy kawałek kodu który ma być wykorzystywany tylko do debugowania.
 *      - `def` - Makra kontrolujące różne definicje wewnątrz całego silnika. Zwykle umożliwiają rozszerzanie cech silnika przez programistę gry bez potrzeby manipulowania we wnętrzu samego silnika. Ich konstrukcja jest silnie zoptymalizowana (makra są interpretowane i wykonywane na etapie kompilacji).
 *      - `dependency` - Cudzy (zewnętrzny) kawałek kodu lub program wykorzystany wewnątrz Pixie Engine
 *      - `environment` - Specyfikacja maszyny na której uruchomiony jest Pixie Engine (najczęściej urządzenie gracza)
 *      - `final` - Konfiguracja wyjściowa przeznaczona do dystrybucji gry. Poza kompilacją aplikacji zbuduje także informacje automatycznej aktualizacji i przygotuje pliki do wgrania do publicznego repozytorium.
 *      - `hybrid` - Konfiguracja wyjściowa przeznaczona dla testerów. Jest szybsza niż `debug` i nie zawiera symboli debuggowania. Może być używana do publicznej bety gry.
 *      - `logger` - Sposób zachowania informacji (zwykle do pliku tekstowego) w celu sprawdzania błędów na urządzeniach klienckich
 *      - `manifest` - metadane danego elementu Pixie Engine
 *      - `module` - Duża, logicznie powiązana paczka `asset`. Każda uruchomiona gra składa się z jednego lub więcej `module`. Przykłady: duży rozdział gry, konwersja pełnej modyfikacji. Zobacz `submodule`
 *      - `platform` - Docelowa maszyna na której uruchomiony jest Pixie Engine. Także kod przeznaczony do uruchomienia na konkretnym typie takiej maszyny.
 *      - `private` - Rzecz do wykorzystania tylko przez programistę i nie mająca być dołączona ani do repozytorium ani do konfiguracji wyjściowej.
 *      - `provider` - Prosta klasa dostarczająca niewielką ilość informacji lub logiki innej klasie (najczęściej `component`). Przykład: dostawca ścieżek systemowych, dostawca wyjścia dziennika
 *      - `release` - Wspólna nazwa dla konfiguracji wyjścia `hybrid` i `final`. Zobacz @ref build_configuration "rodzaje konfiguracji wyjściowych"
 *      - `settings` - Ustawienia czasu kompilacji. Są zamrożone w aplikacji i muszą być dostępne dla `bootstrappera` na etapie kompilacji programu
 *      - `submodule` - Mniejsza, logicznie powiązana paczka `asset`. Każda uruchomiona gra może mieć dowolną ilość aktywnych podmodułów. Przykłady: DLC lub mini-modyfikacje fanowskie; Zobacz: `module`
 *      - `terminal` - Zwykle konsola na docelowej platformie, chyba że platforma nie posiada konsoli
 *      - `utility` - Proste klasy i funkcje do wykorzystania w reszcie silnika. Nie zależą od komponentów i innych elementów wyższego poziomu
 *
 * @section code_convention_code Konwencje kodu
 * @subsection code_convention_code_desc Opis
 * Konwencja kodu przyjęta w Pixie Engine opiera się na zasadzie preferowania czytelności ponad ograniczaniem długości. W ten sposób
 * uzyskano poprawę jakości (czytelny kod jest np. łatwiej debuggować) kosztem nieznacznego wydłużenia ilości kodu. Konwencje które przyjęto
 * w Pixie Engine można podzielić na:
 * @subsubsection code_convention_code_desc_struct Strukturalne
 * Każda klamra w osobnej linii. Od tej zasady dozwolony jest wyjątek, jeżeli funkcja bądź metoda jest jednolinijkowa, wtedy cały zapis może być umieszczony w jednej linii.
 * @code{.cpp}
 * void foo(int a)
 * {
 *     int ret = a + 5;
 *     return ret;
 * }
 * void baz() { return 5; }
 * @endcode
 * Składniki dostępowe (public/protected/internal) w osobnych liniach wyrównane do klasy
 * @code{.cpp}
 * class foo_t
 * {
 * public:
 *     void bar();
 * private:
 *     int baz;
 * };
 * @endcode
 * Wszystkie typy mają przyrostek `_t`
 * @code{.cpp}
 * class foo_t
 * {
 * 
 * };
 * struct bar_t
 * {
 * 
 * };
 * enum baz_t
 * {
 * 
 * };
 * typedef int id_t;
 * @endcode
 * Argumenty szablonów są definiowane małymi literami (z przyrostkiem `_t`) jako `class` a nie `typename`
 * @code{.cpp}
 * template <class arg_t> class foo_t
 * {
 * private:
 *     arg_t arg;
 * };
 * @endcode
 * Akcesory dla własności `foo` nazywają się `get_foo` oraz `set_foo`. Argument tego drugiego nosi zawsze nazwę `val`.
 * 
 * Getter jest zawsze metodą typu `const`. Setter nie używa `this`.
 * @code{.cpp}
 * class foo_t
 * {
 * public:
 *     int get_bar() const
 *     {
 *         return bar;
 *     }
 *     void set_bar(int val)
 *     {
 *         bar = val;
 *     }
 * private:
 *     int bar;
 * }
 * @endcode
 * Nie używa się konstrukcji `using namespace`. Wszystkie przestrzenie nazw muszą być podane bezpośrednio przed użyciem danego konstruktu
 * @code{.cpp}
 * int foo(std::vector<engine::id_t> & ids)
 * {
 *     ids.push_back(engine::make_id("test"));
 * }
 * @endcode
 * Nie używa się rzutowania w stylu C. Dozwolone są wyłącznie `static_cast` oraz `reinterpret_cast`.
 * @code{.cpp}
 * child_t * get(parent_t * parent)
 * {
 *     return static_cast<child_t*>(parent);
 * }
 * std::uintptr_t foo(obj_t * ptr)
 * {
 *     return reinterpret_cast<std::uintptr_t>(ptr);
 * } 
 * @endcode
 * Nie używa się wyrażeń `NULL`. Zamiast nich dozwolone są wyłącznie `nullptr`
 * @code{.cpp}
 * obj_t * query()
 * {
 *     return nullptr;
 * }
 * @endcode
 * Puste funkcje wymagają komentarza (wyjątek: konstruktory i destruktory)
 * @code{.cpp}
 * class foo_t
 * {
 * public:
 *     foo_t()
 *     {
 *     }
 *     void do_nothing()
 *     {
 *         // This function does nothing.
 *     }
 * }
 * @endcode
 * 
 * 
 * @subsubsection code_convention_code_desc_semantic Semantyczne
 * Gdy tylko to możliwe klasy powinny korzystać z mechanizmu wstrzykiwania zależności, ale bez wykorzystania antywzorca lokalizator usług (ukrywa on informacje na temat wymgań danej klasy, np. na potrzeby testów)
 * @code{.cpp}
 * class foo_t
 * {
 * public:
 *     foo_t(std::shared_ptr<bar_t> bar, std::unique_ptr<baz_t> baz) :
 *              bar(bar), baz(std::move(baz))
 *     {
 *     }
 * private:
 *     std::shared_ptr<bar_t> bar;
 *     std::unique_ptr<baz_t> baz;
 * };
 * @endcode
 * W miarę możności definicje dłuższych funkcji powinny się znaleźć w pliku źródłowym
 * @code{.cpp}
 * class foo_t
 * {
 * public:
 *     void long_operation();
 * };
 * @endcode
 * Nie dopuszcza się wirtualności z implementacją - metoda może być albo niewirtualna albo czysto wirtualna. Wyjątek: destruktor.
 * @code{.cpp}
 * class foo_t
 * {
 * public:
 *     virtual ~foo_t() {}
 *     virtual int bar() = 0;
 *     virtual void baz() = 0;
 * };
 * @endcode
 * Jeżeli zachodzi potrzeba stworzenia metody wirtualnej z implementacją należy posłużyć się następującą konstrukcją: umieścić kod w publicznej
 * metodzie niewirtualnej `foo` która wywołuje prywatną metodę czysto wirtualną z przyrostkiem `_local` - w tym przypadku `foo_local`. 
 * 
 * Używając takiej konstrukcji mamy pewność że kod podstawowej funkcji `foo` zostanie zawsze wykonany - rozwiązuje to problem sytuacji w której
 * programista klasy pochodnej musi się zastanowić czy powinien wywołać funkcję bazową czy też nie.
 * @code{.cpp}
 * class foo_t
 * {
 * public:
 *     void bar(int a)
 *     {
 *         a += 5;
 *         return bar_local(a);
 *     }
 * private:
 *     virtual int bar_local(int a) = 0;
 * };
 * @endcode
 * Klasy dziedziczące nie używają słowa kluczowego `final`. Generuje to błąd kompilacji przy wstrzykiwaniu zależności. Klasy zawsze dziedziczą jako `public`
 * @code{.cpp}
 * class foo_t
 * {
 * public:
 *     virtual ~foo_t() {}
 * };
 * class bar_t : public foo_t
 * {
 * };
 * @endcode
 * Metody wirtualne są dziedziczone ze słowem kluczowym `final`. Nie wolno używać ponownie słowa kluczowego `virtual` (generuje to niepotrzebny wpis `vtable` danej klasy)
 * @code{.cpp}
 * class foo_t
 * {
 * public:
 *     virtual ~foo_t() {}
 *     virtual int bar() = 0;
 * };
 * class baz_t : public foo_t
 * {
 * public:
 *     int bar() final
 *     {
 *          return 5;
 *     }
 * };
 * @endcode
 * Nie wolno używać zmiennych statycznych ani globalnych (jedyny wyjątek: obiekt klasy `program_t` na niektórych platformach)
 * @code{.cpp}
 * void foo()
 * {
 *     int a = 5;
 *     return a;
 * }
 * @endcode
 * Klasa nie może mieć publicznych własności (enkapsulacja)
 * @code{.cpp}
 * class foo_t
 * {
 * public:
 *     int get_bar() const
 *     {
 *         return bar;
 *     }
 *     void set_bar(int val)
 *     {
 *         bar = val;
 *     }
 * private:
 *     int bar;
 * };
 * @endcode
 * Jeżeli zachodzi potrzeba zdefiniowania typu klasy pochodnej w klasie bazowej, należy użyć enumeracji i przekazać go w parametrze konstruktora klasy bazowej.
 * @code{.cpp}
 * class foo_t
 * {
 * public:
 *     virtual ~foo_t() { }
 *     enum class type_t
 *     {
 *         bar,
 *         count
 *     }
 *     type_t get_type() const
 *     {
 *         return type;
 *     }
 * protected:
 *     foo_t(type_t type) : type(type)
 *     {
 *     }
 * private:
 *     type_t type;
 * };
 * class bar_t : public foo_t
 * {
 * public:
 *     bar_t() : foo_t(type_t::bar)
 *     {
 *     }
 * };
 * @endcode
 * Należy używać wyłącznie silnie typowanych enumeracji z ostatnim polem o nazwie `count`. Nie wolno definiować własnych wartości liczbowych dla żadnego z pól enumeracji.
 * @code{.cpp}
 * enum class foo_t
 * {
 *     bar,
 *     baz,
 *     count
 * };
 * @endcode
 * Jeżeli zachodzi potrzeba stworzenia flag (przełączników) należy stworzyć silnie typowaną enumerację o nazwie `flag_t` i użyć jej jako typu szablonowego klasy `flags_t`.
 * @code{.cpp}
 * class foo_t
 * {
 * public:
 *     enum class flag_t
 *     {
 *         bar,
 *         baz,
 *         count
 *     }
 * private:
 *     flags_t<flag_t> flags;
 * };
 * @endcode
 * 
 * @section code_convention_folders Układ katalogów
 * W Pixie Engine przyjęto następujące założenia co do nazw katalogów:
 *     - Katalogi których nazwy zaczynają się od `!` zawierają pliki tymczasowe systemu kompilacji i poza kompilacją mogą być bezpiecznie usunięte
 *     - Katalogi których nazwy zaczynają się od `_` zawierają pliki wewnętrzne silnika i nie powinny być modyfikowane przez twórcę gry
 *     - Katalogi których nazwy zaczynają się od `.` zawierają ustawienia lokalne edytora i jako takie nie powinny być wersjonowane w systemie Git
 * 
 * Katalog główny Pixie Engine po wykonaniu komendy `git clone` zawiera następujące foldery:
 *     - `_command` zawierający skrypty danej platformy do wydawania poleceń silnikowi (np. stworzenie nowego projektu, ściągnięcie zależności)
 *     - `_engine` zawierający pełen kod źródłowy i wszystkie zależności silnika
 * 
 * W katalogu `_command/windows` znajdują się polecenia w formie plików skryptowych Windows Batch. Aby uruchomić polecenie należy kliknąć dwukrotnie na pliku. Wszystkie niezbędne parametry będą wprowadzane interaktywnie podczas działania skryptu. Parametry będą weryfikowane na poprawność a w przypadku wykrycia problemu, pytanie o dany parametr zostanie powtórzone
 *     - `download_dependencies.bat` to polecenie ściągnięcia zależności. Ponieważ zależności liczą sobie 4,5 gb nie są przechowywane bezpośrednio w repozytorium Git. To polecenie należy wykonać dokładnie raz po wykonaniu komendy `git clone`. Dopóki zależności nie zostaną ściągnięte, pozostałe polecenia nie zadziałają i powiadomią o konieczności wykonania tego polecania
 *     - `execute_tests.bat` to polecenie wykonania testów. Stworzy on testowy projekt, skompiluje go, wykona testy, zapisze ich rezultaty i usunie projekt. Rezultat testów znajdzie się w katalogu `!test_results` w głównym folderze silnika, a raport w pliku `test_result.html` w tym samym folderze
 *     - `new_project.bat` to polecenie stworzenia nowej gry w oparciu o Pixie Engine. Po wprowadzeniu podstawowych parametrów takich jak np. nazwa projektu, przygotuje on nowy katalog w folderze głównym Pixie Engine o nazwie równej identyfikatorowi gry. Wszelka praca nad daną grą powinna być prowadzona w tym katalogu. Pixie Engine dopuszcza równoległe tworzenie kilku gier w oparciu o jeden katalogi główny
 *     - `view_documentation.bat` to polecenie przygotowuje i wyświetla dokumentację doxygen Pixie Engine
 *
 * Katalog `_engine` zawiera następujące podkatalogi:
 *     - `!build` zawierający tymczasowe pliki kompilacji
 *     - `!docs` zawierający wygenerowaną dokumentację. Skasowanie tego katalogu wymusi ponowne wygenerowanie dokumentacji
 *     - `.vscode` zawierający ustawienia VS Code projektu
 *     - `build` zawierający skrypty kompilacji Pixie Engine na zadaną platformę. Katalog ten zawiera też skrypty doxygen i Apache Ant
 *     - `code` zawiera pełen kod źródłowy silnika
 *     - `content` zawiera pliki danych silnika które muszą być dołączone do końcowej wersji programu. Pliki te są automatycznie kopiowane do folderu wyjścia kompilacji
 *     - `content_prepare` zawiera pliki źródłowe z których przygotowana została zawartość katalogu `content`
 *     - `dependency` zawiera wszystkie zależności silnika
 *     - `manifest` zawiera manifest silnika - pliki meta kompilacji takie jak logo dokumentacji, logo silnia, pliki nagłówków doxygen
 *     - `private` to folder zwykle nie wersjonowany na systemie git który zawiera pliki wewnętrzne używane przez danego programistę
 * 
 * Katalog projektu ma natomiast następujące podkatalogi:
 *     - `!build` zawierający tymczasowe pliki kompilacji
 *     - `!output` zawierający rezultat ostatniej kompilacji podzielonej wg typu
 *     - `.vscode` zawierający ustawienia VS Code projektu
 *     - `.vscode-user` zawierający ustawienia użytkownika VS Code
 *     - `_changelog` to folder zawierający pliki historii samej gry takie jak plik zawierający numer kompilacji oraz listy zmian od poprzedniej kompilacji.
 *     - `_command` to folder analogiczny do folderu z katalogu głównego, ale którego komendy dotyczą konkretnej gry. Znajdują się tam skrypty wydające rozkaz kompilacji, testowania, budowania instalatora, etc. Można je wywoływać nie uruchamiając edytora VS Code (np. mogą zostać podłączone do zewnętrznego systemu). Większość z tych komend jest nieinteraktywna
 *     - `_ready` to folder przechowujący finałowe kompilacje projektu oraz ewentualne mapy linkera wypuszczonych wersji
 *     - `code` zawierający kod źródłowy danej gry i wszystkich aplikacji z nią związanych (jak np. edytora, albo startera)
 *     - `content` zawierający wszystkie pliki danych gry takie jak grafika, dźwięki, etc. Cała jego zawartość jest automatycznie kopiowana do folderu wyjścia kompilacji
 *     - `content_prepare` zawiera pliki źródłowe z których artysta przygotowuje zawartość katalogu `content`
 *     - `docs` początkowo pusty, ten folder zawiera wewnętrzną dokumentację danego projektu gry
 *     - `manifest` zawierający manifest gry - pliki meta kompilacji takie jak logo i ikonka gry, licencja wyświetlana podczas instalacji, pliki xml opisujące informacje o grze uwzględniające nazwę, domyślną ścieżkę instalacji, etc.
 *     - `press` również początkowo pusty, folder ten zawiera wszystkie materiały promocyjne które zostały bądź zostaną opublikowane dla danej gry
 *     - `private` to folder zwykle nie wersjonowany na systemie git który zawiera pliki wewnętrzne używane przez danego programistę
 */