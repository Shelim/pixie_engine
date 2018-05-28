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
 */