/**
* \~Polish @page depedencies Zależności
* @section dependency_dashboard_into Wprowadzenie
* W~katalogu <tt>_engine/dependency</tt> w~jednym z~dwóch podkatalogów (@c library albo @c other) zostały umieszczone wszelkie biblioteki niezbędne do działania dla silnika.
*
* Jeżeli twój katalog <tt>_engine/dependency</tt> jest pusty, oznacza to najprawdopodobniej wykonanie opereracji <tt>git clone</tt> z~publicznego repozytorium bez ściągnięcia zależności.
* Dzieje się tak dlatego iż zależności liczą sobie około 4,5 gb i~nie są ściągane domyślnie. Należy użyć następujących komend
* aby pobrać je z~serwera Kosek.com:
*
* - Na Windowsie: użyj skryptu z~katalogu <tt>_command/windows/download_dependencies.bat</tt>. Potwierdź czas oczekiwania.
*
* Lista zależności znajduje się poniżej:
*
* @section dependency_dashboard Pełna lista
* <center><table>
* <caption id="dependency_dashboard_table">Lista wszystkich zależności</caption>
* <tr><th>Nazwa<th>Rodzaj<th>Rola w~projekcie<th>Licencja<th>Twórca
* <tr><td>@ref dependency_angelcode AngelCode<td>Biblioteka<td>Dostawca języka skryptowego<td>zlib<td>Andreas J&ouml;nsson
* <tr><td>@ref dependency_anttweakbar AntTweakBar<td>Biblioteka<td>Dostawca podstawowych narzędzi poprawiania UI<td>zlib/libpng<td>Philippe Decaudin
* <tr><td>@ref dependency_apacheant Apache Ant<td>Inne<td>Wieloplaformowy system kompilacji<td>Licencja Apache, wersja 2.0<td>The Apache Software Foundation
* <tr><td>@ref dependency_blowfish Blowfish<td>Biblioteka<td>Szybki moduł szyfrujący<td>The Microsoft Public License (Ms-PL)<td>George Anescu
* <tr><td>@ref dependency_boost Boost<td>Biblioteka<td>Zestaw nagłówków wspierających tworzenie aplikacji C++<td>Licencja Boost Software, wersja 1.0<td>Open Source Contributors
* <tr><td>@ref dependency_di Boost Dependency Injection<td>Biblioteka<td>Zestaw narzędzi wstrzykujący zależności<td>Licencja Boost Software, wersja 1.0<td>Kris Jusiak
* <tr><td>@ref dependency_cereal cereal<td>Biblioteka<td>Narzędzia serializacji<td>Licencja BSD<td>Randolph Voorhies, Shane Grant
* <tr><td>@ref dependency_cg cg<td>Biblioteka<td>Język jednostki ceniującej GPU<td>Wykorzystanie klienckie NVIDIA Software<td>NVIDIA
* <tr><td>@ref dependency_cppcheck C++ Check<td>Inne<td>Statyczny walidator kodu C++<td>GNU GPL (Bez włączenia kodów źródłowych do projektu)<td>Daniel Marjam&auml;ki
* <tr><td>@ref dependency_debugbreak debugbreak<td>Biblioteka<td>Wieloplatformowe programowe instrukcje przerwania<td>MIT/X11<td>Scott Tsai
* <tr><td>@ref dependency_dia dia<td>Inne<td>(Używany wewnętrznie przez LLVM)<td>GNU GPL (Bez włączenia kodów źródłowych do projektu)<td>Open Source Contributors
* <tr><td>@ref dependency_dot dot<td>Inne<td>(Używany wewnętrznie przez doxygen)<td>The Common Public License (Bez włączenia kodów źródłowych do projektu)<td>AT &amp;T
* <tr><td>@ref dependency_doxygen doxygen<td>Inne<td>Narzędzia do generowania dokumentacji<td>GNU GPL (Bez włączenia kodów źródłowych do projektu)<td>Dimitri van Heesch
* <tr><td>@ref dependency_fastbuild FastBuild<td>Inne<td>Akcelelator systemu budowania C++<td>Wariant BSD<td>Franta Fulin 
* <tr><td>@ref dependency_git git<td>Inne<td>System kontroli wersji<td>GNU LGPL v2.1<td>Linus Torvalds, Junio Hamano and others
* <tr><td>@ref dependency_glew glew<td>Biblioteka<td>Rozszerzenia dla OpenGL<td>Licencja Khronos (MIT)<td>Lev Povalahev and others
* <tr><td>@ref dependency_googlemock Google Mock<td>Biblioteka<td>Automatyczne testy makietowe<td>BSD 3-punktowa<td>Google Inc.
* <tr><td>@ref dependency_googletest Google Test<td>Biblioteka<td>Automatyczne testy jednostkowe<td>BSD 3-punktowa<td>Google Inc.
* <tr><td>@ref dependency_gtest_runner Google Test runner<td>Inne<td>Host automatycznych testów jednostkowych<td>MIT/X11<td>Nic Holthaus
* <tr><td>@ref dependency_html_help_workshop HTML Help Workshop<td>Inne<td>Narzędzie do generowania dokumentacji chm dla narzędzia doxygen<td>Licencja końcowego użytkownika HTML Help<td>Microsoft
* <tr><td>@ref dependency_levenshtein Levenshtein distance calculator<td>Biblioteka<td>Obliczanie odległości Levenshteina między dwoma dowolnymi kolekcjami<td>Domena publiczna<td>Keith Schwarz
* <tr><td>@ref dependency_jdk JDK 1.8<td>Inne<td>Wymagane przez Apache Ant<td>Licencja Sun<td>Oracle Corporation
* <tr><td>@ref dependency_llvm LLVM<td>Inne<td>LLVM: środowisko kompilujące Pixie Engine (kompilator, linker, debugger)<td>Licencja LLVM (Bez włączenia kodów źródłowych do projektu)<td>LLVM Team
* <tr><td>@ref dependency_mathjax mathjax<td>Inne<td>Renderer wyrażeń matematycznych w~przeglądarce<td>Licencja Apache, wersja 2.0<td>American Mathematical Society &amp; Society for Industrial and Applied Mathematics
* <tr><td>@ref dependency_vlc libVLC<td>Biblioteka<td>Odtwarzanie filmów<td>GNU LGPL 2<td>VideoLAN
* <tr><td>@ref dependency_minizip miniZip<td>Biblioteka<td>Kompresor/dekompresor archiwów<td>Wariant BSD<td>Nathan Moinvaziri
* <tr><td>@ref dependency_mingw MinGW<td>Inne<td>Kompilator plików zasobów na platformie Windows<td>MIT/X11<td>Open Source Contributors
* <tr><td>@ref dependency_msxsl msxsl<td>Inne<td>Transformator XML / XSLT działający z~wiersza polecenia<td>Tylko binarne<td>msxsl
* <tr><td>@ref dependency_multiconsoles Multiple Consoles<td>Biblioteka<td>Pomocnicza aplikacja umożliwiająca alokację więcej niż jednego okna konsoli dla jednego procesu<td>Domena publiczna<td>Zvika Ferentz
* <tr><td>@ref dependency_nsis NSIS<td>Inne<td>Instalator na platformie Windows<td>zlib/libpng<td>Open Source Contributors
* <tr><td>@ref dependency_openal_soft OpenAL (Software)<td>Biblioteka<td>Moduł wspierający odtwarzanie dźwięków<td>GNU LPGL<td>kcat
* <tr><td>@ref dependency_open_cpp_coverage Open C++ Coverage<td>Inne<td>Aplikacja wyliczająca pokrycie kodu testami na podstawie symboli debugowania PDB<td>GNU GPL (Bez włączenia kodów źródłowych do projektu)<td>OpenCppCoverage Team
* <tr><td>@ref dependency_pugixml pugixml<td>Biblioteka<td>Szybki wczytywacz i~parser dokumentów XML<td>MIT/X11<td>Arseny Kapoulkine
* <tr><td>@ref dependency_python Python 3.5<td>Inne<td>Używany przez LLVM na potrzeby debugowania aplikacji<td>PSF license agreement for Python<td>Python Software Foundation
* <tr><td>@ref dependency_remotery Remotery<td>Biblioteka<td>Zewnętrzny profiler w~formie strony www<td>Licencja Apache, wersja 2.0<td>Don Williamson
* <tr><td>@ref dependency_sdl SDL 2.0<td>Biblioteka<td>Wieloplatformowa baza aplikacji<td>zlib<td>Open Source Contributors
* <tr><td>@ref dependency_sdl_net SDL NET 2.0<td>Biblioteka<td>Wieloplatformowe wsparcie dla sieci<td>GNU LGPL<td>Open Source Contributors
* <tr><td>@ref dependency_stb stb<td>Biblioteka<td>Proste pojedynczo nagłówkowe narzędzia<td>Domena publiczna<td>Sean Barrett
* <tr><td>@ref dependency_texlive texLIVE<td>Inne<td>Kompilator dokumentacji do formatu PDF<td>Licencja TL<td>Karl Berry
* <tr><td>@ref dependency_thttpd thttpd<td>Inne<td>Prosty serwer HTTP do testowania funkcjonalności aktualizacji<td>Wariant BSD<td>Jef Poskanzer
* <tr><td>@ref dependency_upx UPX<td>Inne<td>Uniwersalny kompresor plików binarnych, zmniejszający rozmiar plików wykonywalnych<td>Wariant BSD<td>Markus F.X.J. Oberhumer, L&aacute;szl&oacute; Moln&aacute;r & John F. Reiser.
* <tr><td>@ref dependency_vscode Visual Studio Code<td>Inne<td>Główny edytor w~którym powstaje Pixie Engine<td>MIT/X11<td>Microsoft
* <tr><td>@ref dependency_wxwidgets wxWidgets<td>Biblioteka<td>GUI dla narzędzi<td>Licencja wxWindows<td>Open Source Contributors
* <tr><td>@ref dependency_windbg WinDBG (Debugging Tools for Windows)<td>Inne<td>Debugger wiersza poleceń podpięty do projektu Visual Studio Code<td>Tylko binarne<td>Microsoft
* </table></center>
*
* @section dependency_angelcode AngelCode
* Angelscript@cite AngelScript (albo Biblioteka Skryptowa AngelCode) to dojrzały, aktywnie rozwijany język skryptowy o~składni opartej częściowo
* o~C++ udostępniany na wolnej licencji zlib.
* 
* Wybór konkretnego języka skryptowego nie jest łatwy w~dzisiejszych czasach z~racji obecności na rynku wielu
* dojrzałych i~gotowych-do-produkcji rozwiązań. W~szczególności mogłoby zostać zadane pytanie czemu nie została wybrana Lua albo Python,
* najpopularniejsze obecnie języki skryptowe stosowane w~grach; Prawdą jest, że o~ile wydajnościowo Lua nieznacznie sprawuje
* się lepiej niż AngelScript, Angelscript z~racji swojej składni zbliżonej do C++ jest nieporównywalnie łatwiejszy do podpięcia do natywnego
* systemu klas (zwykle także z~pominięciem konieczności istnienia obiektów proxy). Python jako język pod względem
* wydajności sprawdza się o~wiele gorzej a~i jego zdolność do odwzorowania konstrukcji C++ jest dużo bardziej ograniczona.
* 
* <center><table>
* <caption id="dependency_angelcode_benchmark_table">Wydajność języków skryptowych w~przebiegu testowym</caption>
* <tr><th>Nazwa<th>Czas wykonania<th>Względem Lua
* <tr><td>Lua<td>1,341 s.<td>100%
* <tr><td>Angelscript<td>1,859 s.<td>72%
* <tr><td>Python<td>2,842 s.<td>47%
* </table>źródło: https://github.com/r-lyeh-archived/scriptorium</center>
*
* @section dependency_anttweakbar AntTweakBar
* AntTweakBar@cite AntTweakBar to niewielka, łatwa do wykorzystania biblioteka która pozwala programistom i~twórcom zawartości dodać prosty interfejs użytkownika
* na potrzeby interaktywnego dobierania parametrów. Biblioteka pozwala podpiąć dowolną zmienną C/C++ i~umożliwia mechanizmy prostej modyfikacji
* wartości, na podstawie rozpoznanego typu (liczba, test, wektor, kolor, macierz, kwaternion). Biblioteka ta wspiera wszystkie współczesne stosowane
* renderery (OpenGL, DirectX, SDL)@cite howtoAntTweakBar
* 
* @image html dependency_anttweakbar_image.jpg "Przykładowy zrzut ekranu AntTweakBar"
* @image latex dependency_anttweakbar_image.jpg "Przykładowy zrzut ekranu AntTweakBar" height=5cm
* 
* Motywacją za włączeniem AntTweakBar było udostępnienie prostego narzędzia, które pozwoli parametryzować rozgrywkę (głównie do wykorzystania
* przy tworzeniu fanowski modyfikacji) bez opuszczania gry.
*
* @section dependency_apacheant Apache Ant
* Apache Ant@cite ApacheAnt to system budujący oparty o~język XML zaimplementowany w~Javie z~wykorzystaniem komend języka skryptowego JavaScript. Posiada rozbudowaną
* bazę zadań (np. "wykonaj komendę na pliku", "skopiuj plik", "wgraj plik na serwer FTP", etc.), które można parametryzować z~wykorzystaniem zmiennych.
*
* Istnieje na rynku wiele komercyjnie-gotowych systemów budujących, poczynając od makefile i~cmake - bardzo dojrzałych narzędzi operujących na skryptach
* budowy, idąc przez SCONS oparty o~pełnoprawny język skryptowy Python, a~kończąc na Fastbuild (wykorzystany w~Pixie Engine do budowania iteracyjnego)
* i~rozwiązaniach wewnętrznych. Rozwiązania różnią się filozofią tworzenia plików skryptowych, ale jako że różnice są bardziej kosmetyczne niż efektywne,
* wybór konkretnego rozwiązania był podyktowany preferencją autora i~względami historycznymi (wszystkie wersje Pixie Engine opierały się na Apache Ant,
* z~czego wynikała istniejąca baza skryptów którą można było po prostu zaadoptować do obecnego projektu)
*
* W~Pixie Engine jest włączona jedynie binarna dystrybucja Apache Ant.  
*
* @section dependency_blowfish Blowfish
* System szyfrujący Blowfish@cite Blowfishcipher jest uznanym algorytmem blokowym operującym na 8-bajtowych blokach danych (64 bity).
* Algorytm opiera się o~dwa przebiegi: rozwijanie klucza i~szyfrowanie danych. Klucz może zawierać od 1 do 56 znaków i~jest rozwijany do 4168 bajtów.
* Blowfish opiera się na 16 rundach kryptograficznych, a~siła algorytmu bierze się z~zastosowania nieliniowej operacji S-Block
*
* W~Pixie Engine blowfish został włączony ze względu na potrzebę szybkiego, symetrycznego algorytmu szyfrującego dane, niezbędne do przesłania
* przez sieć, bądź zapisania na dysku. Wybór konkretnego algorytmu był podyktowany względami dostępności - praktycznie każdy współczesny symetryczny szyfr blokowy
* spełniałby wymogi stawiane przed algorytmem, ale istnieje prosta, efektywna i~oparta o~wolną licencję Ms-PL implementacja Blowfisha
*
* @section dependency_boost Boost - rozszerzone biblioteki C++
* Jako iż standardowa biblioteka C++, nawet rozszerzona o~funkcje C++11 i~C++14 zawiera relatywnie ograniczony zestaw narzędzi, Pixie Engine wymaga
* rozszerzonej nieoficjalnej biblioteki Boost@cite BoostCLibraries. Boost jest dojrzałą, aktywnie rozwijaną i~recenzowaną przez specjalistów biblioteką - lub raczej zbiorem bibliotek
* umożliwiającą - i~optymalizującą - wiele typowych operacji jakie można spotkać podczas tworzenia oprogramowania w~C++.
* Pełna lista komponentów Boost jest dostępna tutaj: https://www.boost.org/doc/libs/
*
* Na ten moment Pixie Engine korzysta m.in. z~komponentu Spirit, umożliwającego parsowanie wyrażeń tekstowych, inteligentnych wskaźników oraz wyrażeń czasowych.
* Kolejne komponenty będą integrowane w~miarę potrzeb.
*
* @section dependency_di Boost Dependency Injection
* Biblioteka wstrzykiwania zależności@cite BoostDI jest propozycją rozszerzenia Boost (obecnie oczekującą na recenzenta), ale do tej pory nie została włączona w~zestaw bibliotek Boost
* i~pozostaje niezależnym produktem. Jest to relatywnie młoda biblioteka, wciąż aktywnie rozwijana przez twórców. Więcej informacji na temat wykorzystania
* wstrzkiwania zależności i~wykorzystania wzorca odwrócenia odpowiedzialności w~Pixie Engine znajduje się w~rozdziale @ref engine_architecture "architektura silnika".
*
* Podczas praktycznego wykorzystania wstrzykiwania zależności Boost natrafiono na ciekawy problem z~tworzeniem fabryk abstrakcyjnych. Opis problemu i~rozwiązanie
* zaimplementowane przez twórcę Pixie Engine znajduje się w~rozdziale @ref engine_problems_di "problem z~wstrzykiwaniem zależności Boost".
*
* @section dependency_cereal cereal
* cereal@cite cereal (oficjalnie nazwa pisana z~małej litery) to biblioteka łatwej serializacji danych C++ czyli zamiany drzewka referencji na strumień binarny bądź tekstowy
* i~vice versa. Cereal zostało wybrane ze względu na nieinwazyjną składnię, rozpoznawanie polimorfizmu (dzięki mechanizmom RTTI - rozpoznawania typów w~trakcie
* wykonania), relatywnie proste API, dużą wydajność oraz łatwość implementacji nowych strumieni wyjściowych. Jego największym ograniczeniem jest problem
* z~tworzeniem paska postępu zapisywania/wczytywania danych - istnieje tylko jeden przebieg i~niewielkie możliwości podczepiania zdarzenia
* pod wykrycie nowego obiektu (bez opcji detekcji liczby obiektów). Z~tego powodu w~praktycznym zastosowaniu cereal musi być uruchamiany
* dwukrotnie (pierwszy przebieg jałowy, bez strumieniowania danych tylko po to by policzyć liczbę obiektów w~grafie, i~drugi - faktycznie serializujący)
*
* @section dependency_cg cg
* cg@cite CgprogramminglanguageWikipedia (oficjalnie nazwa pisana z~małej litery) to autorska biblioteka firmy nVidia, nierozwijana i~nieutrzymywana od kwietnia 2012, ale dostatecznie
* dojrzała i~stabilna by zyskać szerokie zastosowania. cg to abstrakcyjny język jednostki cieniującej, umożliwiający transkompilację do kodu GLSL oraz HLSL
* z~wykorzystaniem konkretnych list cech danej wersji języka. W~ten sposób programy jednostki cieniującej mogą zostać stworzone przez grafika jednorazowo
* i~być od razu dostępne na wszystkich wspieranych rendererach. Uzyskuje się w~ten sposób warstwę niezależności zasobów nad konkretną implementacją.
* 
* Pomimo iż biblioteka nie jest dłużej wspierania, jest wciąż powszechnie wykorzystywana w~branży, np. pełną integrację z~cg oferuje Unity.
* Ponieważ cg nie jest już dłużej rozwijane, nie oferuje on dostępu do podprogramów i~procedur GLSL oraz HLSL powstałych po kwietniu 2012.
*
* @section dependency_cppcheck C++ Check
* Jednym z~najważniejszych wyzwań stawianych przed współczesnym oprogramowaniem jest jego stabilność. Testy możemy z~grubsza podzielić na ręczne -
* podczas których tester fizycznie próbuje "popsuć" program i~zapamiętuje kroki niezbędne by taką sytuację powtórzyć, oraz automatyczne - gdy inny program
* stara się "popsuć" nasz program. Istnieje wiele podejść do testów automatycznych, np. testy jednostkowe, które sprawdzają rezultat konkretnego
* fragmentu kodu, testy makietowe (zaimplementowane w~Pixie Engine przy użyciu biblioteki Google Mockup) które opierają
* się o~weryfikację komponentu w~otoczeniu makiet innych komponentów, oraz testy integracyjne, które opierają się na całym systemie.
*
* Osobną gałąź stanowią testy statyczne, oparte wyłącznie na maszynowej analizie kodu źródłowego z~pominięciem fazy kompilacji i~uruchomienia programu.
* Testy statyczne mogą być oparte o~ręczną analizę, ale powstały narzędzia zdolne przeprowadzić taką analizę w~sposób całkowicie nienadzorowany.
*
* Najważniejszym narzędziem tego typu jest C++ Check@cite CppcheckAtoolforstaticCCcodeanalysis, statyczny analizator kodu źródłowego który wyłuskuje potencjalne błędy (np. niezainicjowane zmienne,
* niezwolnioną pamięć, niezłapane wyjątki), posługując się wyłącznie oryginalnymi kodami źródłowymi z~pominięciem budowania pliku wykonywalnego.
* C++ Check jest z~powodzeniem wykorzystywany w~produkcyjnych aplikacjach (między innymi wykrył 27 błędów w~jądrze Linuxa w~wersji 2.6@cite CppCheckFoundBugs) i~jest
* uznany za dojrzałe, dodatkowe narzędzie weryfikujące kod. Minusem rozwiązania jest czas analizy - 6 rdzeni sprawdzało 34 pliki *.cpp Pixie w~ciągu
* blisko 45 minut.
*
* W~przyszłości Pixie Engine może automatycznie uruchamiać C++ Check przy każdej prośbie o~dołączenie kodu. W~ten sposób jakość kodu
* może zostać utrzymana przez analizę potencjalnych braków.
* 
* Więcej informacji na temat sposobów testowania Pixie Engine znajduje się w~rozdziale @ref engine_tests "testy".
*
* @section dependency_debugbreak DebugBreak
* DebugBreak@cite DebugBreak to niewielka - jednonagłówkowa - biblioteka zdolna przełączyć punkt zatrzymania za pomocą wstawki assembly (zwykle zaimplementowanej jako
* pułapka bądź przerwanie procesora) na wszystkich wspieranych platformach. W~projekcie jest wykorzystana głównie na etapie tworzenia oprogramowania
* lub testów. Dzięki zastosowaniu DebugBreak programista może zobaczyć wykryty problem (np. nadpisanie pamięci) w~chwili jest wystąpienia.
*
* DebugBreak *nie* jest włączony do finałowej kompilacji (ze względu na brak dołączanych symbol debuggera, jego obecność nie miałaby sensu),
* dlatego w~przypadku zawieszenia aplikacji na komputerze klienckim stosowane są inne mechanizmy zbierania i~raportowania błędów.
*
* @section dependency_dia dia
* dia@cite DebugInterfaceAccessSDK (oficjalnie nazwa pisana z~małej litery) to framework niezbędny do poprawnego działania LLVM/Clang. Poza zastosowaniem
* w~kompilatorze nie jest wykorzystywany bezpośrednio w~projekcie.
*
* @section dependency_dot dot
* dot@cite GraphvizGraphVisualizationSoftware (oficjalnie nazwa pisana z~małej litery) to niewielkie narzędzie do generowania graficznych diagramów używane wewnętrznie przez
* doxygen. Poza wykorzystaniem przy tworzeniu dokumentacji nie jest wykorzystywane bezpośrednio w~projekcie.
*
* @section dependency_doxygen doxygen
* doxygen@cite DoxygenMainPage (oficjalnie nazwa pisana z~małej litery) stanowi dojrzały, stabilny system generowania dokumentacji bezpośrednio z~kodu źródłowego.
* doxygen wyłuskuje otagowane specjalnymi komentarzami komendy i~tworzy z~nich opis programu wraz z~informacjami wyciągniętymi bezpośrednio
* z~kodu źródłowego. Narzędzie jest w~stanie wygenerować graf obiektów (także graficzny, z~wykorzystaniem narzędzia dot),
* oraz odczytać podstawowe informacje z~nagłówków klas, funkcji i~metod. doxygen jest udostępniany na licencji GNU GPL, ale w~projekcie jest uwzględniona
* wyłącznie jego binarna wersja (bez włączania kodów źródłowych samego doxygena).
*
* Wybór doxygena jako narzędzia generującego dokumentację był dość oczywisty - jest to obecnie najbardziej znane, najpowszechniej stosowane rozwiązanie
* w~przypadku tworzenia aplikacji C++. Posiada największą bazę odpowiedzi, najlepszą dokumentację i~uchodzi za najbardziej stabilne spośród wszystkich
* narzędzi dostępnych na rynku.
*
* doxygen jest zdolny do generowania dokumentacji w~różnych językach i~formatach wyjściowych.
*
* @section dependency_fastbuild FastBuild
* Jednym z~najbardziej irytujących problemów na etapie tworzenia kodu Pixie Engine były bardzo długie czasy kompilacji projektu (ponad 15 minut na pełen cykl).
* Ze względu na realny wpływ na czas prac nad silnikiem, została powzięta decyzja by znaleźć alternatywny sposób (nie oparty o~Apache Ant)
* sposób dokonywania szybkich, iteracyjnych kompilacji testowych, pozostawiając jednocześnie mechanizm pełnej rekompilacji do tworzenia finałowych
* wersji plików wykonywalnych.
*
* Rozważano kilka rozwiązań spośród których wybrano właśnie narzędzie FastBuild@cite FASTBuildHighPerformanceBuildSystem. FastBuild to relatywnie młody, zaawansowany system budujący
* oparty o~proces iteracyjny z~zachowaniem maksymalnej ilości danych między kompilacjami w~pamięci podręcznej. W~przeciwieństwie do innych rozważanych
* rozwiązań jest niezależny od wybranego kompilatora i~potrafi się dobrze
* integrować z~istniejącymi systemami budowania. Fastbuild potrafi rozłożyć kompilację na wiele rdzeni, sprawdzić różnice plików na poziomie makr preprocesora
* i~wykryć zmiany bez stosowania mechanizmów prekompilacji nagłówków.
*
* Zastosowanie FastBuild skróciło podstawowy cykl kompilacji na 8-rdzeniowym procesorze 6x-7x razy, a~przy minimalnej liczbie zmian kodu (budowa iteracyjna)
* pozwolił osiągnąć blisko 20x-25x wzrost prędkości kompilacji.
*
* FastBuild nie zmienia architektury operacji kompilowania i~nie wpływa na proces konsolidacji. Z~tego powodu jest on transparentny dla ewentualnych
* problemów lub bugów, które mogą się pojawić w~kodzie produkcyjnym i~które mogłyby pozostać niezauważone do wykonania pełnego cyklu kompilacji.
*
* FastBuild pozwala także na rozłożenie kompilacji na więcej niż jedną fizyczną maszynę, ale taka funkcjonalność nie została wykorzystana w~niniejszej pracy.
*
* @section dependency_git git
* Podczas tworzenia jądra Linuxa jego twórcy stanęli przed problemem koordynowania prac w~zespole rozproszonym po całym świecie z~otwartą możliwością
* kontrybuowania kodów źródłowych z~zewnątrz zespołu. W~takiej sytuacji oczywistym rozwiązaniem jest użycie systemu kontroli wersji, systemu który
* zapewnia możliwość tworzenia kontrolowalnego repozytorium zdolnego do zapamiętania (i odwrócenia) dowolnej zmiany w~produkcie. W~owych czasach
* na rynku znajdowało się kilka produktów tej klasy, jednak żaden nie spełniał wymagań twórców Linuxa. Stojąc przed oczywistą blokadą, twórcy Linuxa podjęli
* decyzję o~stworzeniu autorskiego, własnego rozwiązania które nazwali właśnie gitem.@cite GitWikipedia Tak powstał obecnie najpopularniejszy - i~wykorzystywany także w~Pixie Engine -
* zdecentralizowany system kontroli wersji, zdolny do łatwego scalania zmian pochodzących z~różnych gałęzi i~umożliwiający ocenę i~akceptację kodu pochodzącego
* z~zewnątrz projektu (w formie tzw. próśb o~ściągnięcie).
*
* Analogicznym rozwiązaniem do systemu kontroli git jest Mercurial@cite MercurialSCM (hg), posiadający bardzo podobny zestaw cech, ale w~przeciwieństwie do git operujący na
* zmianach (ang. changes) a~nie stanach (ang. snapshots). Autor niniejszej pracy preferował by system Mercurial jednak najpopularniejszy
* serwis oprogramowania otwartych-źródeł, github który przechowuje repozytoria kodu źródłowego projektu, nie dopuszczał repozytoriów hg (w momencie rozpoczynania projektu). Migracja z~git do hg nie została do dziś
* przeprowadzona
*
* Dodatkowym problemem z~gitem jest ilość binarnych plików (np. 4,7 gb zależności) jakie naturalnie pojawiają się w~projektach gier. Git rozwiązuje problem
* częściowo za pomocą rozszerzenia Magazyn Dużych Plików (Large File Storage - LFS), jednak jego wsparcie w~środowisku produkcyjnym nie jest w~pełni
* stabilne (np. wspomniany github dopuszcza jedynie 1 gb plików binarnych). Z~tego powodu zależności muszą być utrzymywane osobno; obecnie są przechowywane
* *poza* systemem wersjonowania na serwerze Kosek.com
*
* Oczywiście utrzymywanie plików źródłowych git w~projekcie nie miałoby sensu - Pixie Engine używa jedynie binarnej wersji gita.
*
* @section dependency_glew glew
* OpenGL, jeden z~dwóch rendererów wspieranych przez Pixie Engine do niedawna był niemal całkowicie zamrożony w~produkcji - konsorcjum twórców OpenGL
* nie dopuszczało zmian standardu, poza tworzeniem (np. przez producentów kart graficznych) autorskich rozszerzeń.@cite OpenGLWikipedia O~ile sytuacja ta zmieniła się niedawno,
* o~tyle wciąż ogromna część elementów OpenGL jest zaimplementowana właśnie jako wspomniane rozszerzenia (z których cześć stała się de facto elementami
* standardu). Sprawdzanie dostępności rozszerzeń na danej architekturze i~weryfikowanie możliwości kart graficznych stało się dość niewdzięcznym zadaniem - 
* zwłaszcza iż ta sama funkcjonalność potrafiła być zawarta w~różnych rozszerzeniach zależenie od producenta danej karty graficznej (a czasem różnić
* się w~detalach implementacyjnych!). GLEW@cite GLEW (OpenGL Extension Wrangler Library), to zestaw nagłówków i~bibliotek które uproszczają proces sprawdzania i~uruchamiania
* opisanych rozszerzeń.
* 
* @section dependency_googlemock Google Mock
* Google Mock@cite GoogleTestMock to zaawansowana biblioteka do tworzenia makiet (ang. mockups) kodu na potrzeby testowania modułów. Makiety, to niewielkie "wydmuszkowe" klasy
* które nie posiadają faktycznej implementacji, a~są zdolne do odnotowania aktu wywołania dowolnej z~własnych metod, lub dostępu do akcesorów.
*
* Przykładem zastosowania Google Mock może być następująca sytuacja: rozważmy moduł ustawień. Akt zapamiętania ustawienia powinien wywołać funkcję zapisu
* magazynu ustawień. Możemy wywołać metodę zapamiętania ustawień na faktycznej implementacji menadżera ustawień przy jednoczesnym wstrzyknięciu makiety magazynu.
* Oczywiście makieta powinna odnotować prośbę o~zapis - brak takiej prośby powinien być zgłoszony jako błąd.
*
* @section dependency_googletest Google Test
* Google Test@cite GoogleTestMock to rozbudowane narzędzie Google do tworzenia testów jednostkowych. O~ile Google Test i~opisany powyżej Google Mock zaczęły jako osobne projekty, obecnie
* są wykorzystywane niemal zawsze wspólnie. Google Test pozwala na tworzenie przypadków testowych, ewaluowania środowiska testowego i~sprawdzanie warunków korzystając
* z~bogatej biblioteki asercji. Testy wykonywane są jeden po drugim na wyczyszczonym środowisku testowym, co pozwala na ewaluacje i~późniejszą agregację danych z~wielu
* przebiegów testowych
*
* @section dependency_gtest_runner Google Test runner
* Google Test runner@cite GoogleTestRunner to niewielkie narzędzie, wbrew nazwie *nie* tworzone przez Google ani powiązane podmioty. Narzędzie to pozwala na uruchamianie testów
* stworzonych za pomocą Google Test i~generowanie na podstawie przebiegów raportów w~formacie XML. Pixie Engine wykorzystuje jedynie wariant z~linii komend, ale oczywiście
* nic nie uniemożliwia uruchomienia tego narzędzia ręcznie w~trybie graficznym
*
* @section dependency_html_help_workshop HTML Help Workshop
* HTML Help Workshop to bezpłatne narzędzie Microsoftu zdolne przygotować skompilowany plik pomocy (chm@cite MicrosoftCompiledHTMLHelpWikipedia ) z~zadanych źródeł przygotowanych w~formie 
* strony internetowej (html). HTML Help Workshop jest wykorzystywany w~Pixie Engine jedynie jako narzędzie z~wiersza polecenia przez doxygena
* na potrzeby generowania dokumentacji na platformie Windows.
* 
* W~Pixie Engine jest włączona jedynie binarna dystrybucja HTML Help Workshop. 
* 
* @section dependency_levenshtein Kalkulator odległości Levenshteina
* Odległość Levenshteina między dwoma ciągami danych to minimalna ilość operacji jednostkowych (dodania, zmiany, bądź usunięcia) na dowolnym ciągu
* potrzebnych by oba ciągi stały się identyczne.@cite LevenshteindistanceWikipedia
* 
* Przykład, dla danych ciągów:
*  - abcb
*  - ccb
*
* Odległość Levenshteina wynosi 2 (wystarczy usunąć literkę 'a' z~przodu pierwszego i~zamienić kolejną 'b' na 'c').
*
* Kalkulator odległości Levenshteina to prosta biblioteka o~otwartych źródłach która oblicza wartość odległości dla dowolnych ciągów o~porównywalnych elementach.
* 
* @section dependency_jdk JDK 1.8
* Java Development Kit (JDK)@cite JavaSE to pełne SDK i~platforma uruchomieniowa Javy, współczesnego języka zarządzanego. W~projekcie jest używana wyłącznie wersja
* prekompilowana w~formie binarnej przenośnej - jest to zależność niezbędna do działania Apache Ant i~poza nim nie jest wykorzystywana.
*
* @section dependency_llvm LLVM
* Kompilator języka C++ to jeden z~najbardziej skomplikowanych programów jaki można współcześnie napisać. Na rynku, do niedawna, istniały tylko trzy
* produkcyjne implementacje: GCC, MSVC i~Intel C++ Compiler, spośród których tylko GCC był darmowy i~posiadał otwarte źródła. Jedną z~najbardziej problematycznych
* cech GCC był styl jego wewnętrznej implementacji. Kompilator był tworzony bez żadnego planu i~posiadał bardzo chaotycznie rozrzucony kod źródłowy, pełen
* haków i~skrótów utrudniających modyfikacje.@cite Comparingclangtootheropensourcecompilers
*
* W~2000 roku na uniwersytecie Illinois założono nowy projekt LLVM (początkowo skrót rozwijał się do Low-Level Virtual Machine)@cite TheLLVMCompilerInfrastructureProject; Celem było sprawdzenie
* metod optymalizacji kodu pośredniego przed etapem asemblacji. Wkrótce okazało się że ze względu na "czystą" implementację projekt posiada znacznie szerszą
* gamę zastosowań.@cite LLVMWikipedia Pięć lat później powstała fasada CLang, kompilująca C++ do postaci pośredniej na potrzeby LLVM. W~ten sposób oba projekty stworzyły podwalny
* środowiska pod nową rodzinę kompilatorów. Obecnie LLVM to ponad 20 projektów (wliczając preprocesor, kompilator, konsolidator, linker, debugger, assembler,
* analizator, etc.) i~jest produkcyjnie gotowy do kompilowania projektów na wszystkich wspieranych przez Pixie Engine platformach. Zastosowanie jednego kompilatora
* znacząco upraszcza fazę debuggowania i~testowania (nie ma np. problemów z~miskompilacją generowaną przez różne narzędzia).
* 
* Jako ciekawostkę można zauważyć że od 2017 linker LLVM jest w~stanie wygenerować symbole debuggera systemu Window (.pdb)@cite ThePDBFileFormatLLVM7documentation, tym samym można wykorzystać wszystkie
* istniejące narzędzia debuggowania / testowania które nie są zgodne z~symbolami stosowanymi w~GCC. To pozwoliło m.in. na zastosowanie
* Open C++ Coverage oraz zrzucania informacji o~kodzie w~przypadku niezłapanego wyjątku.
*
* @section dependency_mathjax MathJax
* MathJax@cite MathJax to zaawansowany parser wyrażeń matematycznych w~przeglądarce. W~Pixie Engine jest wykorzystywany jako narzędzie
* pomocnicze do generowania wzorów matematycznych w~niniejszej dokumentacji. Tylko binarna wersja jest dołączona.
*
* @section dependency_vlc libVLC
* VLC@cite VLCFeaturesVideoLAN to wieloplatformowe środowisko odtwarzacza multimediów, zdolne do wyświetlania filmów w~różnych formatach, konwersji i~rekompresji strumieni.
* Jedną z~najważniejszych cech VLC jest brak założeń co do systemu w~którym odtwarzacz jest uruchamiany - podstawa kodowa zawiera wszystkie realnie liczące się
* kompresory i~dekompresory (tzw. kodeki), filtry oraz zasoby generujące strumienie wyjściowe.
*
* libVLC@cite libVLCVideoLANWiki to biblioteka C++ na bazie której odtwarzacz VLC jest skonstruowany. Pixie Engine wykorzystuje libVLC do odtwarzania filmów bezpośrednio w~grze
* (np. logo producenta, przerywniki filmowe). libVLC wspiera zarówno wyświetlanie filmu bezpośrednio na ekranie jak i~strumieniowanie do tekstury (DirectX i~OpenGL).
* libVLC pozwala też na zrzucanie filmu bezpośrednio z~ekranu (przechwytywanie wideo) i~tworzenie zrzutów ekranów.
* 
* @section dependency_minizip miniZip
* Wczytywanie wielu plików z~dysku (twardego, DVD, etc.) wiąże się z~narzutem potrzebnym systemowi plików na odszukanie ścieżki na urządzeniu, otwarcie
* deskryptora zabezpieczeń i~wreszcie otwarcie uchwytu do samego pliku. Z~tego powodu łatwo zauważyć że jeden plik o~rozmiarze 100 mb będzie kopiowany bądź
* czytany znacznie szybciej niż sto plików o~rozmiarze 1 mb każdy.@cite Archive1 @cite Archive2 Ponieważ - jak już zauważono - gra to setki tysięcy zasobów - aby przyspieszyć czas wczytywania
* danych, powiązane zasoby najlepiej jest spakować w~jedno (bądź kilka) archiwum. Dla wirtualnego systemu plików (Zobacz rozdział @ref vfs) Pixie Engine archiwum staje się transparentne
* i~dostęp do danych w~środku jest równie łatwy jak w~przypadku niezależnych plików. Badania wykazały że wczytując dane z~archiwum można skrócić czasy ładowania
* o~około 50%-60% dla 250+ plików przy założeniu że archiwum jest nieskompresowane.@cite Archive3
*
* Oczywiście kwestią otwartą pozostaje sam format archiwum. W~początkowej fazie rozważano archiwum tar, jako najprostszy format zdolny przechować połączone pliki.
* Wiele gier korzysta z~własnych formatów opracowanych przez ich twórców, czasem jednak stosuje się popularne formaty maskując je innym rozszerzeniem.
* Np. archiwum PK3 używane w~Quake to po prostu archiwum zip.
*
* Opracowanie własnego formatu archiwum przekroczyłoby zakres niniejszej pracy, dlatego po rozważeniu argumentów za i~przeciw wybrano jako format archiwum właśnie zip.
* Za tym formatem przemawia:
* - łatwe dodawanie plików bez potrzeby przepisania całego archiwum
* - bardzo szybkie czasy szukania po archiwum
* - wsparcie dla kompresji (zwykle nieużywane ze względu na narzut ładowania, ale przydatne przy przesyłaniu archiwum przez sieć)
* - wsparcie dla szyfrowania
* - szerokie wsparcie narzędzi
*
* miniZip@cite miniZip to otwarta biblioteka umożliwiająca zapisywanie i~czytanie archiwum zip (także z~wykorzystaniem funkcji zwrotnych).
*
* @section dependency_mingw MinGW
* Opisany powyżej LLVM ma jedno zasadnicze ograniczenie na platformie Windows - nie posiada natywnego kompilatora zasobów
* (pików rc)@cite LLVMrc, niezbędnych by dołączyć dodatkowe informacje do plików wykonywalnych na tej platformie. Potrzebny był zewnętrzny program 
* posiadający taką funkcjonalność. W~chwili obecnej na rynku znajdują się tylko dwa kompilatory zasobów: MSVC (komercyjny) i~program wchodzący
* w~skład MinGW (Minimalistyczne Gnu dla Windows)@cite MinGW. Z~oczywistych względów - licencji i~przenośności - do Pixie Engine włączono ten drugi.
* 
* Poza kompilatorem zasobów, MinGW *nie* jest wykorzystywane w~projekcie - w~szczególności nie jest wykorzystywany kompilator GCC który wchodzi w~jego skład.
*
* W~Pixie Engine jest włączona jedynie binarna wersja MinGW
* 
* @section dependency_msxsl msxsl
* Ponieważ raporty Google Test runner są tworzone w~formacie XML, istniała potrzeba konwersji do formatu bardziej czytelnego
* dla użytkownika nietechnicznego. Najprostszą opcją jest zastosowanie transformaty XSLT która generuje dokument HTML, czytelne przez każdą przeglądarkę stron
* internetowych na każdej wspieranej platformie. XSLT mógł zostać uruchomiony bezpośrednio w~przeglądarce, ale testy wykazały że przeglądarki posiadają
* spore ograniczenia @cite xsltBrowserSupport (np. MSIE nie pozwoli na uruchomienie transformaty na dokumencie lokalnym). Jedynym sensownym rozwiązaniem było zastosowanie osobnego
* programu który wykona pełen przebieg transformacji. Na platformie Windows wybrano do tego celu msxml (Microsoft XML)@cite msxml, darmowy program uruchamiany z~linii
* komend. Jest on obecnie podpięty do systemu testów (zobacz rozdział @ref engine_tests ) i~uruchamiany automatycznie po każdym przebiegu testowym.
*
* @section dependency_multiconsoles Multiple Consoles
* Standardowo na platformie Windows nie ma możliwości alokowania więcej niż jednej konsoli dla programu. Istnieje także szereg ograniczeń, których nie da
* się łatwo ominąć, np. po wciśnięciu kombinacji klawiszy ctrl + break w~konsoli system Windows zabije aplikację w~ciągu 5 sekund, jeżeli ona sama nie
* wyłączy się wcześniej @cite ctrlbreakWinApi (tego mechanizmu nie da się w~żaden sposób obejść ani wyłączyć @cite whyctrlbreakWinApi ).
*
* Jedynym sposobem na rozwiązanie tych problemów jest stworzenie zewnętrznej aplikacji (pliku wykonywalnego) która otwierałaby własną konsolę i~przyjmowałaby
* polecenia wydruku informacji (np. poprzez gniazdo lub strumień komunikacyjny). Oczywiście taką aplikację można by uruchamiać kilkukrotnie aby
* uzyskać efekt wielu konsoli. Zamknięcie tej aplikacji nie wpływałoby też na zamknięcie głównego programu.
*
* Oczywiście jak przystało na środowisko otwartych źródeł, ktoś zrealizował już taką bibliotekę przed Pixie Engine. Tym samym zamiast tworzyć ponowną
* implementację, po prostu włączono taką bibliotekę - w~tym wypadku jest ona autorstwa Zvika Ferentza i~udostępniana na wolnej licencji.@cite Multipleconsolesforasingleapplication
*
* @section dependency_nsis NSIS
* Pierwszą rzeczą jaką zobaczy użytkownik danej gry jest jej program instalacyjny. Oczywiście na każdej platformie istnieją już gotowe wzorce
* których użytkownik się spodziewa gdy instaluje nowy program - na platformie Windows takich wzorcowych implementacji jest kilka, poczynając od
* InstallShield (komercyjny), idąc przez InstallMaker, a~kończąc właśnie na otwarto źródłowym NSIS@cite NSIS .
*
* Wybrano NSIS ze względu na dużą zdolność konfigurowalności, liberalną licencję, stabilny i~dojrzały kod oraz dużą ilość materiałów dostępnych
* w~Internecie. Instalator NSIS opiera się o~skrypty NSIS i~jest zdolny do wykonania wszystkich poleceń potrzebnych do instalacji gry
* (w tym np. rejestracji gry w~panelu Gry Windows 7+), oraz do stworzenia interaktywnego deinstalatora. Program jest także zdolny do generowania
* inkrementacyjnych łatek, które pozwalają poprawić pliki gry zawierając tylko niezbędne zmiany (i tym samym znacząco ograniczając rozmiar takiej łatki)
* 
* @image html dependency_nsis_image.jpg "Przykładowy zrzut ekranu NSIS"
* @image latex dependency_nsis_image.jpg "Przykładowy zrzut ekranu NSIS" height=5cm
*
* @section dependency_openal_soft OpenAL (Software)
* W~latach 90, na wzór Otwartej Biblioteki Graficznej (OpenGL) powstała także Otwarta Biblioteka Audio (OpenAL). Projekt był rozwijany przez kilka
* firm zajmujących się tworzeniem kart dźwiękowych do komputerów i~ostatecznie został porzucony wraz z~rozwojem kart zintegrowanych na rynku Koreańskim.@cite OpenAL
*
* Ponieważ interfejs programistyczny OpenAL był w~miarę sensownie ułożony i~ustandaryzowany, pojawiła się otwarta implementacja oparta całkowicie o
* standardowy procesor (CPU), nie korzystająca ze wsparcia sprzętowego (m.in. SoundBlaster). W~ten sposób projekt przetrwał do dnia dzisiejszego a
* biblioteka OpenAL Software posiada pełną funkcjonalność droższych kart dźwiękowych i~jest w~stanie realizować je całkowicie programowo.
*
* OpenAL został włączony do projektu jako zaplecze renderera dźwięku. Jego przewagą nad konkurencyjnymi rozwiązaniami jest jego pełna przenośność na 
* wszystkich wspieranych przez Pixie Engine platfomrach.
*
* @section dependency_open_cpp_coverage Open C++ Coverage
* Elementem testowania aplikacji jest ocena (metryka) pokrycia kodu testami. Najczęściej wykorzystywana metryka określa stosunek linii w~kodzie wykonanych przez testy
* do wszystkich linii kodu w~programie i~najczęściej wyraża się w~procentach.
*
* W~językach wyposażonych w~refleksję (jak np. C# lub Java) taki test jest wyjątkowo łatwy do przeprowadzenia; W~C++ potrzeba dodatkowych informacji
* debuggera by móc ustalić dokładną lokalizację wykonywanej linijki. Na szczęście wykorzystywany do kompilacji Pixie Engine LLVM jest w~stanie stworzyć
* takie symbole, a~program Open C++ Coverage@cite OpenCppCoverage jest w~stanie pobrać z~nich informacje o~pokryciu w~formacie XML (bezpośrednio integrującym się z
* Visual Studio Code); Istnieje także możliwość przygotowania raportu HTML z~wylistowaniem dokładnie które linijki były testowane
* a~które nie.
*
* @image html dependency_open_cpp_coverage_image.jpg "Przykładowy raport Open C++ Coverage"
* @image latex dependency_open_cpp_coverage_image.jpg "Przykładowy raport Open C++ Coverage" height=5cm
*
* @section dependency_pugixml pugixml
* Wybranym metaformatem danych w~Pixie Engine jest język XML. Istnieje duża ilość narzędzi i~parserów do tego języka, poszukiwania skupiały się więc na
* rozwiązaniu wydajnym i~bezpiecznym. Obecnie istnieje ponad 20 parserów, z~których najszybszym jest rapidxml. Drugie miejsce w~rankingu zajmuje
* pugixml@cite pugixml, który w~przeciwieństwie do rapidxml jest parserem dużo bezpieczniejszym (np. nie używa pustych wskaźników jako brakujących pól, co znacząco
* upraszcza kod sprawdzający potencjalne błędy bądź uszkodzone pliki). Oczywiście szybkość parsera wynika z~jego podstawowego ogracznienia: jest to parser
* destruktywny in-situ, który znakuje oryginalne dane zerowymi bajtami i~tworzy drzewko odniesień do oryginalnego łańcucha znaków. Dzięki takiemu podejściu
* pugixml jest zaledwie 3-4x wolniejszy niż użycie funkcji @c strlen() na tych samych danych.@cite pugixmlBenchmark
*
* @section dependency_python Python 3.5
* Python w~wersji 3.5@cite python jest używany wewnętrznie przez debugger LLVM. Poza tym nie jest wykorzystywany bezpośrednio w~projekcie.
*
* @section dependency_remotery Remotery
* Remotery@cite remotery to współczesny profiler gier oparty o~gniazda sieciowe. Jako biblioteka integruje się z~aplikacją i~wystawia na gnieździe TCP informacje o
* czasach wykonania i~próbkach renderera, a~także informacje z~dziennika i~debuggera. Drugą częścią aplikacji jest strona www, która łączy się z~portem
* TCP za pomocą websocket, odbiera dane od aplikacji i~przedstawia je w~graficznej formie.
*
* Zaletą tego rozwiązania jest to że profiler i~kokpit nie muszą być uruchomione na tej samej maszynie - nawet nie musi być to ta sama architektura (!).
* Jedynym wymogiem jest by profiler był osiągalny przez sieć z~komputera z~kokpitem. W~ten sposób można np. profilować konsole do gier, albo komórki.
*
* @image html dependency_remotery_image.jpg "Interfejs przeglądarkowy Remotery"
* @image latex dependency_remotery_image.jpg "Interfejs przeglądarkowy Remotery" height=7.5cm
*
* @section dependency_sdl SDL 2.0
* SDL@cite sdl to dojrzała, wieloplatformowa biblioteka podstawowa do tworzenia gier. Posiada standardowy zestaw funkcji zaimplementowany na każdej ze wspieranych
* przez Pixie Engine platform, takich jak obsługa okien, sterowników sprzętowych, uruchamianie rendera, sprawdzanie platformy, obsługa klawiatury, myszy i~
* kontrolerów gier, etc. SDL zostało wybrane ze względu na otwarte źródła, dużą ilość dostępnych materiałów, dojrzałość kodu i~stabilność implementacji.
*
* 
* @section dependency_sdl_net SDL NET 2.0
* SDL NET@cite sdl to wtyczka do SDL umożliwiająca programowanie wieloplatformowe gniazd sieciowych (TCP/UDP). W~Pixie Engine SDL NET jest wybraną platformą do
* programowania gier siecowych i~odpytywania serwera HTTP w~celu sprawdzenia dostępności aktualizacji. SDL NET dostarcza warstwę abstrakcji nad
* surowe gniazda sieciowe na danej platformie. Biblioteka jest niskopoziomowa i~pozwala na bezpośrednie wysyłanie i~odbieranie ramek/pakietów oraz tworzenie
* i~przywiązywanie gniazd sieciowych.
*
* Biblioteka jest całkowicie synchroniczna, z~tego powodu aby była użyteczna w~środowisku growym musi być opakowana w~obiekty wielowątkowe.
*
* @section dependency_stb stb
* Biblioteki Sean T. Barrett (STB) to zbiór jedno nagłówkowych narzędzi programistycznych napisanych w~czystym C. Cechuje je brak zewnętrznych zależności,
* prostota implementacji i~duża wydajność.
*
* Narzędzia stb są opublikowane jako własność publiczna i~są otwarte na nowe zgłoszenia kodu.
* 
* <center><table>
* <caption id="dependency_stb_table">Komponenty stb wykorzystywane w~Pixie Engine</caption>
* <tr><th>Nazwa<th>Wersja<th>Linie kodu<th>Opis
* <tr><td>stb_image.h <td> 2.19<td>7462<td>Wczytywanie z~plików/pamięci: JPG, PNG, TGA, BMP, PSD, GIF, HDR, PIC
* <tr><td>stb_truetype.h <td> 1.19<td>4853<td>Parser, rasteryzer i~dekoder fontów truetype
* <tr><td>stb_image_write.h <td> 1.09<td>1568<td>Zapisuje obrazki na dysk w~formatach: PNG, TGA, BMP
* <tr><td>stb_image_resize.h <td> 0.95<td>2627<td>Skaluje obrazki w~pamięci w~dobrej jakości
* <tr><td>stb_rect_pack.h <td> 0.11<td>624<td>Generator dynamicznych atlasów tekstur
* <tr><td>stb_textedit.h <td> 1.12<td>1404<td>Podstawy pod implementację własnego edytora tekstowego
* <tr><td>stb_perlin.h <td> 0.3<td>316<td>Generator szumu perlina
* <tr><td>stb_easy_font.h <td> 1.0<td>303<td>Prosty font do drukowania napisów debuggera
* </table>źródło: https://github.com/nothings/stb#stb_libs</center>
*
* @section dependency_texlive texLIVE
* texLIVE@cite texlive to darmowa, prosta wersja LaTeXa na platformę Windows służąca do generowania dokumentacji pdf bezpośrednio z~kodu źródłowego.
* Poza włączeniem w~system budowania dokumentacji nie jest używana w~projekcie.
*
* @section dependency_thttpd thttpd
* thttpd@cite thttpd (oficjalnie nazwa pisana z~małej litery) to bardzo prosty serwer HTTP używany w~projekcie wyłącznie do testowania funkcjonalności aktualizacji.
* W~wersji finałowej aplikacja odpytuje serwer twórcy danej gry szukając potencjalnych aktualizacji; Na potrzeby testów zapytania zewnętrzne byłyby kłopotliwe
* (np. trudno jest symulować awarię zdalnego serwera), dlatego na potrzeby testów adres serwera jest przepięty na adres thttpd, uruchamianego
* lokalnie. Dzięki takiemu podejściu można kontrolować i~debuggować oba końce połączenia.
*
* Nie jest łatwo znaleźć dobry, pozbawiony zależności serwer HTTP. Thttpd został wybrany ze względu na jego nieinwazyjność (nie wymaga instalacji usługi,
* pozwala na wybór powiązanego portu i~nie wykonuje aktywnie żadnego ze skryptów CGI).
*
* @section dependency_upx UPX
* Ponieważ pliki binarne Pixie Engine potrafią osiągnąć spore rozmiary (w tej chwili ponad 55 mb głównego pliku exe) aby przyspieszyć ich ładowanie do
* pamięci i~ułatwić proces łatkowania potrzebne było narzędzie które skompresuje je dynamicznie.
* 
* W~chwili obecnej na rynku znajduje się tylko jeden system pakujący pliki wykonywalne - właśnie UPX@cite upx (Universal Packer for eXecutables), zdolny pakować
* pliki wykonywalne wszystkich wspieranych przez Pixie Engine platform. UPX jest uruchomiony na ostatnim etapie budowania wersji końcowych gry
* i~w wariancie BRUTAL zmniejszył rozmiar wspomnianego pliku exe prawie dziesięciokrotnie (do 6 mb).
*
* Pliki spakowane UPXem są wciąż poprawnymi plikami wykonywalnymi danej platformy i~zawierają prosty program rozruchowy, który wypakowuje je do pamięci
* dynamicznej, po czym uruchamia właściwy program. Cały proces jest transparentny dla użytkownika.
*
* UPX powoduje też że próby wstecznej inżynierii kodu są nieco trudniejsze niż przy zastosowaniu "czystych" plików wykonywalnych.
*
* @section dependency_vscode Visual Studio Code
* Poprzednie wersje Pixie Engine powstały w~Visual Studio Community Edition@cite msvc (MSVC). Z~takim podejściem wiązało się sporo problemów: po pierwsze MSVC jest dostępny
* jedynie na platformę Windows, po drugie jest nieprzenośny (w znaczeniu: wymaga instalacji), a~po trzecie ma bardzo wysokie wymagania sprzętowe, szczególnie
* jeżeli chodzi o~ilość zajmowanego miejsca (pełna instalacja MSVC zabiera 130 gb@cite msvcdrivespace ).
* 
* Z~chwilą upowszechnienia się wersji stabilnej VS Code@cite vscode pojawił się pomysł zmiany środowiska; Za VS Code przemawiało przede wszystkim możliwość
* stworzenia wersji przenośnej dołączonej bezpośrednio do projektu, ogromną bazę darmowych wtyczek, otwarte źródła i~wsparcie jednej z~największych firm sektora IT,
* czyli Microsoft. Zmiana środowiska nastąpiła pod koniec 2017 roku i~okazała się strzałem w~dziesiątkę. W~tej chwili VS Code stanowi podstawowy edytor
* kodu na wszystkich wspieranych przez Pixie Engine platformach (sam edytor jest w~pełni wieloplatformowy).
*
* @section dependency_wxwidgets wxWidgets
* wxWidgets to wieloplatforma biblioteka GUI, służąca w~Pixie Engine za podstawę interfejsu aplikacji nie związanej z~grą (np. edytora plansz, interfejsu startowego).
* 
* W~chwili obecnej na rynku znajdują się dwie podstawowe biblioteki GUI: wxWidgets@cite wxwidgets i~qt@cite qt. Porównanie obu znajduje się poniżej:
* 
* <center><table>
* <caption id="dependency_wxwidgets_table">Analiza porównawcza wxWidgets i~qt metodą wag (-10/+10)</caption>
* <tr><th>wxWidgets<th>qt
* <tr><td>
* - Bardzo dobra jakość aplikacji końcowej (+9)
* - Natywny wygląd na każdej ze wspieranych platform (+3)
* - Łatwość integracji z~biblioteką (+7)
* - Modułowość (+2)
* - Słabe API (szczególnie rozmiarowe, ang. sizer) (-5)
* </td><td>
* - Ujednolicony wygląd aplikacji na każdej ze wspieranych platform (-1)
* - Problemy z~integracją w~C++ (-9)
* - Modułowość (+2)
* - Dedykowany edytor okienek (+5)
* - Lepsze API (+2)
* </td></tr>
* <tr><td>+16<td>-1
* </table>Wynik analizy: wxWidets wygrywa przewagą 17 punktów</center>
*
* wxWidgets jest zintegrowany bezpośrednio z~głównym modułem gry (więcej informacji znajduje się w~rozdziale @ref engine_architecture "architektura silnika")
*
* @section dependency_windbg WinDBG (Debugging Tools for Windows)
* WinDBG@cite winDBG to prosty, darmowy debugger C++ integrujący się z~VS Code. Poza debuggowaniem na platformie Windows nie jest używany w~projekcie.
*
* WinDBG został wybrany ponad LLDB (debuggerem LLVM) ze względu na brakującą implementację tego drugiego; W~chwili obecnej LLDB nie jest w~stanie
* realnie pracować nad aplikacją Windowsa skompilowaną w~architekturze x64. Twórcy LLVM znają ten problem od Marca 2017
* (https://bugs.llvm.org/show_bug.cgi?id=32343) jednak do dziś nie powstała implementacja naprawiająca.
* 
*
*/