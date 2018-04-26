/**
* \~Polish @mainpage Wprowadzenie do Pixie Engine
*
* @verbatim
*  #############################################################\
*  #  _____ _      _        ______             _              # |
*  # |  __ (_)    (_)      |  ____|           (_)             # |
*  # | |__) |__  ___  ___  | |__   _ __   __ _ _ _ __   ___   # |
*  # |  ___/ \ \/ / |/ _ \ |  __| | '_ \ / _` | | '_ \ / _ \  # |
*  # | |   | |>  <| |  __/ | |____| | | | (_| | | | | |  __/  # |
*  # |_|   |_/_/\_\_|\___| |______|_| |_|\__, |_|_| |_|\___|  # |
*  #                                      __/ |               # |
*  #                                     |___/   Version 4.0  # |
*  ############################################################ |
*  \___________________________________________________________\|
*
*
*  "Nie podążaj dokąd prowadzi cię ścieżka;
*  Idź tam gdzie jej nie ma
*  i samemu przetrzyj szlak"
*
*                       Ralph Waldo Emerson
* @endverbatim
*
* @section pixie_intro Wprowadzenie
*     	Niniejsza praca (dokumentacja, implementacja oraz przykładowa aplikacja) przedstawia czwartą już iterację silnika Pixie Engine;
*       Projektu stworzonego we współczesnym C++, wykorzystującym najnowsze standardy i przestrzegającym wszystkich
*       obecnie stosowanych w praktyce wzorców projektowych.
*
*       Silnik jest przeznaczony do tworzenia gier 2D (choć nie jest wykluczone dodanie renderera 3D),
*       szczególnie takich których stworzenie jest trudne bądź niemożliwe używając innych silników i narzędzi
*       (zobacz sekcję @ref pixie_why "Dlaczego kolejny silnik?"). Ze wzlędu na ogromny zakres tematyki i ograniczenia
*       czasowe, nie wszystkie elementy silnika zostały w tej pracy w pełni zaimplementowane (zobacz sekcję @ref pixie_missing "Brakujące elementy")
*
*       Przedsięwzięcie Pixie Engine jest aktywnie rozwjane od 2016 w ramach działalności Kosek.com (jednoosobowej działalności gospodarczej, założonej
*       przez autora niniejszej pracy), a obecna implementacja liczy sobie ponad 25 000 linii kodu w ponad 300 plikach źródłowych.
*       Poprzednia wersja silnika została wykorzystana w wydanej na Steam przez Kosek.com grze Magician's Apprentice.
*
* @section pixie_platforms Wspierane platformy
* @subsection pixie_platforms_win Windows Vista/7/8/8.1/10
*    - 64-bitowa architektura AMD
*    - Renderer OpenGL oraz DirectX 11.0
*    - Wsparcie dla zrzucania informacji z symboli odpluskwiacza (na potrzeby odpluskwiania post-mortem)
*    - Dwie drogi implementacji dostawców: rodzajowa i dedykowana platformie
*    - Wsparcie dla jednoczesnego wyświetlania wielu okienek terminalowych
* @subsection pixie_platforms_lin Linux
*    (Spodziewana, jeszcze nie zaimplementowana)
* @subsection pixie_platforms_osx OS X
*    (Spodziewana, jeszcze nie zaimplementowana)
*
* @section pixie_why Dlaczego kolejny silnik?
*      Obecnie na rynku znajduje się wiele bardzo dobrych, komercyjnie-gotowych silników gier,
*      także takich o otwartych źródłach bądź też zamkniętych lecz posiadających permisywne licencje.
*      Ogromna większość z tych narzędzi jest jednak nastawiona na gry akcji, gry zręcznościowe
*      bądź zręcznościowo-sieciowe.
*      Motywacją za powstaniem Pixie Engine był brak dedykowanej technologii do gier strategicznych,
*      fabularnych bądź wielkoskalowych.
*
*      Kuriozalnym przykładem jest wsparcie serializacji w najpopularniejszym obecnie silniku gier,
*      Unity – serializacja nie była w stanie sforsować więcej niż 5 poziomów zagłębień referencji
*      i próbowała alokować obiekty nawet jeżeli serializowane dane zawierały pustą referencję (`null`).
*      Z tego powodu poniższy przykładowy kod:
* @code{.cs}
* Class Foo
* {
*     Foo f1 = null;
*     Foo f2 = null;
*     Foo f3 = null;
* }
* Foo f = new Foo();
* // Serializuj i natychmiast deserializuj f
* @endcode
*      Wygeneruje dokładnie 243 (= \f$3^5\f$) alokacje klasy Foo.
*      Jeżeli rozpatrujemy grę wielkoskalową (duży graf obiektów, który wymaga strumieniowania danych z dysku twardego),
*      takie rozwiązanie jest całkowicie nieakceptowalne.
*
*      Wywiad środowiska i ograniczenia istniejących silników zostały zebrane poniżej:
*
* @subsection pixie_why_example Problemy z istniejącymi rozwiązaniami
* @subsubsection pixie_why_example_unity Unity
* Unity to dojrzały, aktywnie rozwijany silnik 3D wykorzystujący C#/VBasic i obsługujący wszystkie platformy
* wspierane aktywnie przez Mono. Unity jest bezpłatny do określonego pułapu rocznych przychodów studia; 
* Potem należy zapłacić stawkę zależną od liczby osób zatrudnianych przez studio, ale niezależną od jego przychodów 
* z tytułu wydania gry napisanej w Unity.
* 
* Problemy z obecną wersją Unity obejmują przede wszystkim:
*    - Bardzo słaby serializator (jak wspomniano wyżej)
*    - Brak wsparcia dla fanowskich modyfikacji, czyli możliwości tworzenia dodatkowej zawartości przez fanów
*    - Z powodu wykorzystana języka zarządzanego: opóźnienia związane z odśmieciarką (zwłaszcza dla skomplikowanych grafów obiektów)
*    - Duże problemy z grą sieciową czasu rzeczywistego (brak przewidywania martwych pakietów, brak interpolacji czasowej)
* 
* @subsubsection pixie_why_example_unrealengine Unreal Engine
* Unreal Engine to także dojrzały, aktywnie rozwijany silnik 3D oparty o C++ i własnościowy język UScript. W przeciwieństwie do Unity
* jest przeznaczony przede wszystkim do produkcji wysokobudżetowych i posiada gorsze wsparcie do prototypowania. Jest także
* nieco mniej stabilny, a część cech jest wciąż na etapie planowania i implementacji.
*    
* Problemy z obecną wersją Unreal Engine obejmują przede wszystkim:
*    - Słabe wsparcie dla prototypowania
*    - Dużo większe wymagania sprzętowe, szczególnie po stronie programistów i twórców zawartości
*    - Trudniejszy do nauki
*    - Niestabilny, zdarzają się twarde awarie i wyjście do pulpitu
*   
* @subsubsection pixie_why_example_ligdx LibGDX
* LibGDX jest frameworkiem wspierającym tworzenie gier w OpenGLu na platformach mobilnych i stacjonarnych.
* Oryginalna implementacja była dedykowana platformie Android, ale późniejsze warianty wspierały też innych dostawców
* oraz komputery osobiste. LibGDX jest całkowicie darmowe i posiadające otwarte źródła (na licencji `apache2`).
* 
* Problemy z obecną wersją LibGDX obejmują przede wszystkim:
*    - Brak wsparcia dla fanowskich modyfikacji
*    - Z powodu wykorzystana języka zarządzanego: opóźnienia związane z odśmieciarką (zwłaszcza dla skomplikowanych grafów obiektów)
*    - Duże problemy z budowaniem i wykonywaniem projektu na komputerach osobistych
*    - Bardzo proste i zarazem ograniczone API; Wymaga często 'surowych' komend OpenGLa
* 
* @subsubsection pixie_why_example_c4 C4 (Terathon)
* C4 firmy Terathon to nowoczesny, dojrzały silnik napisany całkowicie w kodzie natywnym (C++) i wspierający wszystkie popularne
* platformy stacjonarne oraz konsole gier. Narzędzie wspiera wszystkie popularne wzorce projektowe i posiada zaawansowane edytory.
* Jest to także jedyny silnik na rynku implementujący przenikanie poziomów detalu map wokselowych.
* 
* Obecnie jedynie komercyjne licencje C4 są dostępne.
* 
* Problemy z obecną wersją C4 obejmują przede wszystkim:
*    - Brak wsparcia dla fanowskich modyfikacji
*    - Brak darmowej licencji
*    - Brak temporalnego wygładzenia grafu sceny
*    - Podejście serwer-pierwszy przy implementacji gry wieloosobowej (wprowadzające opóźnienia przy akceptowaniu poleceń)
*
* @subsection pixie_why_us Odpowiedź na problemy
* Pixie Engine wychodzi naprzeciw opisanym problemom dostarczając zarówno pełne wsparcie dla grup modyfikujących
* (uwzględniając @ref assets_hotreload "przeładowanie zasobów na gorąco"), maksymalne wykorzystanie języka natywnego
* (w celu osiągnięcia maksymalnej wydajności gdy tylko jest to możliwe), wsparcie dla skryptowych języków zarządzanych
* (krótkie kody które korzystają z zalet automatycznej odśmieciarki), wsparcie dla różnych menadżerów sceny
* (uwzględniający przewidywanie martwych pakietów, interpolację czasową i temporalne wygładzenie) oraz dostępu do
* natywnych narzędzi gdy tylko jest to możliwe. Dokładny opis tych elementów znajduje się w rodziale @ref pixie_features "cechy".
*
* @subsection pixie_why_limitations Ograniczenia silnika
* Oczywiście w sytuacji rywalizujących zasobów niemożliwe jest zawarcie wszystkich możliwych cech w jednym rozwiązaniu.
* Z tego powodu z góry przyjęto ograniczenia, jakie wprowadzono do Pixie Engine:
*    - Akceptowalne czasy kompilacji; W chwili obecnej około 15 minut pracy jednego rdzenia (np. ~3 minuty 45 sekund przy 4 rdzeniach)
*    - Preferuj kod natywny nad zarządzanym (ograniczenia modyfikacji w zamian za zwiększenie wydajności)
*    - Wykorzystuj inteligentne wskaźniki (niewielki narzut na wykonanie w zamian za stabilność implementacji)
*    - Preferuj ustalenia czasu kompilacji nad ustawieniami czasu wykonania (przyspiesza wykonanie poprzed eliminację zbędnego kodu na etapie kompilacji kosztem utraty modyfikowalności niektórych ustawień)
*    - Akceptuj narzut na uruchomienie związany z wstrzykiwaniem zależności
*    - Korzystaj z wstrzykiwania zależności i warstw pośrednich, nawet jeżeli zależności między modułami stają się bardziej widoczne (efektywnie zwiększa znacząco współczynnik utrzymywalności rozwiązania, kosztem niewielkiego narzutu)
*
* @section pixe_copyrights Prawa autorskie
*    Ten silnik jest napisany i utrzymywany przez Kosek.com i jest dystrybuowany na otwartej licencji MIT/X11. Najnowsza wersja jest dostępna zawsze pod adresem https://github.com/Shelim/pixie_engine
*/
