 /**
 * \~Polish @page code_convention Konwencje kodu
 * @tableofcontents
 * @section code_convention_naming Konwencje nazw
 *
 *  1. Wszystkie nazwy są pisane małymi literami alfabetu łacińskiego z podkreślnkiem jako separatorem (np. `hello_world`)
 *  2. Podstawowe wzorce nazw:
 *      - `obiekt_właściwość` (np. `game_name`)
 *      - `akcja_obiekt_właściwość` (np. `new_window_handle`)
 *      - Więcej niż jedno słowo może być rozdzielane dodatkowym podkreślnikiem (np. `new_game_window_handle`)
 *      - Wszystkie typy mają przyrostek `_t` (np. `game_handle_t`)
 *      - Nie używa się notacji węgierskiej!
 *  3. Ustandaryzowane nazwy:
 *      - `asset` -> Każda nie wpisana na stałe do kodu źródłowego dana która może być modyfikowana. Zobacz `content`
 *      - `build_number` -> Numer zwiększany za każdym przebiegiem kompilacji (współdzielony przez wszystkie typy kompilacji)
 *      - `changelog` -> Katalog zawierający meta-informacje o zmianach we wszystkich poprzednich kompilacjach (wykorzystywany przez automatycznego aktualizera)
 *      - `component` -> Pojedynczy element aplikacji (kodu źródłowego). Komponenty dzielą się na globalne (współdzielone między aplikacjami) i lokalne (należące do konkretnej instancji aplikacji). Wszystkie komponenty mogą być włączone i wyłączonane poprzez zastąpienie ich funkcjonalności wariantem pustym (`dummy`) bądź makietą (`mockup`) - na potrzeby testów
 *      - `concurrention` -> Kod dedykowany wielowątkowości
 *      - `config` -> Ustawienia czasu wykonania (nie mylić z `settings`) które zwykle przechowuje się w sposób zależny od platformy
 *      - `console` -> Wewnątrzna konsola wypisująca dane tekstowe i reagująca na wprowadzanie danych tekstowych
 *      - @todo Uzupełnić i przetłumaczyć listę nazw
 *      - `container` -> Usually C++'s Standard Template Library container, or custom one
 *      - `content` -> All `assets`
 *      - `data` -> Piece of information for engine. Common term for `assets`, `config` files, user profiles, saves, etc.
 *      - `debug` -> Output configuration intended on running only on developer machine. Also piece of code only to be used in debug. See @ref build_configuration "build specification"
 *      - `dependency` -> Other party piece of code or application that is used inside Pixie engine
 *      - `environment` -> Specification of machine running pixie (usually end user machine)
 *      - `final` -> Output configuration inteded for relese. Beside application it will build auto-updater script and prepare them to push to repository. See @ref build_configuration "build specification"
 *      - `hybrid` -> Output configuration inteded for testers. It is faster then debug and does not contain debugging symbols. Can be open-tested. See @ref build_configuration "build specification"
 *      - `logger` -> A way to dump information to a text file for bug checking on end user machines
 *      - `manifest` -> metadata for given item
 *      - `module` -> Large logical data-pack of assets. Each running game is composed of one or more modules. Example: large chapters/Total Conversion mods; See: `submodule`
 *      - `platform` -> Target PC. See `environment` (borader term)
 *      - `private` -> A thing to be used by developer himself and not included in final build
 *      - `project` -> Project for given platform (like Visual Studio for Windows, XCode for OS_X, etc.)
 *      - `provider` -> Simple class giving small pieces of information to other classes (like default path)
 *      - `release` -> Common name for `hybrid` and `final` output configuration. See @ref build_configuration "build specification"
 *      - `settings` -> Compile-time engine-wide settings. Needs to be passed to `bootstrapper`
 *      - `std` -> Macros controlling various engine-wide definitions. Usually used to expand features by client application without need for tampering with engine internals. Have little to no runtime impact. See @ref std
 *      - `submodule` -> Smaller logical data-pack of assets. Each running game can have zero or more active submodules. Example: DLC or mini-mods; See: `module`
 *      - `terminal` -> Usually end machine console, unless it does not have one
 *      - `utility` -> Simple class and functions for rest engine to utilize
 *
 * @section code_convention_code Code conventions
 * @code{.cpp}
 *
 *    // no 'using namespace' ever. Ever.
 *
 *    namespace game
 *    {
 *        void foo(int a, int b = 5) { return a + b; } // single line allowed for simple functions
 *        void bar(int a, int b)
 *        {
 *            int ret = 5;
 *            ret += b;
 *            return ret;
 *        }
 *        
 *        class baz_t // types always end with _t
 *        {
 *
 *        public: // separate line, aligned to class
 *
 *           baz_t(std::shared_ptr<logger_t> logger) : logger(logger) // Use dependency injection
 *           {
 *
 *           }
 *
 *           virtual ~baz_t()
 *           {
 * 
 *           }
 *
 *           int get_foo() const // accessor
 *           {
 *               return foo;
 *           }
 *           void set_foo(int val) // argument name always 'val'
 *           {
 *               foo = val; // no this->
 *           }
 *
 *           void do_nothing()
 *           {
 *               // Do nothing because blah, blah blah <- empty functions require comments!
 *           }
 *
 *           void long_operation(); // defnition in source
 *
 *           virtual void sample() = 0; // no non-pure virtuals, unless you want some default functionality
 *
 *           void operate()
 *           {
 *               set_foo(5);
 *               operate_local(); // _local for virtual methods of the same name
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
 *        class baz2_t : public baz_t // no final here, it get messed with Depedency Injection... :-(
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
 *            void sample() final // no virtual if its final!
 *            {
 *                operate(); // call base class. No this!
 *            }
 *
 *        private:
 *
 *            
 *           void operate_local()
 *           {
 *               get_logger()->log_msg(core, "Simple log message"_u);
 *           }
 *
 *        }
 *
 *        template<class arg_t> class foo_t // template arguments as `class`. Lowercase.
 *        {
 *
 *        public:
 *             int get_a()
 *             {
 *                 return a; // no static variables!
 *             }
 *
 *        private:
 *
 *             int a; // no direct expsoure
 *        }
 *
 *    } // each bracket in own line
 *
 * @endcode
 */