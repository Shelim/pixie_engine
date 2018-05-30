
/**
*
* \~Polish @page engine_app Przykładowa aplikacja
* Przykładowa aplikacja dołączona do&nbsp;niniejszej pracy prezentuje sposób w&nbsp;jaki można utworzyć za&nbsp;pomocą Pixie Engine prostą grę strategiczną.
* 
* @image html sample_app.jpg "Przykładowy ekran gry"
* @image latex sample_app.jpg "Przykładowy ekran gry" height=9cm
*
* Gracz wciela się w&nbsp;tej grze w&nbsp;rolę dowódcy oddziału łuczników i&nbsp;może wydawać im rozkazy za&nbsp;pomocą myszki oraz klawiatury. 
* 
* @section engine_app_code Szczegóły implementacji
* Pierwszym elementem jaki musi zaimplementować twórca danej gry są tzw. funkcje i&nbsp;klasy klienckie. Jest to niewielki zbiór elementów które
* silnik będzie automatycznie wywoływał po&nbsp;uruchomieniu danego programu. W&nbsp;chwili obecnej do&nbsp;zaimplementowania są dwa takie elementy.
* 
* @subsection engine_app_code_resolve_app Odkrywca aplikacji
* Ten element informuje silnik która aplikacja ma zostać uruchomiona w&nbsp;przypadku otrzymania konkretnych argumentów linii komend.
* Odkrywca aplikacji musi być też zdolny do&nbsp;wykonania zapytania wstecznego, tj. określenia jakie argumenty linii komend są potrzebne do&nbsp;uruchomienia
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
* Funkcja główna gry jest standardowo wywoływana przez silnik z&nbsp;chwilą uruchomienia aplikacji, zaś jej opuszczenie jest sygnałem do&nbsp;jej wyłączonia.
* Funkcja gry jest zwyczajowo uruchamiana w&nbsp;dedykowanym wątku, a&nbsp;silnik zezewala na&nbsp;kilka funkcji głównych działających współbieżnie, np. w&nbsp;sytuacji uruchomienia
* kilku instancji gry. W&nbsp;finałowym produkcie funkcja main powinna, po&nbsp;wykonaniu czynności rozruchowych, oddać sterowanie silnikowemu rendererowi.
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
* Wyjaśnienia może wymagać metoda `has_ended`. Zwraca ona wartość logiczną 1 w&nbsp;przypadku wystąpienia co najmniej jednego ze&nbsp;zdarzeń:
*     - użytkownik zamknął okno programu
*     - użytkownik nacisnął skrót Alt + F4
*     - System Windows wydał polecenie zamknięcia programu np. przed ponownym uruchomieniem z&nbsp;powodu instalacji aktualizacji
* @subsection engine_app_code_world Świat gry
* Oczywiście Pixie Engine nie nakłada żadnego ograniczenia na&nbsp;sposób przedstawienia świata gry. W&nbsp;tym przykładzie postanowiono zaimplementować
* świat w&nbsp;oparciu o&nbsp;mapę kwadratowych kafli z&nbsp;których każdy był w&nbsp;stanie pomieścić dowolną ilość aktorów takich jak postaci. Dla uproszczenia
* implementacji podjęto decyzję o&nbsp;pominięciu systemu kolizji między postaciami. Wymagałoby to m.in. zaimplementowania algorytmu poszukiwania drogi takiego
* jak algorytm Dijstry czy A*.
*
* W&nbsp;chwili obecnej świat gry składa się z&nbsp;mapy kafli o&nbsp;rozmiarze 8x8 i&nbsp;liczebności 256 x 256. Oczywiście nic nie stoi na&nbsp;przeszkodzie tworzenia dużo większych
* plansz przy zastosowaniu poprawnego algorytmu przycinania mapy do&nbsp;rozmiarów widoku
*
* @image html sample_app_tiles.jpg "Ilustracja rozmiaru kafla"
* @image latex sample_app_tiles.jpg "Ilustracja rozmiaru kafla" height=7cm
*
* Jedną z&nbsp;ciekawszych wizualnie decyzji było rozsynchronizowanie animacji postaci (zarówno prędkości animacji jak i&nbsp;punktu startowego). Uzyskano w&nbsp;ten sposób
* naturalny efekt drobnych różnic między żołnierzami, bez potrzeby angażowania dodatkowej grafiki.
*
* Jako ciekawostkę można odnotować fakt iż&nbsp;wszystkie animacje postaci w&nbsp;aplikacji testowej zostały nagrane w&nbsp;prostym systemie motion-capture stworzonym
* za&nbsp;pomocą urządzenia Kinect przez autora niniejszej pracy.
*
* \~Polish @page engine_future Spojrzenie w&nbsp;przyszłość
* Projekt w&nbsp;stanie obecnym oczywiście nie wyczerpuje oczywiście pełnej możliwej implementacji silnika gier 2D.
* Doprowadzenie go do&nbsp;zdolności produkcyjnej to wyzwanie autora na&nbsp;nadchodzące miesiące i&nbsp;lata i&nbsp;prawdopodobnie pracę magisterską.
* W&nbsp;chwili obecnej szczególnie brakuje:
*  - Ukończonego renderera; Zaimplementowana jest tylko wersja testowa oparta o&nbsp;SDL
*  - Obsługi dźwięków za&nbsp;pomocą OpenAL
*  - Komponentów wykonania. Ich obecna liczba (&nbsp;30 sztuk) zostanie prawdopodobnie zwiększona czterokrotnie w&nbsp;wersji ostatecznej
*  - Zwiększenia pokrycia przypadkami testowymi do&nbsp;poziomu 90%
*  - Wsparcia dla dodatkowych platform: Linuxa i&nbsp;OS X.
*/