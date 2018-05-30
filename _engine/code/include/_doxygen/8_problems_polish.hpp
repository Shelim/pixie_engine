
/**
* \~Polish @page engine_tests Testy
* @section engine_tests_intro Wprowadzenie
* Kluczowym elementem utrzymania jakości kodu we&nbsp;współczesnych aplikacjach są oczywiście testy, w&nbsp;tym testy automatyczne. 
* Jedną z&nbsp;głównych przesłanek przyjęcia takiej, a&nbsp;nie innej architektury Pixie Engine było wsparcie dla możliwie wysokiego pokrycia kodu
* przypadkami testowymi i&nbsp;zintegrowanie ich z&nbsp;repozytorium git. W&nbsp;ten sposób w&nbsp;przypadku modyfikacji któregokolwiek z&nbsp;fragmentów silnika
* programista zostałby powiadomiony o&nbsp;potencjalnym problemie regresji w&nbsp;momencie jej wprowadzenia, co znacznie skróciłoby czas jej poprawienia.
* 
* Pixie Engine zawiera trzy poziomy testów automatycznych:
*  - Testy jednostkowe biblioteki narzędzi. Te testy obejmują sprawdzanie funkcji i&nbsp;klas znajdujących się w&nbsp;najniższej warstwie implementacji silnika. Ich konstrukcja jest silnie modularna, pozbawiona wzajemnych odwołań. Testy narzędzi są najprostsze do&nbsp;implementacji gdyż opierają się na&nbsp;zasadzie: wykonaj funkcję, sprawdź rezultat. Rzadko kiedy istnieje realna potrzeba testowania więcej niż jednego elementu narzędzi w&nbsp;jednym przypadku testowym
*  - Testy globalne. Te testy obejmują wszystkie globalne komponenty i&nbsp;pliki rdzenia, ale przyjmuje się że&nbsp;podczas ich przeprowadzania silnik nie uruchamia żadnej aplikacji klienckiej (zobacz @ref engine_architecture). Na&nbsp;potrzeby testu przyjmuje się że&nbsp;każdy przypadek testowy to osobna funkcja main, która tworzy rozrusznik, uruchamia silnik, tworzy drzewko referencji, wykonuje test, niszczy silnik i&nbsp;przechodzi do&nbsp;kolejnego testu. Z&nbsp;powodu zastosowania klasy `program_t` testy globalne nie mogą być uruchamiane równolegle
*  - Testy aplikacji. Ostatnia kategoria obejmuje wszystkie lokalne komponenty, lokalne pliki rdzenia i&nbsp;samą aplikację kliencką. Są to najbardziej skomplikowane testy, jako iż&nbsp;poza rozrusznikiem musi zostać skonfigurowany i&nbsp;uruchomiony bootstrapper który standardowo przekazałby sterowanie do&nbsp;funkcji main danej gry. Funkcjonalność ta jest oczywiście zawieszona na&nbsp;moment testu, a&nbsp;po uruchomieniu modułu gry test wykonywany jest na&nbsp;klasach lokalnych gry
* @section engine_tests_example Przykłady testów
* @subsection engine_tests_example_unit Testy jednostkowe biblioteki narzędzi
* Poniższy test sprawdza poprawność weryfikacji białych znaków Ascii za&nbsp;pomocą funkcji biblioteki `is_whitespace_ascii`.
* Należy zwrócić uwagę na&nbsp;użyte makra `EXPECT_TRUE` i&nbsp;`EXPECT_FALSE`.
* @code{.cpp}
* TEST(UtilityTextUstringSymbol, IsWhitespaceAscii)
* {
*     EXPECT_TRUE(engine::is_whitespace_ascii(' '));
*     EXPECT_TRUE(engine::is_whitespace_ascii('\t'));
*     EXPECT_TRUE(engine::is_whitespace_ascii('\n'));
*     EXPECT_TRUE(engine::is_whitespace_ascii('\r'));
*     EXPECT_FALSE(engine::is_whitespace_ascii('a'));
*     EXPECT_FALSE(engine::is_whitespace_ascii(';'));
*     EXPECT_FALSE(engine::is_whitespace_ascii('='));
*     EXPECT_FALSE(engine::is_whitespace_ascii(u'\u20AC'));
* }
* @endcode
* @subsection engine_tests_example_global Testy globalne
* Poniższy test sprawdza poprawność działania modułu ustawień gry. Pierwsze linijki opisują rozrusznik,
* którego najważniejszą częścią jest wyłączenie wszystkich globalnych komponentów z&nbsp;wyjątkiem `config_storage`
* i&nbsp;stworzenie obiektu typu `progam_t`. Następnie zachowujemy w&nbsp;ustawieniach wartość test typu `true` i&nbsp;oczekujemy
* że&nbsp;po wczytaniu wciąż będzie `true` - drugi argument funkcji `retrieve` to wartość domyślna, ustawiona gdyby
* danej wartości nie dało sie odczytać, i&nbsp;w tym przypadku przerwałaby test.
* @code{.cpp}
* TEST(GlobalExample, Sample)
* {
*     BEGIN_PLATFORM_CONFIGURATION(test)
*     USE_STANDARD_POLICIES()
*     ALL_GLOBAL_COMPONENTS_BY_DEFAULT_ARE(disabled)
*     ENABLE_GLOBAL_COMPONENT(config_storage)
*     USE_STANDARD_GLOBAL_PROVIDERS()
*     USE_STANDARD_GLOBAL_SETTINGS()
*     END_PLATFORM_CONFIGURATION()
* 
*     std::shared_ptr<engine::program_t> program = test.ignite_for_tests();
*     
*     ASSERT_TRUE(program);
* 
*     program->get_config_storage()->store("test"_u, true);
*     EXPECT_TRUE(program->get_config_storage()->retrieve("test"_u, false));
* 
* }
* @endcode
* @subsection engine_tests_example_local Testy lokalne
* Poniższy test sprawdza możliwość dostania się do&nbsp;komponentu `terminal_t`. Jego początek jest analogiczny do&nbsp;testu globalnego.
* Następnie tworzy on obiekt kontekstu (zwykle tworzony przez instancer aplikacji) i&nbsp;rozpoczyna tworzenie bootstrappera, traktując
* resztę ciała funkcji jako `main` danej aplikacji. Oczywiście ponownie wyłączamy wszystkie lokalne komponenty poza terminalem
* i&nbsp;w końcowej części zawierającej test właściwy próbujemy go utworzyć.
* @code{.cpp}
* TEST(LocalExample, Sample)
* {
*     BEGIN_PLATFORM_CONFIGURATION(test)
*     USE_STANDARD_POLICIES()
*     ALL_GLOBAL_COMPONENTS_BY_DEFAULT_ARE(disabled)
*     USE_STANDARD_GLOBAL_PROVIDERS()
*     USE_STANDARD_GLOBAL_SETTINGS()
*     END_PLATFORM_CONFIGURATION()
* 
*     std::shared_ptr<engine::program_t> program = test.ignite_for_tests();
*     
*     ASSERT_TRUE(program);
* 
*     std::unique_ptr<engine::app_context_t> context = std::make_unique<engine::app_context_t>(program.get(), engine::args_t(""));
* 
*     BEGIN_BOOTSTRAPPER(test_app, context.get())
*     ALL_COMPONENTS_BY_DEFAULT_ARE(disabled)
*     ENABLE_COMPONENT(terminal)
*     USE_STANDARD_LOCAL_PROVIDERS()
*     USE_STANDARD_LOCAL_SETTINGS()
*     APP(game)
*     END_BOOTSTRAPPER()
* 
*     auto terminal = test_app.get_terminal();
* 
*     ASSERT_TRUE(terminal);
* 
* }
* @endcode
* @section engine_tests_coverage Pokrycie testami
* W&nbsp;chwili obecnej zaimplementowane jest 8 testów sprawdzających 30% kodu źródłowego (1040 z&nbsp;3437 dróg). Istnieje możliwość dokładnego sprawdzenia
* w&nbsp;edytorze VS Code które linie są już pokryte testami, co obrazuje poniższy zrzut ekranu. Oczywiście tylko linie zawierające aktywny
* kod są uwzględnione w&nbsp;tym zestawieniu.
* @image html tests_coverage.jpg "Ilustracja pokrycia testami kodu"
* @image latex tests_coverage.jpg "Ilustracja pokrycia testami kodu" height=9cm
*
* \~Polish @page engine_problems_di Problem z&nbsp;wstrzykiwaniem zależności Boost
*
* Poza problemami opisywanymi na&nbsp;bieżąco w&nbsp;poprzednich rozdziałach, problem z&nbsp;wstrzykiwaniem zależności Boost był najpoważniejszym
* wyzwaniem przed jakim stanął autor Pixie Engine. Problem został zgłoszony autorom biblioteki Boost DI, jednak do&nbsp;dziś nie powstała
* implementacja naprawcza. Istnieje natomiast implementacja omijająca, którą przygotował sam autor Pixie Engine;
* 
* Problem polegał na&nbsp;niewłaściwym rozwiązywaniu zależności przez fabryki abstrakcyjne (tworzenie nowych instancji singletonów), 
* co powodowało twarde zawieszenie się aplikacji na&nbsp;etapie tworzenia drzewka obiektów. Poniższy przykład ilustruje sytuację:
* @code{.cpp}
* class interface_test
* {
* public:
* 
*     virtual int print_address() = 0;
* };
* 
* class real_test : public interface_test
* {
* public:
* 
*     int print_address() final
*     {
*         return (int)(void*)this;
*     }
* };
* 
* class ifactorable
* {
* 
* public:
* 
*     virtual int get_address() = 0;
* 
* };
* 
* class factorable : public ifactorable
* {
* public:
* 
*     factorable(std::shared_ptr<interface_test> test) : test(test)
*     {
* 
*     }
* 
*     int get_address() final
*     {
*         return test->print_address();
*     }
* 
* private:
* 
*     std::shared_ptr<interface_test> test;
* };
* 
* int main() {
*     auto injector1 = di::make_injector(di::bind<interface_test>().in(scoped).to<real_test>()[boost::di::override]
*         , di::bind<ifactorable>().in(scoped).to<factorable>()
*         , di::bind<ifactory<ifactorable>>().to(factory<factorable>{})
*     );
* 
*     auto injector2 = di::make_injector(di::bind<interface_test>().in(scoped).to<real_test>()[boost::di::override]
*         , di::bind<ifactorable>().in(scoped).to<factorable>()
*         , di::bind<ifactory<ifactorable>>().to(factory<factorable>{})
*     );
* 
*     std::shared_ptr<ifactorable> ptr1 = injector1.create<std::shared_ptr<ifactorable>>();
*     std::shared_ptr<ifactorable> ptr2 = injector1.create<std::shared_ptr<ifactorable>>();
*     std::unique_ptr<ifactorable> ptr3 = injector1.create<std::unique_ptr<factorable>>();
*     std::unique_ptr<ifactorable> ptr4 = injector1.create<const ifactory<ifactorable> & >().create();
*     std::unique_ptr<ifactorable> ptr5 = injector1.create<const ifactory<ifactorable> & >().create();
* 
*     std::shared_ptr<ifactorable> ptr6 = injector2.create<std::shared_ptr<ifactorable>>();
*     std::unique_ptr<ifactorable> ptr7 = injector2.create<const ifactory<ifactorable> & >().create();
* 
*     std::cout << "These should be equal!" << std::endl;
*     std::cout << "ptr 1 = " << ptr1->get_address() << std::endl;
*     std::cout << "ptr 2 = " << ptr2->get_address() << std::endl;
*     std::cout << "ptr 3 = " << ptr3->get_address() << std::endl;
*     std::cout << "ptr 4 = " << ptr4->get_address() << std::endl;
*     std::cout << "ptr 5 = " << ptr5->get_address() << std::endl;
*     std::cout << "These should be equal (but different from previous group)!" << std::endl;
*     std::cout << "ptr 6 = " << ptr6->get_address() << std::endl;
*     std::cout << "ptr 7 = " << ptr7->get_address() << std::endl;
* 
*     return 0;
* }
* @endcode
* 
* Przykładowy rezultat wykonania tego kodu znajduje się poniżej:
* @code
* These should be equal!
* ptr 1 = 5953696
* ptr 2 = 5953696
* ptr 3 = 5953696
* ptr 4 = 5958336
* ptr 5 = 5953744
* These should be equal (but different from previous group)!
* ptr 6 = 5954720
* ptr 7 = 5977712
* @endcode
* Problem został opisany pod adresem https://github.com/boost-experimental/di/issues/240
* 
* Przyczyną okazało się użycie statycznej własności wewnątrz fabryki i&nbsp;przesuwanie zarządcy wstrzykiwania na&nbsp;potrzeby
* tworzenia list argumentów bez przesuwania go z&nbsp;powrotem.
* 
* Rozwiązanie drugiego problemu było trywialne, wymagało dodania jednej linijki kodu. Natomiast problem
* statycznej zmiennej był dużo poważniejszy. W&nbsp;C++ statyczna własność w&nbsp;klasie szablonowej istnieje raz
* dla każdego wariantu szablonu. W&nbsp;tym przypadku fabryka wykorzystywała jako szablon podstawę samego zarządcy wstrzykiwania.
* Oznaczało to, między innymi, iż&nbsp;zmiana zarządcy wstrzykiwania - nawet niewielka - generowała nową instancję obiektu (co widać
* po&nbsp;poprzednim przykładzie). Zmianą taką było na&nbsp;przykład przypięcie dodatkowych argumentów do&nbsp;konstruktora obiektu
* fabrykowanego. Z&nbsp;tego powodu ta funkcjonalność musiała zostać usunięta.
* 
* Po&nbsp;jej usunięciu pojawił się inny problem: wszystkie instancje obiektu singleton tworzone przez danego zarządcę wstrzykiwania
* dzieliły ten sam wskaźnik, nawet jeżeli były wyraźnie zadeklarowane jako osobne obiekty. Stało się oczywiste, że
* statyczne pole musiało zostać usunięte. Wciąż jednak potrzebne było sprawdzenie obiektu po&nbsp;typie szablonowym
* na&nbsp;test istnienia. Pierwotnie rozważano formę mapy-czasu-kompilacji@cite compileTimeKeyValueStore, ale wkrótce okazało się że&nbsp;było to rozwiązanie
* niemożliwe do&nbsp;zaimplementowania z&nbsp;przyczyn logicznych; W&nbsp;szczególności obiekty mogły być tworzone dynamicznie już po
* kompilacji (np. w&nbsp;formie wtyczki dll). W&nbsp;związku z&nbsp;tym jednym rozwiązaniem stała się zwykła mapa (a dokładniej niesortowana
* mapa - z&nbsp;przyczyn wydajnościowych) której kluczem musiała być informacja o&nbsp;typie szablonowym. Szczęśliwie C++ dopuszcza
* taką konstrukcję w&nbsp;formie elementu biblioteki standardowej `std::type_index`. Ostatnią kwestią do&nbsp;rozwiązania był
* problem jednego klucza: szablon musiał opierać się o&nbsp;dwie własności, a&nbsp;mapa przyjmuje tylko jeden klucz. Dlatego
* w&nbsp;poniższym kodzie użyto pustej klasy szablonowej żeby stworzyć nowy pojedynczy typ z&nbsp;dwóch typów `TInjector` i&nbsp;`TDependency`.
* 
* Ostatecznie implementacja fabryki przybrała następującą formę:
* @code{.cpp}
*   template <class T>
*     struct factory {
*         template <class TInjector, class TDependency>
*         auto operator()(const TInjector& injector, const TDependency&) noexcept {
*             auto iter = data_map.find(std::type_index(typeid(typename factory<T>::key_t<TInjector, TDependency>)));
*             std::shared_ptr<ifactory_base> ret;
*             if(iter == data_map.end()) {
*                 ret = std::make_shared<factory_impl<TInjector, T, typename TDependency::expected>>(injector);
*                 data_map.insert(std::make_pair(std::type_index(typeid(typename factory<T>::key_t<TInjector, TDependency>)), ret));
*             }
*             else
*                 ret = iter->second;
*             return std::static_pointer_cast<factory_impl<TInjector, T, typename TDependency::expected>>(ret);
*         }
*     private:
*         template <class T1, class T2> class key_t {};
*         // Important! This is NOT static:
*         std::unordered_map<std::type_index, std::shared_ptr<ifactory_base> > data_map;
*     };
* @endcode
*/