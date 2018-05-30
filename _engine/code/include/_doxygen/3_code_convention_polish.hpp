 /**
 * \~Polish @page code_convention Konwencje kodu
 * @tableofcontents
 * @section code_convention_intro Wprowadzenie
 * Jedną z&nbsp;pierwszych decyzji organizacyjnych jaką muszą podjąć autorzy niemal każdego przedsięwzięcia programistycznego jest
 * ustandaryzowanie procesu wytwarzania danego oprogramowania. W&nbsp;ramach procesu decyzyjnego należy przyjąć zgodę co do&nbsp;procedur takich jak:
 *      - Wspólna konwencja nazw. W&nbsp;ten sposób programiści dołączający do&nbsp;danego projektu po&nbsp;jego rozpoczęciu mogą łatwiej wdrożyć się w&nbsp;proces tworzenia
 *      - Wspólna konwencja kodu. Składa się na&nbsp;to całość organizacji wizualnej która nie powinna mieć wpływu na&nbsp;logikę aplikacji. Mowa o&nbsp;takich elementach jak podział wyrównania, umieszczenie nawiasów czy konwencja deklarowania typów.
 *      - Układ katalogów projektu
 *      - Procedury rozwiązywania potencjalnych problemów
 * 
 * @section code_convention_naming Konwencje nazw
 * Poniżej znajdują się przyjęte przez autora konwencje nazw Pixie Engine.
 * 
 *  1. Wszystkie nazwy są pisane małymi literami alfabetu łacińskiego z&nbsp;podkreślnikiem jako separatorem (np. `hello_world`)
 *  2. Podstawowe wzorce nazw:
 *      - `obiekt_właściwość`, nie na&nbsp;odwrót (np. `game_name`)
 *      - `akcja_obiekt_właściwość` (np. `new_window_handle`)
 *      - Więcej niż jedno słowo może być rozdzielane dodatkowym podkreślnikiem (np. `new_game_window_handle`)
 *      - Wszystkie typy mają przyrostek `_t` (np. `game_handle_t`)
 *      - Nie używa się notacji węgierskiej!
 *  3. Ustandaryzowane nazwy:
 *      - `asset` - Każda nie wpisana na&nbsp;stałe do&nbsp;kodu źródłowego dana która może być modyfikowana. Zobacz `content`
 *      - `build_number` - Numer zwiększany za&nbsp;każdym przebiegiem kompilacji (współdzielony przez wszystkie typy kompilacji)
 *      - `changelog` - Katalog zawierający meta-informacje o&nbsp;zmianach we&nbsp;wszystkich poprzednich kompilacjach (wykorzystywany przez automatycznego aktualizera)
 *      - `component` - Pojedynczy element aplikacji (kodu źródłowego). Komponenty dzielą się na&nbsp;globalne (współdzielone między aplikacjami) i&nbsp;lokalne (należące do&nbsp;konkretnej instancji aplikacji). Wszystkie komponenty mogą być włączone i&nbsp;wyłączonane poprzez zastąpienie ich funkcjonalności wariantem pustym (`dummy`) bądź makietą (`mockup`) - na&nbsp;potrzeby testów
 *      - `concurrention` - Kod dedykowany wielowątkowości
 *      - `config` - Ustawienia czasu wykonania (nie mylić z&nbsp;`settings`) które zwykle przechowuje się w&nbsp;sposób zależny od&nbsp;platformy
 *      - `console` - Wewnętrzna konsola wypisująca dane tekstowe i&nbsp;reagująca na&nbsp;wprowadzanie danych tekstowych
 *      - `container` - Zwykle standardowy kontener Biblioteki Szablonów C++ lub dodatkowy zdefiniowany bezpośrednio przez Pixie Engine
 *      - `content` - Zbiór wszystkich `asset`
 *      - `data` - Fragment informacji silnika. Wspólny termin na&nbsp;`assets`, pliki `config`, profile użytkownika, zapisane stany gry, itd.
 *      - `debug` - Konfiguracja wyjściowa która powinna być uruchamiana tylko przez developera. Także każdy kawałek kodu który ma być wykorzystywany tylko do&nbsp;debugowania.
 *      - `def` - Makra kontrolujące różne definicje wewnątrz całego silnika. Zwykle umożliwiają rozszerzanie cech silnika przez programistę gry bez potrzeby manipulowania we&nbsp;wnętrzu samego silnika. Ich konstrukcja jest silnie zoptymalizowana (makra są interpretowane i&nbsp;wykonywane na&nbsp;etapie kompilacji).
 *      - `dependency` - Cudzy (zewnętrzny) kawałek kodu lub program wykorzystany wewnątrz Pixie Engine
 *      - `environment` - Specyfikacja maszyny na&nbsp;której uruchomiony jest Pixie Engine (najczęściej urządzenie gracza)
 *      - `final` - Konfiguracja wyjściowa przeznaczona do&nbsp;dystrybucji gry. Poza kompilacją aplikacji zbuduje także informacje automatycznej aktualizacji i&nbsp;przygotuje pliki do&nbsp;wgrania do&nbsp;publicznego repozytorium.
 *      - `hybrid` - Konfiguracja wyjściowa przeznaczona dla testerów. Jest szybsza niż `debug` i&nbsp;nie zawiera symboli debuggowania. Może być używana do&nbsp;publicznej bety gry.
 *      - `logger` - Sposób zachowania informacji (zwykle do&nbsp;pliku tekstowego) w&nbsp;celu sprawdzania błędów na&nbsp;urządzeniach klienckich
 *      - `manifest` - metadane danego elementu Pixie Engine
 *      - `module` - Duża, logicznie powiązana paczka `asset`. Każda uruchomiona gra składa się z&nbsp;jednego lub więcej `module`. Przykłady: duży rozdział gry, konwersja pełnej modyfikacji. Zobacz `submodule`
 *      - `platform` - Docelowa maszyna na&nbsp;której uruchomiony jest Pixie Engine. Także kod przeznaczony do&nbsp;uruchomienia na&nbsp;konkretnym typie takiej maszyny.
 *      - `private` - Rzecz do&nbsp;wykorzystania tylko przez programistę i&nbsp;nie mająca być dołączona ani do&nbsp;repozytorium ani do&nbsp;konfiguracji wyjściowej.
 *      - `provider` - Prosta klasa dostarczająca niewielką ilość informacji lub logiki innej klasie (najczęściej `component`). Przykład: dostawca ścieżek systemowych, dostawca wyjścia dziennika
 *      - `release` - Wspólna nazwa dla konfiguracji wyjścia `hybrid` i&nbsp;`final`. Zobacz @ref build_configuration "rodzaje konfiguracji wyjściowych"
 *      - `settings` - Ustawienia czasu kompilacji. Są zamrożone w&nbsp;aplikacji i&nbsp;muszą być dostępne dla `bootstrappera` na&nbsp;etapie kompilacji programu
 *      - `submodule` - Mniejsza, logicznie powiązana paczka `asset`. Każda uruchomiona gra może mieć dowolną ilość aktywnych podmodułów. Przykłady: DLC lub mini-modyfikacje fanowskie; Zobacz: `module`
 *      - `terminal` - Zwykle konsola na&nbsp;docelowej platformie, chyba że&nbsp;platforma nie posiada konsoli
 *      - `utility` - Proste klasy i&nbsp;funkcje do&nbsp;wykorzystania w&nbsp;reszcie silnika. Nie zależą od&nbsp;komponentów i&nbsp;innych elementów wyższego poziomu
 *
 * @section code_convention_code Konwencje kodu
 * @subsection code_convention_code_desc Opis
 * Konwencja kodu przyjęta w&nbsp;Pixie Engine opiera się na&nbsp;zasadzie preferowania czytelności ponad ograniczaniem długości. W&nbsp;ten sposób
 * uzyskano poprawę jakości (czytelny kod jest np. łatwiej debuggować) kosztem nieznacznego wydłużenia ilości kodu. Konwencje które przyjęto
 * w&nbsp;Pixie Engine można podzielić na:
 * @subsubsection code_convention_code_desc_struct Strukturalne
 * Każda klamra w&nbsp;osobnej linii. Od&nbsp;tej zasady dozwolony jest wyjątek, jeżeli funkcja bądź metoda jest jednolinijkowa, wtedy cały zapis może być umieszczony w&nbsp;jednej linii.
 * @code{.cpp}
 * void foo(int a)
 * {
 *     int ret = a + 5;
 *     return ret;
 * }
 * void baz() { return 5; }
 * @endcode
 * Składniki dostępowe (public/protected/internal) w&nbsp;osobnych liniach wyrównane do&nbsp;klasy
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
 * Argumenty szablonów są definiowane małymi literami (z przyrostkiem `_t`) jako `class` a&nbsp;nie `typename`
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
 * Nie używa się rzutowania w&nbsp;stylu C. Dozwolone są wyłącznie `static_cast` oraz `reinterpret_cast`.
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
 * Puste funkcje wymagają komentarza (wyjątek: konstruktory i&nbsp;destruktory)
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
 * Gdy tylko to możliwe klasy powinny korzystać z&nbsp;mechanizmu wstrzykiwania zależności, ale bez wykorzystania antywzorca lokalizator usług (ukrywa on informacje na&nbsp;temat wymgań danej klasy, np. na&nbsp;potrzeby testów)
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
 * W&nbsp;miarę możności definicje dłuższych funkcji powinny się znaleźć w&nbsp;pliku źródłowym
 * @code{.cpp}
 * class foo_t
 * {
 * public:
 *     void long_operation();
 * };
 * @endcode
 * Nie dopuszcza się wirtualności z&nbsp;implementacją - metoda może być albo niewirtualna albo czysto wirtualna. Wyjątek: destruktor.
 * @code{.cpp}
 * class foo_t
 * {
 * public:
 *     virtual ~foo_t() {}
 *     virtual int bar() = 0;
 *     virtual void baz() = 0;
 * };
 * @endcode
 * Jeżeli zachodzi potrzeba stworzenia metody wirtualnej z&nbsp;implementacją należy posłużyć się następującą konstrukcją: umieścić kod w&nbsp;publicznej
 * metodzie niewirtualnej `foo` która wywołuje prywatną metodę czysto wirtualną z&nbsp;przyrostkiem `_local` - w&nbsp;tym przypadku `foo_local`. 
 * 
 * Używając takiej konstrukcji mamy pewność że&nbsp;kod podstawowej funkcji `foo` zostanie zawsze wykonany - rozwiązuje to problem sytuacji w&nbsp;której
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
 * Metody wirtualne są dziedziczone ze&nbsp;słowem kluczowym `final`. Nie wolno używać ponownie słowa kluczowego `virtual` (generuje to niepotrzebny wpis `vtable` danej klasy)
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
 * Nie wolno używać zmiennych statycznych ani globalnych (jedyny wyjątek: obiekt klasy `program_t` na&nbsp;niektórych platformach)
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
 * Jeżeli zachodzi potrzeba zdefiniowania typu klasy pochodnej w&nbsp;klasie bazowej, należy użyć enumeracji i&nbsp;przekazać go w&nbsp;parametrze konstruktora klasy bazowej.
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
 * Należy używać wyłącznie silnie typowanych enumeracji z&nbsp;ostatnim polem o&nbsp;nazwie `count`. Nie wolno definiować własnych wartości liczbowych dla żadnego z&nbsp;pól enumeracji.
 * @code{.cpp}
 * enum class foo_t
 * {
 *     bar,
 *     baz,
 *     count
 * };
 * @endcode
 * Jeżeli zachodzi potrzeba stworzenia flag (przełączników) należy stworzyć silnie typowaną enumerację o&nbsp;nazwie `flag_t` i&nbsp;użyć jej jako typu szablonowego klasy `flags_t`.
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
 * W&nbsp;Pixie Engine przyjęto następujące założenia co do&nbsp;nazw katalogów:
 *     - Katalogi których nazwy zaczynają się od&nbsp;`!` zawierają pliki tymczasowe systemu kompilacji i&nbsp;poza kompilacją mogą być bezpiecznie usunięte
 *     - Katalogi których nazwy zaczynają się od&nbsp;`_` zawierają pliki wewnętrzne silnika i&nbsp;nie powinny być modyfikowane przez twórcę gry
 *     - Katalogi których nazwy zaczynają się od&nbsp;`.` zawierają ustawienia lokalne edytora i&nbsp;jako takie nie powinny być wersjonowane w&nbsp;systemie Git
 * 
 * Katalog główny Pixie Engine po&nbsp;wykonaniu komendy `git clone` zawiera następujące foldery:
 *     - `_command` zawierający skrypty danej platformy do&nbsp;wydawania poleceń silnikowi (np. stworzenie nowego projektu, ściągnięcie zależności)
 *     - `_engine` zawierający pełen kod źródłowy i&nbsp;wszystkie zależności silnika
 * 
 * W&nbsp;katalogu `_command/windows` znajdują się polecenia w&nbsp;formie plików skryptowych Windows Batch. Aby uruchomić polecenie należy kliknąć dwukrotnie na&nbsp;pliku. Wszystkie niezbędne parametry będą wprowadzane interaktywnie podczas działania skryptu. Parametry będą weryfikowane na&nbsp;poprawność a&nbsp;w przypadku wykrycia problemu, pytanie o&nbsp;dany parametr zostanie powtórzone
 *     - `download_dependencies.bat` to polecenie ściągnięcia zależności. Ponieważ zależności liczą sobie 4,5 gb nie są przechowywane bezpośrednio w&nbsp;repozytorium Git. To polecenie należy wykonać dokładnie raz po&nbsp;wykonaniu komendy `git clone`. Dopóki zależności nie zostaną ściągnięte, pozostałe polecenia nie zadziałają i&nbsp;powiadomią o&nbsp;konieczności wykonania tego polecania
 *     - `execute_tests.bat` to polecenie wykonania testów. Stworzy on testowy projekt, skompiluje go, wykona testy, zapisze ich rezultaty i&nbsp;usunie projekt. Rezultat testów znajdzie się w&nbsp;katalogu `!test_results` w&nbsp;głównym folderze silnika, a&nbsp;raport w&nbsp;pliku `test_result.html` w&nbsp;tym samym folderze
 *     - `new_project.bat` to polecenie stworzenia nowej gry w&nbsp;oparciu o&nbsp;Pixie Engine. Po&nbsp;wprowadzeniu podstawowych parametrów takich jak np. nazwa projektu, przygotuje on nowy katalog w&nbsp;folderze głównym Pixie Engine o&nbsp;nazwie równej identyfikatorowi gry. Wszelka praca nad daną grą powinna być prowadzona w&nbsp;tym katalogu. Pixie Engine dopuszcza równoległe tworzenie kilku gier w&nbsp;oparciu o&nbsp;jeden katalogi główny
 *     - `view_documentation.bat` to polecenie przygotowuje i&nbsp;wyświetla dokumentację doxygen Pixie Engine
 *
 * Katalog `_engine` zawiera następujące podkatalogi:
 *     - `!build` zawierający tymczasowe pliki kompilacji
 *     - `!docs` zawierający wygenerowaną dokumentację. Skasowanie tego katalogu wymusi ponowne wygenerowanie dokumentacji
 *     - `.vscode` zawierający ustawienia VS Code projektu
 *     - `build` zawierający skrypty kompilacji Pixie Engine na&nbsp;zadaną platformę. Katalog ten zawiera też skrypty doxygen i&nbsp;Apache Ant
 *     - `code` zawiera pełen kod źródłowy silnika
 *     - `content` zawiera pliki danych silnika które muszą być dołączone do&nbsp;końcowej wersji programu. Pliki te są automatycznie kopiowane do&nbsp;folderu wyjścia kompilacji
 *     - `content_prepare` zawiera pliki źródłowe z&nbsp;których przygotowana została zawartość katalogu `content`
 *     - `dependency` zawiera wszystkie zależności silnika
 *     - `manifest` zawiera manifest silnika - pliki meta kompilacji takie jak logo dokumentacji, logo silnia, pliki nagłówków doxygen
 *     - `private` to folder zwykle nie wersjonowany na&nbsp;systemie git który zawiera pliki wewnętrzne używane przez danego programistę
 * 
 * Katalog projektu ma natomiast następujące podkatalogi:
 *     - `!build` zawierający tymczasowe pliki kompilacji
 *     - `!output` zawierający rezultat ostatniej kompilacji podzielonej wg typu
 *     - `.vscode` zawierający ustawienia VS Code projektu
 *     - `.vscode-user` zawierający ustawienia użytkownika VS Code
 *     - `_changelog` to folder zawierający pliki historii samej gry takie jak plik zawierający numer kompilacji oraz listy zmian od&nbsp;poprzedniej kompilacji.
 *     - `_command` to folder analogiczny do&nbsp;folderu z&nbsp;katalogu głównego, ale którego komendy dotyczą konkretnej gry. Znajdują się tam skrypty wydające rozkaz kompilacji, testowania, budowania instalatora, etc. Można je wywoływać nie uruchamiając edytora VS Code (np. mogą zostać podłączone do&nbsp;zewnętrznego systemu). Większość z&nbsp;tych komend jest nieinteraktywna
 *     - `_ready` to folder przechowujący finałowe kompilacje projektu oraz ewentualne mapy linkera wypuszczonych wersji
 *     - `code` zawierający kod źródłowy danej gry i&nbsp;wszystkich aplikacji z&nbsp;nią związanych (jak np. edytora, albo startera)
 *     - `content` zawierający wszystkie pliki danych gry takie jak grafika, dźwięki, etc. Cała jego zawartość jest automatycznie kopiowana do&nbsp;folderu wyjścia kompilacji
 *     - `content_prepare` zawiera pliki źródłowe z&nbsp;których artysta przygotowuje zawartość katalogu `content`
 *     - `docs` początkowo pusty, ten folder zawiera wewnętrzną dokumentację danego projektu gry
 *     - `manifest` zawierający manifest gry - pliki meta kompilacji takie jak logo i&nbsp;ikonka gry, licencja wyświetlana podczas instalacji, pliki xml opisujące informacje o&nbsp;grze uwzględniające nazwę, domyślną ścieżkę instalacji, etc.
 *     - `press` również początkowo pusty, folder ten zawiera wszystkie materiały promocyjne które zostały bądź zostaną opublikowane dla danej gry
 *     - `private` to folder zwykle nie wersjonowany na&nbsp;systemie git który zawiera pliki wewnętrzne używane przez danego programistę
 */