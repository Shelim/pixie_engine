 /**
 * \~Polish @page code_convention Konwencje kodu
 * @tableofcontents
 * @section code_convention_naming Konwencje nazw
 *
 *  1. Wszystkie nazwy są pisane małymi literami alfabetu łacińskiego z podkreślnikiem jako separatorem (np. `hello_world`)
 *  2. Podstawowe wzorce nazw:
 *      - `obiekt_właściwość`, nie na odwrót! (np. `game_name`)
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
 *      - `debug` - Konfiguracja wyjściowa która powinna być uruchamiana tylko na maszynie developera. Także każdy kawałek kodu który ma być wykorzystywany tylko do odpluskwiania. Zobacz @ref build_configuration "rodzaje konfiguracji wyjściowych"
 *      - `def` - Makra kontrolujące różne definicje wewnątrz całego silnika. Zwykle umożliwiają rozszerzanie cech silnika przez programistę gry bez potrzeby manipulowania we wnętrzu samego silnika. Ich konstrukcja jest silnie zoptymalizowana (makra są rozwijane na etapie kompilacji).
 *      - `dependency` - Cudzy (zewnętrzny) kawałek kodu lub program wykorzystany wewnątrz Pixie Engine
 *      - `environment` - Specyfikacja maszyny na której uruchomiony jest Pixie Engine (najczęściej urządzenie gracza)
 *      - `final` - Konfiguracja wyjściowa przeznaczona do wypuszczenia gry. Poza kompilacją aplikacji zbuduje także informacje automatycznej aktualizacji i przygotuje pliki do wgrania do publicznego repozytorium. Zobacz @ref build_configuration "rodzaje konfiguracji wyjściowych"
 *      - `hybrid` - Konfiguracja wyjściowa przeznaczona dla testerów. Jest szybsza niż `debug` i nie zawiera symboli odpluskwiacza. Może być używana do publicznej bety gry. Zobacz @ref build_configuration "rodzaje konfiguracji wyjściowych"
 *      - `logger` - Sposób zrzucania informacji (zwykle do pliku tekstowego) w celu sprawdzania błędów na urządzeniach klienckich
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
 * @code{.cpp}
 *
 *    // Nigdy nie używamy 'using namespace...'
 *
 *    namespace game
 *    {
 *        void foo(int a, int b = 5) { return a + b; } // Pojedyncza linia dozwolona dla prostych funkcji
 *        void bar(int a, int b)
 *        {
 *            int ret = 5;
 *            ret += b;
 *            return ret;
 *        }
 *        
 *        class baz_t // Każdy typ kończy się na _t
 *        {
 *
 *        public: // Pojedyncza linia, wyrównana do słowa kluczowego class
 *
 *           baz_t(std::shared_ptr<logger_t> logger) : logger(logger) // Wykorzystanie wstrzykiwania zależności
 *           {
 *
 *           }
 *
 *           virtual ~baz_t()
 *           {
 * 
 *           }
 *
 *           int get_foo() const // Akcesor
 *           {
 *               return foo;
 *           }
 *           void set_foo(int val) // Nazwa argumentu ustawiacza to zawsze 'val'
 *           {
 *               foo = val; // Bez this->
 *           }
 *
 *           void do_nothing()
 *           {
 *               // Funkcja jest pusta ponieważ bla, bla, bla <- puste funkcje wymagają komentarza
 *           }
 *
 *           void long_operation(); // Definicje dłuższych funkcji w plikach źródłowych
 *
 *           virtual void sample() = 0; // Brak nie czysto wirtualnych metod
 *
 *           void operate()
 *           {
 *               set_foo(5);
 *               operate_local(); // przyrostek _local dla wirtualnych metod o tej samej nazwie
 *           }
 *
 *        protected:
 *
 *           std::shared_ptr<logger_t> get_logger()
 *           {
 *               return logger;
 *           }
 *
 *        private:
 *
 *           int foo;
 *           virtual void operate_local() = 0;
 *           std::shared_ptr<logger_t> logger;
 *
 *        };
 *
 *        class baz2_t : public baz_t // Bez final, ponieważ stwarza problemy z wstrzykiwaniem zależności... :-(
 *        {
 *
 *        public:
 * 
 *            baz2_t(std::shared_ptr<logger_t> logger) : baz_t(logger)
 *            {
 * 
 *            }
 *
 *
 *            void sample() final // Bez virtual jeżeli metoda jest final
 *            {
 *                operate(); // Wywołuje klasę podstawową. Bez this!
 *            }
 *
 *        private:
 *
 *            
 *           void operate_local()
 *           {
 *               get_logger()-log_msg(core, "Simple log message"_u);
 *           }
 *
 *        }
 *
 *        template<class arg_t> class foo_t // argument szablonowy to `class` a nie `typename`. Przyrostek _t. Pisane małym literami.
 *        {
 *
 *        public:
 *             int get_a()
 *             {
 *                 return a; // bez statycznych zmiennych!
 *             }
 *
 *        private:
 *
 *             int a; // bez bezpośredniego wystawienia (enkapsulacja)
 *        }
 *
 *    } // Każdy nawias w osobnej linii
 *
 * @endcode
 */