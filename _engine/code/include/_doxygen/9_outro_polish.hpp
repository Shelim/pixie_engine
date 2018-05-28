
/**
*
* \~Polish @page engine_app Przykładowa aplikacja
* Przykładowa aplikacja dołączona do niniejszej pracy prezentuje sposób w jaki można utworzyć za pomocą Pixie Engine prostą grę strategiczną.
* 
* @image html sample_app.jpg "Przykładowy ekran gry"
* @image latex sample_app.jpg "Przykładowy ekran gry" height=9cm
*
* Gracz wciela się w tej grze w rolę dowódcy oddziału łuczników i może wydawać im rozkazy za pomocą myszki oraz klawiatury. 
* 
* @section engine_app_code Szczegóły implementacji
* Pierwszym elementem jaki musi zaimplementować twórca danej gry są tzw. funkcje i klasy klienckie. Jest to niewielki zbiór elementów które
* silnik będzie automatycznie wywoływał po uruchomieniu danego programu. W chwili obecnej do zaimplementowania są dwa takie elementy.
* 
* @subsection engine_app_code_resolve_app Odkrywca aplikacji
* Ten element informuje silnik która aplikacja ma zostać uruchomiona w przypadku otrzymania konkretnych argumentów linii komend.
* Odkrywca aplikacji musi być też zdolny do wykonania zapytania wstecznego, tj. określenia jakie argumenty linii komend są potrzebne do uruchomienia
* domyślnej instancji danej aplikacji. Ponieważ nasza gra posiada tylko jedną aplikację, implementacja odkrywcy jest bardzo prosta.
* @code{.cpp}
* class resolve_app_t
* {
* 
* public:
* 
*     resolve_app_t(std::shared_ptr<program_args_t> program_args) : program_args(program_args)
*     {
* 
*     }
* 
*     engine::app_t::kind_t resolve_app(app_context_t * context)
*     {
*         return engine::app_t::kind_t::game;
*     }
* 
*     engine::args_t startup_args_to_run_new_instance_of(app_t::kind_t kind)
*     {
*         return program_args->get_program_args();
*     }
* 
* private:
* 
*     std::shared_ptr<program_args_t> program_args;
* };
* @endcode
* @subsection engine_app_code_main Funkcja główna gry (ang. main)
* Funkcja główna gry jest standardowo wywoływana przez silnik z chwilą uruchomienia aplikacji, zaś jej opuszczenie jest sygnałem do jej wyłączonia.
* Funkcja gry jest zwyczajowo uruchamiana w dedykowanym wątku, a silnik zezewala na kilka funkcji głównych działających współbieżnie, np. w sytuacji uruchomienia
* kilku instancji gry. W finałowym produkcie funkcja main powinna, po wykonaniu czynności rozruchowych, oddać sterowanie silnikowemu rendererowi.
* Ponieważ jednak funkcjonalność renderera nie została jeszcze ukończona, wprowadzono pewne uproszczenie wykorzystując prosty renderer SDL.
* @code{.cpp}
* engine::app_t::return_code_t engine::client::main_game(app_context_t* context)
* {
*     BOOTSTRAP_APP(game, context)
* 
*     std::shared_ptr<renderer_t> renderer = std::make_shared<renderer_t>(&game, context);
*     std::shared_ptr<assets_t> assets = std::make_shared<assets_t>(renderer);
*     std::shared_ptr<input_t> input = std::make_shared<input_t>(renderer);
*     std::shared_ptr<state_t> state = std::make_shared<state_t>(renderer, assets, input);
* 
*     while(!renderer->has_ended())
*     {
*         input->update();
*         state->update();
* 
*         renderer->begin_frame();
*         state->draw();
*         input->draw_cursor(assets->get_tex(assets_t::tex_t::cursor));
*         renderer->end_frame();
*     }
* 
*     return engine::app_t::return_code_t::success;
* }
* @endcode
* Wyjaśnienia może wymagać metoda `has_ended`. Zwraca ona wartość logiczną 1 w przypadku wystąpienia co najmniej jednego ze zdarzeń:
*     - użytkownik zamknął okno programu
*     - użytkownik nacisnął skrót Alt + F4
*     - System Windows wydał polecenie zamknięcia programu np. przed ponownym uruchomieniem z powodu instalacji aktualizacji
* @subsection engine_app_code_world Świat gry
* Oczywiście Pixie Engine nie nakłada żadnego ograniczenia na sposób przedstawienia świata gry. W tym przykładzie postanowiono zaimplementować
* świat w oparciu o mapę kwadratowych kafli z których każdy był w stanie pomieścić dowolną ilość aktorów takich jak postaci. Dla uproszczenia
* implementacji podjęto decyzję o pominięciu systemu kolizji między postaciami. Wymagałoby to m.in. zaimplementowania algorytmu poszukiwania drogi takiego
* jak Dijkstra czy A*.
*
* W chwili obecnej świat gry składa się z mapy kafli o rozmiarze 8x8 i liczebności 256 x 256. Oczywiście nic nie stoi na przeszkodzie tworzenia dużo większych
* plansz przy zastosowaniu poprawnego algorytmu przycinania mapy do rozmiarów widoku
*
* @image html sample_app_tiles.jpg "Ilustracja rozmiaru kafla"
* @image latex sample_app_tiles.jpg "Ilustracja rozmiaru kafla" height=7cm
*
* Jedną z ciekawszych wizualnie decyzji było rozsynchronizowanie animacji postaci (zarówno prędkości animacji jak i punktu startowego). Uzyskano w ten sposób
* naturalny efekt drobnych różnic między żołnierzami, bez potrzeby angażowania dodatkowej grafiki.
*
* Jako ciekawostkę można odnotować fakt iż wszystkie animacje postaci w aplikacji testowej zostały nagrane w prostym systemie motion-capture stworzonym
* za pomocą urządzenia Kinect przez autora niniejszej pracy.
*
* \~Polish @page engine_future Spojrzenie w przyszłość
* Projekt w stanie obecnym oczywiście nie wyczerpuje oczywiście pełnej możliwej implementacji silnika gier 2D.
* Doprowadzenie go do zdolności produkcyjnej to wyzwanie autora na nadchodzące miesiące i lata i prawdopodobnie pracę magisterską.
* W chwili obecnej szczególnie brakuje:
*  - Ukończonego renderera; Zaimplementowana jest tylko wersja testowa oparta o SDL
*  - Obsługi dźwięków za pomocą OpenAL
*  - Komponentów wykonania. Ich obecna liczba (~30 sztuk) zostanie prawdopodobnie zwiększona czterokrotnie w wersji ostatecznej
*  - Zwiększenia pokrycia przypadkami testowymi do poziomu 90%
*  - Wsparcia dla dodatkowych platform: Linuxa i OS X.
*/