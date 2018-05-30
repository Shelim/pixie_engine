
/**
* \~Polish @page engine_startup Sekwencja rozruchowa silnika
* @tableofcontents
* @section engine_startup_intro Wprowadzenie
* Sekwencja rozruchowa to pierwszy etap uruchamiania silnika Pixie Engine. Jednym z~najważniejszych problemów który wystąpił
* na tym etapie to niejednolitość sposobów uruchamiania na różnych docelowych platformach. O~ile na komputerach osobistych możemy
* założyć istnienie funkcji `main` (aczkolwiek już na Windows mamy `WinMain`), to platformy mobilne nie muszą wcale mieć jej odpowiednika.
* Dla przykładu wczesne wersje Androida nie pozwalały linkowania pliku wykonywalnego; aplikacja natywna mogła być uruchamiana tylko
* jako wywołanie zwrotne do współdzielonej biblioteki. Takie wywołania zwrotne cechowała podstawowa cecha: musiało się zakończyć
* możliwie szybko, jako że blokowało główny wątek aplikacji. Na platformie Android istnieje także jeszcze jeden poważny problem:
* system operacyjny nie gwarantuje zniszczenia biblioteki między uruchomieniami programu co oznacza że potencjalnie wszystkie
* zmienne statyczne w~programie mogą nie zostać przeładowane w~takiej sytuacji. Z~tego powodu Pixie Engine nie pozwala na tworzenie
* zmiennych statycznych w~żadnej części implementacji.
* 
* @section engine_startup_platform Konfiguracja platformy
* Pierwszym etapem tworzenia instancji programu jest konfiguracja ustawień danej platformy (oczywiście mówimy tu o~standardowym
* uruchomieniu) Odpowiadają za to specjalne makra preprocesora które generują
* obiekt rozrusznika. Przykład takiej konfiguracji znajduje się poniżej:
* @code{.cpp}
* BEGIN_PLATFORM_CONFIGURATION(windows)
* USE_STANDARD_POLICIES()
* ALL_GLOBAL_COMPONENTS_BY_DEFAULT_ARE(enabled)
* USE_STANDARD_GLOBAL_PROVIDERS()
* USE_STANDARD_GLOBAL_SETTINGS()
* #if !PIXIE_IS_DEBUG_BUILD
*     DISABLE_GLOBAL_COMPONENT(profiler)
* #endif
* END_PLATFORM_CONFIGURATION()
* @endcode
* Taka konfiguracja wygeneruje nowy obiekt o~nazwie `windows` i~nieokreślonym typie szablonowym. Jedną z~jego metod jest `ignite_from_main`
* która bierze jako argumenty wartości linii komend i~generuje obiekt programu. Jeżeli obiekt jest pusty, to znaczy że silnik z~jakiegoś
* powodu nie może wystartować (np. inna instancja jest już uruchomiona).
* 
* @section engine_startup_program Obiekt programu
* Obiekt programu musi bezwzględnie "żyć" całą długość działania programu. Na platformach na których nie istnieje funkcja `main`
* zdolna utrzymać instancję, należy umieścić obiekt programu w~zmiennej globalnej (jest to jedyny przypadek w~którym używa sie zmiennych
* globalnych w~Pixie Engine). Uwaga: nie można tego zrobić w~zmiennej statycznej, patrz wprowadzenie do tego rozdziału.
* Obiekt programu jest z~definicji singletonem@cite designPattern.
* 
* Obiekt programu zawiera jako własności akcesory wszystkich globalnych komponentów. Jeżeli potrzeba dostać się do obiektu programu
* z~poziomu aplikacji istnieje specjalny element rdzenia o~nazwie `program_reference_t` który trzyma referencję na obiekt programu.
* Uwaga - o~ile obiekt programu jest singletonem, nic nie zabrania jego zniszczenia i~ponownego stworzenia (jest to np. wykorzystywane
* w~trakcie przebiegów testowych). Jedynym ograniczeniem jest to że na raz może istnieć tylko jedna instancja.
* 
* Stworzenie i~obsługa obiektu programu nie blokuje aktualnego wątku - wszystkie operacje są asynchroniczne. Z~tego powodu
* implementacja funkcji `main` zwykle musi czekać na zakończenie programu (np. używając metody `wait_for_completion` z~komponentu
* `app_overseer`). Na platformach na których nie ma możliwości czekać na główny wątek (np. wspomniany Android) rozwiązaniem jest
* użycie metody `has_completed` która zwraca `bool` i~również nie blokuje danego wątku.
*
* @section engine_startup_test Uruchomienie testowe
* Uruchomienie testowe silnika Pixie Engine zwykle wymaga kompilacji w~trybie testowym (z makrem `PIXIE_IS_TESTER_BUILD` zdefiniowanym jako 1).
* W~tym przypadku wszystkie rozważania o~czekaniu na ukończenie z~powyższego rozdziału są utrzymane, ale zamiast używać rozrusznika,
* używa się obiektu `test_runner_t`. Przykładowa implementacja `main` na Linuxie może wyglądać tak:
* @code{.cpp}
* int main(int argc, char * argv[])
* {
*     engine::test_runner_t runner;
* 
*     runner.run_test(argc, const_cast<const char**>(argv));
*     runner.wait_till_completed();
* 
*     return runner.get_return_code();
* }
* @endcode
* 
* Oczywiście na platformach na których nie mamy funkcji `main` obiekt `test_runner_t` również jest obiektem globalnym,
* a~zamiast `wait_till_completed` używane byłoby zapytanie zwrotne `has_completed`
* 
* @section engine_startup_app Uruchomienie aplikacji
* Pierwsza aplikacja jest zwykle uruchamiana przez program na podstawie argumentów linii komend.
* Każda aplikacja ma własną funkcję `main_XXX` która zwykle jest uruchamiana w~osobnym wątku. Funkcja ta przyjmuje
* zawsze jeden argument: `app_context_t` czyli kontekst wykonania. Kontekst zawiera informacje o~instancji,
* jak również referencję na program. Zwykle następnym krokiem jest uruchomienie bootrappera. Przykład funkcji
* `main` dla aplikacji `game`:
* @code{.cpp}
* engine::app_t::return_code_t engine::client::main_game(app_context_t* context)
* {
*     BOOTSTRAP_APP(game, context)
*     return engine::app_t::return_code_t::success;
* }
* @endcode
* Ta przykładowa aplikacja wystartuje boostrappera o~nazwie `game` i~natychmiast zakończy program.
* 
* Bootrapper zawiera wskaźniki na wszystkie komponenty silnika (zarówno lokalne jak i~globalne) i~zwykle jest używany
* tylko w~trakcie uruchomienia do stworzenia drzewka obiektów aplikacji. Nieco bardziej zaawansowany przykład `main_game` znajduje się poniżej:
* @code{.cpp}
* engine::app_t::return_code_t engine::client::main_game(app_context_t* context)
* {
*     BOOTSTRAP_APP(game, context) 
* 
*     auto term = game.get_terminal()->open("Konsolka", engine::terminal_t::color_t::black);
*     term->write("Hello World!", engine::terminal_t::color_t::green_light);
* 
*     return engine::app_t::return_code_t::success;
* }
* @endcode
* 
* @section engine_startup_testcase Przypadek testowy
* W~trakcie testów silnik jest wielokrotnie uruchamiany, tworzony jest boostraper i~konkretny komponent, po czym wykonywany
* jest test, sprawdzany jest rezultat i~całość jest zabijana, po czym proces powtarza się od początku na kolejnym komponencie.
* Przypadek testowy stworzenia instancji terminala może wyglądać tak:
* @code{.cpp}
* TEST(TerminalCanBeInstanced, LocalBasics)
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
*/