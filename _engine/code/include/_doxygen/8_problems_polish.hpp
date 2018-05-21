
/**
* \~Polish @page engine_problems_di Problem z wstrzykiwaniem zależności Boost
*
* Poza problemami opisywanymi na bieżąco w poprzednich rozdziałach, problem z wstrzykiwaniem zależności Boost był najpoważniejszym
* wyzwaniem przed jakim stanął autor Pixie Engine. Problem został zgłoszony autorom biblioteki Boost DI, jednak do dziś nie powstała
* implementacja naprawcza. Istnieje natomiast implementacja omijająca, którą przygotował sam autor Pixie Engine;
* 
* Problem polegał na niewłaściwym rozwiązywaniu zależności przez fabryki abstrakcyjne (tworzenie nowych instancji singletonów), 
* co powodowało twarde zawieszenie się aplikacji na etapie tworzenia drzewka obiektów. Poniższy przykład ilustruje sytuację:
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
* Przyczyną okazało się użycie statycznej własności wewnątrz fabryki i przesuwanie wstrzykiwacza na potrzeby
* tworzenia list argumentów bez przesuwania go z powrotem.
* 
* Rozwiązanie drugiego problemu było trywialne, wymagało dodania jednej linijki kodu. Natomiast problem
* statycznej zmiennej był dużo poważniejszy. W C++ statyczna własność w klasie szablonowej istnieje raz
* dla każdego wariantu szablonu. W tym przypadku fabryka wykorzystywała jako szablon podstawę samego wstrzykiwacza.
* Oznaczało to, między innymi, iż zmiana wstrzykiwacza - nawet niewielka - generowała nową instancję obiektu (co widać
* po poprzednim przykładzie). Zmianą taką było na przykład przypięcie dodatkowych argumentów do konstruktora obiektu
* fabrykowanego. Z tego powodu ta funkcjonalność musiała zostać usunięta.
* 
* Po jej usunięciu pojawił się inny problem: wszystkie instancje obiektu singletona tworzone przez dany wstrzykiwacz
* dzieliły ten sam wskaźnik, nawet jeżeli były wyraźnie zadeklarowane jako osobne obiekty. Stało się oczywiste że
* statyczne pole musiało zostać usunięte. Wciąż jednak potrzebne było sprawdzenie obiektu po typie szablonowym
* na test istnienia. Pierwotnie rozważano formę mapy-czasu-kompilacji, ale wkrótce okazało się że było to rozwiązanie
* niemożliwe do zaimplementowania z przyczyn logicznych; W szczególności obiekty mogły być tworzone dynamicznie już po
* kompilacji (np. w formie wtyczki dll). W związku z tym jednym rozwiązaniem stała się zwykła mapa (a dokładniej niesortowana
* mapa - z przyczyn wydajnościowych) której kluczem musiała być informacja o typie szablonowym. Szczęśliwie C++ dopuszcza
* taką konstrukcję w formie elementu biblioteki standardowej `std::type_index`. Ostatnią kwestią do rozwiązania był
* problem jednego klucza: szablon musiał opierać się o dwie własności, a mapa przyjmuje tylko jeden klucz. Dlatego
* w poniższym kodzie użyto pustej klasy szablonowej żeby stworzyć nowy pojedynczy typ z dwóch typów `TInjector` i `TDependency`.
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
* \~Polish @page engine_future Spojrzenie w przyszłość
* Projekt w stanie obecnym oczywiście nie wyczerpuje oczywiście pełnej możliwej implementacji silnika gier 2D.
* Doprowadzenie go do zdolności produkcyjnej to wyzwanie autora na nadchodzące miesiące i lata - i prawdopodobnie pracę magisterską.
* W chwili obecnej szczególnie brakuje:
*  - Ukończonego renderera; Zaimplementowana jest tylko wersja testowa oparta o SDL
*  - Obsługi dźwięków za pomocą OpenAL
*  - Komponentów wykonania. Ich obecna liczba (~30 sztuk) zostanie prawdopodobnie zwiększona czterokrotnie w finałowej wersji
* 
* W pracy wykorzystano materiały pochodzące z książek Perełki Programowania Silników Gier (tomy 1 - 3) pod redakcją dr Erica Leyngela.
* Wykorzystano także Perełki Programowania Gier (tomy 1 - 2) oraz książki Wprowadzenie do Programowania OpenGL oraz
* Triki Najlepszych Programistów Gier 3D wydawnictwa Helion.
* 
* Praca powstała pod opieką dr Krzysztofa Podlaskiego. Autor chciałby także podziękować Ghassanowi Al-Mashareqa za cenne dyskusje
* (zwłaszcza w kontekście problemu z wstrzykiwaniem zależności Boost).
* 
*  - Całkowita ilość linii kodu: ~25 000 w 295 plikach źródłowych
*  - Adres projektu: https://github.com/Shelim/pixie_engine
*/