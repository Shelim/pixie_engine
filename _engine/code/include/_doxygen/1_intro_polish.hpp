/**
* \~Polish @mainpage Wprowadzenie do&nbsp;Pixie Engine
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
*     	Niniejsza praca przedstawia czwartą już iterację silnika Pixie Engine;
*       Projektu stworzonego we&nbsp;współczesnym C++, wykorzystującym najnowsze standardy i&nbsp;przestrzegającym wszystkich
*       obecnie stosowanych w&nbsp;praktyce wzorców projektowych.
*
*       Silnik jest przeznaczony do&nbsp;tworzenia gier 2D,
*       szczególnie takich których stworzenie jest trudne bądź niemożliwe używając innych silników i&nbsp;narzędzi
*       (zobacz sekcję @ref pixie_why). Ze&nbsp;względu na&nbsp;ogromny zakres tematyki i&nbsp;ograniczenia
*       czasowe, nie wszystkie elementy silnika zostały w&nbsp;tej pracy w&nbsp;pełni zaimplementowane (zobacz sekcję @ref engine_future)
*
*       Przedsięwzięcie Pixie Engine jest aktywnie rozwijane od&nbsp;2016 w&nbsp;ramach działalności Kosek.com (jednoosobowej działalności gospodarczej, założonej
*       przez autora niniejszej pracy), a&nbsp;obecna implementacja liczy sobie ponad 25 000 linii kodu w&nbsp;prawie 300 plikach źródłowych.
*       Poprzednia wersja silnika została wykorzystana w&nbsp;wydanej na&nbsp;Steam przez Kosek.com grze Magician's Apprentice.
*
* @section pixie_platforms Wspierane platformy
* @subsection pixie_platforms_win Windows Vista/7/8/8.1/10
*    - 64-bitowa architektura AMD
*    - Renderer OpenGL oraz DirectX 11.0
*    - Wsparcie dla zrzucania informacji z&nbsp;symboli debugera (na potrzeby debuggowania post-mortem)
*    - Dwie drogi implementacji dostawców: rodzajowa i&nbsp;dedykowana platformie
*    - Wsparcie dla jednoczesnego wyświetlania wielu okienek terminalowych
*
* @section pixie_why Dlaczego kolejny silnik?
*      Obecnie na&nbsp;rynku znajduje się wiele bardzo dobrych, komercyjnie-gotowych silników gier,
*      także takich o&nbsp;otwartych źródłach bądź też zamkniętych lecz posiadających permisywne@footnote{Permisywna licencja oznacza tutaj możliwość przynajmniej częściowego wykorzystania w produktach komercyjnych przy rozsądnej dla twórców niezależnych stawce cenowej} licencje.
*      Ogromna większość z&nbsp;tych narzędzi jest jednak nastawiona na&nbsp;gry akcji, zręcznościowe
*      bądź zręcznościowo-sieciowe.
*      Motywacją za&nbsp;powstaniem Pixie Engine był brak dedykowanej technologii do&nbsp;gier strategicznych,
*      fabularnych bądź wielkoskalowych.
*
*      Kuriozalnym przykładem jest wsparcie serializacji w&nbsp;najpopularniejszym obecnie silniku gier,
*      Unity – serializacja nie była w&nbsp;stanie sforsować więcej niż 5 poziomów zagłębień referencji
*      i&nbsp;próbowała alokować obiekty nawet jeżeli serializowane dane zawierały pustą referencję (`null`).
*      Z&nbsp;tego powodu poniższy przykładowy kod:
* @code{.cs}
* Class Foo
* {
*     Foo f1 = null;
*     Foo f2 = null;
*     Foo f3 = null;
* }
* Foo f = new Foo();
* // Serializuj i natychmiastnatychmiast deserializuj f
* @endcode
*      Wygeneruje dokładnie 243 (= \f$3^5\f$) alokacje klasy Foo.
*      Jeżeli rozpatrujemy grę wielkoskalową (duży graf obiektów, który wymaga strumieniowania danych z&nbsp;dysku twardego),
*      takie rozwiązanie jest całkowicie nieakceptowalne.
*
*      Ocena istniejących rozwiązań oraz ich ograniczenia zostały zebrane poniżej:
*
* @subsection pixie_why_example Problemy z&nbsp;istniejącymi rozwiązaniami
* @subsubsection pixie_why_example_unity Unity
* Unity@cite Unity to dojrzały, aktywnie rozwijany silnik 3D wykorzystujący C#/VBasic i&nbsp;obsługujący wszystkie platformy
* wspierane aktywnie przez Mono@cite Mono. Unity jest bezpłatny do&nbsp;określonego pułapu rocznych przychodów studia; 
* Potem należy zapłacić stawkę zależną od&nbsp;liczby osób zatrudnianych przez studio, ale niezależną od&nbsp;jego przychodów 
* z&nbsp;tytułu wydania gry napisanej w&nbsp;Unity.
* 
* Problemy z&nbsp;obecną wersją Unity obejmują przede wszystkim:
*    - Bardzo słaby serializator@cite UnityManualScriptSerialization (jak wspomniano wyżej)
*    - Brak wsparcia dla fanowskich modyfikacji@cite UnityGameConsiderationsforModdingSupport, czyli możliwości tworzenia dodatkowej zawartości przez fanów
*    - Z&nbsp;powodu wykorzystana języka zarządzanego: opóźnienia związane z&nbsp;automatycznym oczyszczaniem pamięci@cite UnitysgarbagecollectorWhynongenerationalandnoncompacting (Garbage Collector - zwłaszcza dla skomplikowanych grafów obiektów). Wydajność dla komputera z&nbsp;2007 roku wynosi 5 ms na&nbsp;każdy 1 MB pamięci, skaner jest uruchamiany okresowo.
*    - Duże problemy z&nbsp;grą sieciową czasu rzeczywistego (brak przewidywania martwych pakietów@cite UnityMultiplayer51UnityMultiplayerDeadReckoningUnityForum, brak interpolacji czasowej)
* 
* @subsubsection pixie_why_example_unrealengine Unreal Engine
* Unreal Engine@cite WhatisUnrealEngine4 to także dojrzały, aktywnie rozwijany silnik 3D oparty o&nbsp;C++ i&nbsp;własnościowy język UScript. W&nbsp;przeciwieństwie do&nbsp;Unity
* jest przeznaczony przede wszystkim do&nbsp;produkcji wysokobudżetowych i&nbsp;posiada gorsze wsparcie do&nbsp;prototypowania. Jest także
* nieco mniej stabilny, a&nbsp;część cech jest wciąż na&nbsp;etapie planowania i&nbsp;implementacji.
*    
* Problemy z&nbsp;obecną wersją Unreal Engine obejmują przede wszystkim:
*    - Słabe wsparcie dla prototypowania@cite WhatarethedisadvantagesofUnrealEngine4
*    - Dużo większe wymagania sprzętowe, szczególnie po&nbsp;stronie programistów i&nbsp;twórców zawartości@cite WhatarethedisadvantagesofUnrealEngine4
*    - Trudniejszy do&nbsp;nauki@cite WhatarethedisadvantagesofUnrealEngine4
*    - Niestabilny, zdarzają się twarde awarie i&nbsp;wyjście do&nbsp;pulpitu@cite WhatarethedisadvantagesofUnrealEngine4
*   
* @subsubsection pixie_why_example_ligdx LibGDX
* LibGDX@cite libgdx jest frameworkiem wspierającym tworzenie gier w&nbsp;OpenGLu na&nbsp;platformach mobilnych i&nbsp;stacjonarnych.
* Oryginalna implementacja była dedykowana platformie Android, ale późniejsze warianty wspierały też innych dostawców
* oraz komputery osobiste. LibGDX jest całkowicie darmowe i&nbsp;posiadające otwarte źródła (na licencji `apache2`).
* 
* Problemy z&nbsp;obecną wersją LibGDX obejmują przede wszystkim:
*    - Brak wsparcia dla fanowskich modyfikacji
*    - Z&nbsp;powodu wykorzystana języka zarządzanego: opóźnienia związane z&nbsp;automatycznym oczyszczaniem pamięci@cite GarbageCollectionPerformance (Garbage Collector - zwłaszcza dla skomplikowanych grafów obiektów)
*    - Duże problemy z&nbsp;budowaniem i&nbsp;wykonywaniem projektu na&nbsp;komputerach osobistych@cite PackagingaProject
*    - Bardzo proste i&nbsp;zarazem ograniczone API; Wymaga często 'surowych' komend OpenGLa@cite OverviewlibgdxAPI
* 
* @subsubsection pixie_why_example_c4 C4 (Terathon)
* C4@cite C4EngineWikipedia firmy Terathon to nowoczesny, dojrzały silnik napisany całkowicie w&nbsp;kodzie natywnym (C++) i&nbsp;wspierający wszystkie popularne
* platformy stacjonarne oraz konsole gier. Narzędzie wspiera wszystkie popularne wzorce projektowe i&nbsp;posiada zaawansowane edytory.
* Jest to także jedyny silnik na&nbsp;rynku implementujący przenikanie poziomów detalu map wokselowych.
* 
* Obecnie jedynie komercyjne licencje C4 są dostępne.
* 
* Problemy z&nbsp;obecną wersją C4 obejmują przede wszystkim:
*    - Brak wsparcia dla fanowskich modyfikacji
*    - Brak darmowej licencji
*    - Brak temporalnego wygładzenia grafu sceny
*    - Podejście serwer-pierwszy przy implementacji gry wieloosobowej (wprowadzające opóźnienia przy akceptowaniu poleceń)
*
* @subsection pixie_why_us Odpowiedź na&nbsp;problemy
* Pixie Engine wychodzi naprzeciw opisanym problemom dostarczając zarówno pełne wsparcie dla grup modyfikujących
* (uwzględniając @ref assets_hotreload "przeładowanie zasobów na&nbsp;gorąco"), maksymalne wykorzystanie języka natywnego
* (w celu osiągnięcia maksymalnej wydajności gdy tylko jest to możliwe), wsparcie dla skryptowych języków zarządzanych
* (krótkie kody które korzystają z&nbsp;zalet automatycznego oczyszczania pamięci), wsparcie dla różnych menadżerów sceny
* (uwzględniający przewidywanie martwych pakietów, interpolację czasową i&nbsp;temporalne wygładzenie) oraz dostępu do
* natywnych narzędzi gdy tylko jest to możliwe.
*
* @subsection pixie_why_limitations Ograniczenia silnika
* Oczywiście w&nbsp;sytuacji rywalizujących zasobów niemożliwe jest zawarcie wszystkich możliwych cech w&nbsp;jednym rozwiązaniu.
* Z&nbsp;tego powodu z&nbsp;góry przyjęto ograniczenia, jakie wprowadzono do&nbsp;Pixie Engine:
*    - Akceptowalne czasy kompilacji; W&nbsp;chwili obecnej około 15 minut pracy jednego rdzenia@footnote{Domyślnie kompilator wykorzystuje wszystkie dostępne w komputerze rdzenie} (np. &nbsp;3 minuty 45 sekund przy 4 rdzeniach)
*    - Preferuj kod natywny nad zarządzanym (ograniczenia modyfikacji w&nbsp;zamian za&nbsp;zwiększenie wydajności)
*    - Wykorzystuj inteligentne wskaźniki (niewielki narzut na&nbsp;wykonanie w&nbsp;zamian za&nbsp;stabilność implementacji)
*    - Preferuj ustalenia czasu kompilacji nad ustawieniami czasu wykonania (przyspiesza wykonanie poprzed eliminację zbędnego kodu na&nbsp;etapie kompilacji kosztem utraty modyfikowalności niektórych ustawień)
*    - Akceptuj narzut na&nbsp;uruchomienie związany z&nbsp;wstrzykiwaniem zależności@cite BenchmarksBoostDI
*    - Korzystaj z&nbsp;wstrzykiwania zależności i&nbsp;warstw pośrednich, nawet jeżeli zależności między modułami stają się bardziej widoczne (efektywnie zwiększa znacząco współczynnik utrzymywalności rozwiązania, kosztem niewielkiego narzutu)
*
* @section pixie_license Licencja
*
* Licencja MIT
*
* Copyright &copy; 2016 - 2018 Piotr Kosek
*
* Niniejszym gwarantuje się, bez opłat, że&nbsp;każda osoba która wejdzie w&nbsp;posiadanie kopii tego
* oprogramowania i&nbsp;związanych z&nbsp;nim plików dokumentacji (dalej „Oprogramowanie”) może
* wprowadzać do&nbsp;obrotu Oprogramowanie bez żadnych ograniczeń, w&nbsp;tym bez ograniczeń
* prawa do&nbsp;użytkowania, kopiowania, modyfikowania, łączenia, publikowania,
* dystrybuowania, sublicencjonowania i/lub sprzedaży kopii Oprogramowania a&nbsp;także
* zezwalania osobie, której Oprogramowanie zostało dostarczone czynienia tego samego, z
* zastrzeżeniem następujących warunków:
*
* Powyższa nota zastrzegająca prawa autorskie oraz niniejsza nota zezwalająca muszą zostać
* włączone do&nbsp;wszystkich kopii lub istotnych części Oprogramowania.
*
* OPROGRAMOWANIE JEST DOSTARCZONE TAKIM, JAKIE JEST, BEZ JAKIEJKOLWIEK GWARANCJI,
* WYRAŹNEJ LUB DOROZUMIANEJ, NIE WYŁĄCZAJĄC GWARANCJI PRZYDATNOŚCI HANDLOWEJ LUB
* PRZYDATNOŚCI DO&nbsp;OKREŚLONYCH CELÓW A&nbsp;TAKŻE BRAKU WAD PRAWNYCH. W&nbsp;ŻADNYM
* PRZYPADKU TWÓRCA LUB POSIADACZ PRAW AUTORSKICH NIE MOŻE PONOSIĆ
* ODPOWIEDZIALNOŚCI Z&nbsp;TYTUŁU ROSZCZEŃ LUB WYRZĄDZONEJ SZKODY A&nbsp;TAKŻE ŻADNEJ INNEJ
* ODPOWIEDZIALNOŚCI CZY TO WYNIKAJĄCEJ Z&nbsp;UMOWY, DELIKTU, CZY JAKIEJKOLWIEK INNEJ
* PODSTAWY POWSTAŁEJ W&nbsp;ZWIĄZKU Z&nbsp;OPROGRAMOWANIEM LUB UŻYTKOWANIEM GO LUB
* WPROWADZANIEM GO DO&nbsp;OBROTU. 
*
* @subsection pixie_license_copyrights Prawa autorskie
*    Ten silnik jest napisany i&nbsp;utrzymywany przez Kosek.com i&nbsp;jest dystrybuowany na&nbsp;otwartej licencji MIT/X11. Najnowsza wersja jest dostępna zawsze pod adresem https://github.com/Shelim/pixie_engine
*/
