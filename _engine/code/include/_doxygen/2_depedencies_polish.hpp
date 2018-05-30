/**
* \~Polish @page depedencies Zależności
* @section dependency_dashboard_into Wprowadzenie
* W&nbsp;katalogu <tt>_engine/dependency</tt> w&nbsp;jednym z&nbsp;dwóch podkatalogów (@c library albo @c other) zostały umieszczone wszelkie biblioteki niezbędne do&nbsp;działania dla silnika.
*
* Jeżeli twój katalog <tt>_engine/dependency</tt> jest pusty, oznacza to najprawdopodobniej wykonanie opereracji <tt>git clone</tt> z&nbsp;publicznego repozytorium bez ściągnięcia zależności.
* Dzieje się tak dlatego iż&nbsp;zależności liczą sobie około 4,5 gb i&nbsp;nie są ściągane domyślnie. Należy użyć następujących komend
* aby pobrać je z&nbsp;serwera Kosek.com:
*
* - Na&nbsp;Windowsie: użyj skryptu z&nbsp;katalogu <tt>_command/windows/download_dependencies.bat</tt>. Potwierdź czas oczekiwania.
*
* Lista zależności znajduje się poniżej:
*
* @section dependency_dashboard Pełna lista
* <center><table>
* <caption id="dependency_dashboard_table">Lista wszystkich zależności</caption>
* <tr><th>Nazwa<th>Rodzaj<th>Rola w&nbsp;projekcie<th>Licencja<th>Twórca
* <tr><td>@ref dependency_angelcode AngelCode<td>Biblioteka<td>Dostawca języka skryptowego<td>zlib<td>Andreas J&ouml;nsson
* <tr><td>@ref dependency_anttweakbar AntTweakBar<td>Biblioteka<td>Dostawca podstawowych narzędzi poprawiania UI<td>zlib/libpng<td>Philippe Decaudin
* <tr><td>@ref dependency_apacheant Apache Ant<td>Inne<td>Wieloplaformowy system kompilacji<td>Licencja Apache, wersja 2.0<td>The Apache Software Foundation
* <tr><td>@ref dependency_blowfish Blowfish<td>Biblioteka<td>Szybki moduł szyfrujący<td>The Microsoft Public License (Ms-PL)<td>George Anescu
* <tr><td>@ref dependency_boost Boost<td>Biblioteka<td>Zestaw nagłówków wspierających tworzenie aplikacji C++<td>Licencja Boost Software, wersja 1.0<td>Open Source Contributors
* <tr><td>@ref dependency_di Boost Dependency Injection<td>Biblioteka<td>Zestaw narzędzi wstrzykujący zależności<td>Licencja Boost Software, wersja 1.0<td>Kris Jusiak
* <tr><td>@ref dependency_cereal cereal<td>Biblioteka<td>Narzędzia serializacji<td>Licencja BSD<td>Randolph Voorhies, Shane Grant
* <tr><td>@ref dependency_cg cg<td>Biblioteka<td>Język jednostki ceniującej GPU<td>Wykorzystanie klienckie NVIDIA Software<td>NVIDIA
* <tr><td>@ref dependency_cppcheck C++ Check<td>Inne<td>Statyczny walidator kodu C++<td>GNU GPL (Bez włączenia kodów źródłowych do&nbsp;projektu)<td>Daniel Marjam&auml;ki
* <tr><td>@ref dependency_debugbreak debugbreak<td>Biblioteka<td>Wieloplatformowe programowe instrukcje przerwania<td>MIT/X11<td>Scott Tsai
* <tr><td>@ref dependency_dia dia<td>Inne<td>(Używany wewnętrznie przez LLVM)<td>GNU GPL (Bez włączenia kodów źródłowych do&nbsp;projektu)<td>Open Source Contributors
* <tr><td>@ref dependency_dot dot<td>Inne<td>(Używany wewnętrznie przez doxygen)<td>The Common Public License (Bez włączenia kodów źródłowych do&nbsp;projektu)<td>AT &amp;T
* <tr><td>@ref dependency_doxygen doxygen<td>Inne<td>Narzędzia do&nbsp;generowania dokumentacji<td>GNU GPL (Bez włączenia kodów źródłowych do&nbsp;projektu)<td>Dimitri van Heesch
* <tr><td>@ref dependency_fastbuild FastBuild<td>Inne<td>Akcelelator systemu budowania C++<td>Wariant BSD<td>Franta Fulin 
* <tr><td>@ref dependency_git git<td>Inne<td>System kontroli wersji<td>GNU LGPL v2.1<td>Linus Torvalds, Junio Hamano and others
* <tr><td>@ref dependency_glew glew<td>Biblioteka<td>Rozszerzenia dla OpenGL<td>Licencja Khronos (MIT)<td>Lev Povalahev and others
* <tr><td>@ref dependency_googlemock Google Mock<td>Biblioteka<td>Automatyczne testy makietowe<td>BSD 3-punktowa<td>Google Inc.
* <tr><td>@ref dependency_googletest Google Test<td>Biblioteka<td>Automatyczne testy jednostkowe<td>BSD 3-punktowa<td>Google Inc.
* <tr><td>@ref dependency_gtest_runner Google Test runner<td>Inne<td>Host automatycznych testów jednostkowych<td>MIT/X11<td>Nic Holthaus
* <tr><td>@ref dependency_html_help_workshop HTML Help Workshop<td>Inne<td>Narzędzie do&nbsp;generowania dokumentacji chm dla narzędzia doxygen<td>Licencja końcowego użytkownika HTML Help<td>Microsoft
* <tr><td>@ref dependency_levenshtein Levenshtein distance calculator<td>Biblioteka<td>Obliczanie odległości Levenshteina między dwoma dowolnymi kolekcjami<td>Domena publiczna<td>Keith Schwarz
* <tr><td>@ref dependency_jdk JDK 1.8<td>Inne<td>Wymagane przez Apache Ant<td>Licencja Sun<td>Oracle Corporation
* <tr><td>@ref dependency_llvm LLVM<td>Inne<td>LLVM: środowisko kompilujące Pixie Engine (kompilator, linker, debugger)<td>Licencja LLVM (Bez włączenia kodów źródłowych do&nbsp;projektu)<td>LLVM Team
* <tr><td>@ref dependency_mathjax mathjax<td>Inne<td>Renderer wyrażeń matematycznych w&nbsp;przeglądarce<td>Licencja Apache, wersja 2.0<td>American Mathematical Society &amp; Society for Industrial and Applied Mathematics
* <tr><td>@ref dependency_vlc libVLC<td>Biblioteka<td>Odtwarzanie filmów<td>GNU LGPL 2<td>VideoLAN
* <tr><td>@ref dependency_minizip miniZip<td>Biblioteka<td>Kompresor/dekompresor archiwów<td>Wariant BSD<td>Nathan Moinvaziri
* <tr><td>@ref dependency_mingw MinGW<td>Inne<td>Kompilator plików zasobów na&nbsp;platformie Windows<td>MIT/X11<td>Open Source Contributors
* <tr><td>@ref dependency_msxsl msxsl<td>Inne<td>Transformator XML / XSLT działający z&nbsp;wiersza polecenia<td>Tylko binarne<td>msxsl
* <tr><td>@ref dependency_multiconsoles Multiple Consoles<td>Biblioteka<td>Pomocnicza aplikacja umożliwiająca alokację więcej niż jednego okna konsoli dla jednego procesu<td>Domena publiczna<td>Zvika Ferentz
* <tr><td>@ref dependency_nsis NSIS<td>Inne<td>Instalator na&nbsp;platformie Windows<td>zlib/libpng<td>Open Source Contributors
* <tr><td>@ref dependency_openal_soft OpenAL (Software)<td>Biblioteka<td>Moduł wspierający odtwarzanie dźwięków<td>GNU LPGL<td>kcat
* <tr><td>@ref dependency_open_cpp_coverage Open C++ Coverage<td>Inne<td>Aplikacja wyliczająca pokrycie kodu testami na&nbsp;podstawie symboli debugowania PDB<td>GNU GPL (Bez włączenia kodów źródłowych do&nbsp;projektu)<td>OpenCppCoverage Team
* <tr><td>@ref dependency_pugixml pugixml<td>Biblioteka<td>Szybki wczytywacz i&nbsp;parser dokumentów XML<td>MIT/X11<td>Arseny Kapoulkine
* <tr><td>@ref dependency_python Python 3.5<td>Inne<td>Używany przez LLVM na&nbsp;potrzeby debugowania aplikacji<td>PSF license agreement for Python<td>Python Software Foundation
* <tr><td>@ref dependency_remotery Remotery<td>Biblioteka<td>Zewnętrzny profiler w&nbsp;formie strony www<td>Licencja Apache, wersja 2.0<td>Don Williamson
* <tr><td>@ref dependency_sdl SDL 2.0<td>Biblioteka<td>Wieloplatformowa baza aplikacji<td>zlib<td>Open Source Contributors
* <tr><td>@ref dependency_sdl_net SDL NET 2.0<td>Biblioteka<td>Wieloplatformowe wsparcie dla sieci<td>GNU LGPL<td>Open Source Contributors
* <tr><td>@ref dependency_stb stb<td>Biblioteka<td>Proste pojedynczo nagłówkowe narzędzia<td>Domena publiczna<td>Sean Barrett
* <tr><td>@ref dependency_texlive texLIVE<td>Inne<td>Kompilator dokumentacji do&nbsp;formatu PDF<td>Licencja TL<td>Karl Berry
* <tr><td>@ref dependency_thttpd thttpd<td>Inne<td>Prosty serwer HTTP do&nbsp;testowania funkcjonalności aktualizacji<td>Wariant BSD<td>Jef Poskanzer
* <tr><td>@ref dependency_upx UPX<td>Inne<td>Uniwersalny kompresor plików binarnych, zmniejszający rozmiar plików wykonywalnych<td>Wariant BSD<td>Markus F.X.J. Oberhumer, L&aacute;szl&oacute; Moln&aacute;r & John F. Reiser.
* <tr><td>@ref dependency_vscode Visual Studio Code<td>Inne<td>Główny edytor w&nbsp;którym powstaje Pixie Engine<td>MIT/X11<td>Microsoft
* <tr><td>@ref dependency_wxwidgets wxWidgets<td>Biblioteka<td>GUI dla narzędzi<td>Licencja wxWindows<td>Open Source Contributors
* <tr><td>@ref dependency_windbg WinDBG (Debugging Tools for Windows)<td>Inne<td>Debugger wiersza poleceń podpięty do&nbsp;projektu Visual Studio Code<td>Tylko binarne<td>Microsoft
* </table></center>
*
* @section dependency_angelcode AngelCode
* Angelscript@cite AngelScript (albo Biblioteka Skryptowa AngelCode) to dojrzały, aktywnie rozwijany język skryptowy o&nbsp;składni opartej częściowo
* o&nbsp;C++ udostępniany na&nbsp;wolnej licencji zlib.
* 
* Wybór konkretnego języka skryptowego nie jest łatwy w&nbsp;dzisiejszych czasach z&nbsp;racji obecności na&nbsp;rynku wielu
* dojrzałych i&nbsp;gotowych-do-produkcji rozwiązań. W&nbsp;szczególności mogłoby zostać zadane pytanie czemu nie została wybrana Lua albo Python,
* najpopularniejsze obecnie języki skryptowe stosowane w&nbsp;grach; Prawdą jest, że&nbsp;o&nbsp;ile wydajnościowo Lua nieznacznie sprawuje
* się lepiej niż AngelScript, Angelscript z&nbsp;racji swojej składni zbliżonej do&nbsp;C++ jest nieporównywalnie łatwiejszy do&nbsp;podpięcia do&nbsp;natywnego
* systemu klas (zwykle także z&nbsp;pominięciem konieczności istnienia obiektów proxy). Python jako język pod względem
* wydajności sprawdza się o&nbsp;wiele gorzej a&nbsp;i jego zdolność do&nbsp;odwzorowania konstrukcji C++ jest dużo bardziej ograniczona.
* 
* <center><table>
* <caption id="dependency_angelcode_benchmark_table">Wydajność języków skryptowych w&nbsp;przebiegu testowym</caption>
* <tr><th>Nazwa<th>Czas wykonania<th>Względem Lua
* <tr><td>Lua<td>1,341 s.<td>100%
* <tr><td>Angelscript<td>1,859 s.<td>72%
* <tr><td>Python<td>2,842 s.<td>47%
* </table>źródło: https://github.com/r-lyeh-archived/scriptorium</center>
*
* @section dependency_anttweakbar AntTweakBar
* AntTweakBar@cite AntTweakBar to niewielka, łatwa do&nbsp;wykorzystania biblioteka która pozwala programistom i&nbsp;twórcom zawartości dodać prosty interfejs użytkownika
* na&nbsp;potrzeby interaktywnego dobierania parametrów. Biblioteka pozwala podpiąć dowolną zmienną C/C++ i&nbsp;umożliwia mechanizmy prostej modyfikacji
* wartości, na&nbsp;podstawie rozpoznanego typu (liczba, test, wektor, kolor, macierz, kwaternion). Biblioteka ta wspiera wszystkie współczesne stosowane
* renderery (OpenGL, DirectX, SDL)@cite howtoAntTweakBar
* 
* @image html dependency_anttweakbar_image.jpg "Przykładowy zrzut ekranu AntTweakBar"
* @image latex dependency_anttweakbar_image.jpg "Przykładowy zrzut ekranu AntTweakBar" height=5cm
* 
* Motywacją za&nbsp;włączeniem AntTweakBar było udostępnienie prostego narzędzia, które pozwoli parametryzować rozgrywkę (głównie do&nbsp;wykorzystania
* przy tworzeniu fanowski modyfikacji) bez opuszczania gry.
*
* @section dependency_apacheant Apache Ant
* Apache Ant@cite ApacheAnt to system budujący oparty o&nbsp;język XML zaimplementowany w&nbsp;Javie z&nbsp;wykorzystaniem komend języka skryptowego JavaScript. Posiada rozbudowaną
* bazę zadań (np. "wykonaj komendę na&nbsp;pliku", "skopiuj plik", "wgraj plik na&nbsp;serwer FTP", etc.), które można parametryzować z&nbsp;wykorzystaniem zmiennych.
*
* Istnieje na&nbsp;rynku wiele komercyjnie-gotowych systemów budujących, poczynając od&nbsp;makefile i&nbsp;cmake - bardzo dojrzałych narzędzi operujących na&nbsp;skryptach
* budowy, idąc przez SCONS oparty o&nbsp;pełnoprawny język skryptowy Python, a&nbsp;kończąc na&nbsp;Fastbuild (wykorzystany w&nbsp;Pixie Engine do&nbsp;budowania iteracyjnego)
* i&nbsp;rozwiązaniach wewnętrznych. Rozwiązania różnią się filozofią tworzenia plików skryptowych, ale jako że&nbsp;różnice są bardziej kosmetyczne niż efektywne,
* wybór konkretnego rozwiązania był podyktowany preferencją autora i&nbsp;względami historycznymi (wszystkie wersje Pixie Engine opierały się na&nbsp;Apache Ant,
* z&nbsp;czego wynikała istniejąca baza skryptów którą można było po&nbsp;prostu zaadoptować do&nbsp;obecnego projektu)
*
* W&nbsp;Pixie Engine jest włączona jedynie binarna dystrybucja Apache Ant.  
*
* @section dependency_blowfish Blowfish
* System szyfrujący Blowfish@cite Blowfishcipher jest uznanym algorytmem blokowym operującym na&nbsp;8-bajtowych blokach danych (64 bity).
* Algorytm opiera się o&nbsp;dwa przebiegi: rozwijanie klucza i&nbsp;szyfrowanie danych. Klucz może zawierać od&nbsp;1 do&nbsp;56 znaków i&nbsp;jest rozwijany do&nbsp;4168 bajtów.
* Blowfish opiera się na&nbsp;16 rundach kryptograficznych, a&nbsp;siła algorytmu bierze się z&nbsp;zastosowania nieliniowej operacji S-Block
*
* W&nbsp;Pixie Engine blowfish został włączony ze&nbsp;względu na&nbsp;potrzebę szybkiego, symetrycznego algorytmu szyfrującego dane, niezbędne do&nbsp;przesłania
* przez sieć, bądź zapisania na&nbsp;dysku. Wybór konkretnego algorytmu był podyktowany względami dostępności - praktycznie każdy współczesny symetryczny szyfr blokowy
* spełniałby wymogi stawiane przed algorytmem, ale istnieje prosta, efektywna i&nbsp;oparta o&nbsp;wolną licencję Ms-PL implementacja Blowfisha
*
* @section dependency_boost Boost - rozszerzone biblioteki C++
* Jako iż&nbsp;standardowa biblioteka C++, nawet rozszerzona o&nbsp;funkcje C++11 i&nbsp;C++14 zawiera relatywnie ograniczony zestaw narzędzi, Pixie Engine wymaga
* rozszerzonej nieoficjalnej biblioteki Boost@cite BoostCLibraries. Boost jest dojrzałą, aktywnie rozwijaną i&nbsp;recenzowaną przez specjalistów biblioteką - lub raczej zbiorem bibliotek
* umożliwiającą - i&nbsp;optymalizującą - wiele typowych operacji jakie można spotkać podczas tworzenia oprogramowania w&nbsp;C++.
* Pełna lista komponentów Boost jest dostępna tutaj: https://www.boost.org/doc/libs/
*
* Na&nbsp;ten moment Pixie Engine korzysta m.in. z&nbsp;komponentu Spirit, umożliwającego parsowanie wyrażeń tekstowych, inteligentnych wskaźników oraz wyrażeń czasowych.
* Kolejne komponenty będą integrowane w&nbsp;miarę potrzeb.
*
* @section dependency_di Boost Dependency Injection
* Biblioteka wstrzykiwania zależności@cite BoostDI jest propozycją rozszerzenia Boost (obecnie oczekującą na&nbsp;recenzenta), ale do&nbsp;tej pory nie została włączona w&nbsp;zestaw bibliotek Boost
* i&nbsp;pozostaje niezależnym produktem. Jest to relatywnie młoda biblioteka, wciąż aktywnie rozwijana przez twórców. Więcej informacji na&nbsp;temat wykorzystania
* wstrzkiwania zależności i&nbsp;wykorzystania wzorca odwrócenia odpowiedzialności w&nbsp;Pixie Engine znajduje się w&nbsp;rozdziale @ref engine_architecture "architektura silnika".
*
* Podczas praktycznego wykorzystania wstrzykiwania zależności Boost natrafiono na&nbsp;ciekawy problem z&nbsp;tworzeniem fabryk abstrakcyjnych. Opis problemu i&nbsp;rozwiązanie
* zaimplementowane przez twórcę Pixie Engine znajduje się w&nbsp;rozdziale @ref engine_problems_di "problem z&nbsp;wstrzykiwaniem zależności Boost".
*
* @section dependency_cereal cereal
* cereal@cite cereal (oficjalnie nazwa pisana z&nbsp;małej litery) to biblioteka łatwej serializacji danych C++ czyli zamiany drzewka referencji na&nbsp;strumień binarny bądź tekstowy
* i&nbsp;vice versa. Cereal zostało wybrane ze&nbsp;względu na&nbsp;nieinwazyjną składnię, rozpoznawanie polimorfizmu (dzięki mechanizmom RTTI - rozpoznawania typów w&nbsp;trakcie
* wykonania), relatywnie proste API, dużą wydajność oraz łatwość implementacji nowych strumieni wyjściowych. Jego największym ograniczeniem jest problem
* z&nbsp;tworzeniem paska postępu zapisywania/wczytywania danych - istnieje tylko jeden przebieg i&nbsp;niewielkie możliwości podczepiania zdarzenia
* pod wykrycie nowego obiektu (bez opcji detekcji liczby obiektów). Z&nbsp;tego powodu w&nbsp;praktycznym zastosowaniu cereal musi być uruchamiany
* dwukrotnie (pierwszy przebieg jałowy, bez strumieniowania danych tylko po&nbsp;to by&nbsp;policzyć liczbę obiektów w&nbsp;grafie, i&nbsp;drugi - faktycznie serializujący)
*
* @section dependency_cg cg
* cg@cite CgprogramminglanguageWikipedia (oficjalnie nazwa pisana z&nbsp;małej litery) to autorska biblioteka firmy nVidia, nierozwijana i&nbsp;nieutrzymywana od&nbsp;kwietnia 2012, ale dostatecznie
* dojrzała i&nbsp;stabilna by&nbsp;zyskać szerokie zastosowania. cg to abstrakcyjny język jednostki cieniującej, umożliwiający transkompilację do&nbsp;kodu GLSL oraz HLSL
* z&nbsp;wykorzystaniem konkretnych list cech danej wersji języka. W&nbsp;ten sposób programy jednostki cieniującej mogą zostać stworzone przez grafika jednorazowo
* i&nbsp;być od&nbsp;razu dostępne na&nbsp;wszystkich wspieranych rendererach. Uzyskuje się w&nbsp;ten sposób warstwę niezależności zasobów nad konkretną implementacją.
* 
* Pomimo iż&nbsp;biblioteka nie jest dłużej wspierania, jest wciąż powszechnie wykorzystywana w&nbsp;branży, np. pełną integrację z&nbsp;cg oferuje Unity.
* Ponieważ cg nie jest już dłużej rozwijane, nie oferuje on dostępu do&nbsp;podprogramów i&nbsp;procedur GLSL oraz HLSL powstałych po&nbsp;kwietniu 2012.
*
* @section dependency_cppcheck C++ Check
* Jednym z&nbsp;najważniejszych wyzwań stawianych przed współczesnym oprogramowaniem jest jego stabilność. Testy możemy z&nbsp;grubsza podzielić na&nbsp;ręczne -
* podczas których tester fizycznie próbuje "popsuć" program i&nbsp;zapamiętuje kroki niezbędne by&nbsp;taką sytuację powtórzyć, oraz automatyczne - gdy inny program
* stara się "popsuć" nasz program. Istnieje wiele podejść do&nbsp;testów automatycznych, np. testy jednostkowe, które sprawdzają rezultat konkretnego
* fragmentu kodu, testy makietowe (zaimplementowane w&nbsp;Pixie Engine przy użyciu biblioteki Google Mockup) które opierają
* się o&nbsp;weryfikację komponentu w&nbsp;otoczeniu makiet innych komponentów, oraz testy integracyjne, które opierają się na&nbsp;całym systemie.
*
* Osobną gałąź stanowią testy statyczne, oparte wyłącznie na&nbsp;maszynowej analizie kodu źródłowego z&nbsp;pominięciem fazy kompilacji i&nbsp;uruchomienia programu.
* Testy statyczne mogą być oparte o&nbsp;ręczną analizę, ale powstały narzędzia zdolne przeprowadzić taką analizę w&nbsp;sposób całkowicie nienadzorowany.
*
* Najważniejszym narzędziem tego typu jest C++ Check@cite CppcheckAtoolforstaticCCcodeanalysis, statyczny analizator kodu źródłowego który wyłuskuje potencjalne błędy (np. niezainicjowane zmienne,
* niezwolnioną pamięć, niezłapane wyjątki), posługując się wyłącznie oryginalnymi kodami źródłowymi z&nbsp;pominięciem budowania pliku wykonywalnego.
* C++ Check jest z&nbsp;powodzeniem wykorzystywany w&nbsp;produkcyjnych aplikacjach (między innymi wykrył 27 błędów w&nbsp;jądrze Linuxa w&nbsp;wersji 2.6@cite CppCheckFoundBugs) i&nbsp;jest
* uznany za&nbsp;dojrzałe, dodatkowe narzędzie weryfikujące kod. Minusem rozwiązania jest czas analizy - 6 rdzeni sprawdzało 34 pliki *.cpp Pixie w&nbsp;ciągu
* blisko 45 minut.
*
* W&nbsp;przyszłości Pixie Engine może automatycznie uruchamiać C++ Check przy każdej prośbie o&nbsp;dołączenie kodu. W&nbsp;ten sposób jakość kodu
* może zostać utrzymana przez analizę potencjalnych braków.
* 
* Więcej informacji na&nbsp;temat sposobów testowania Pixie Engine znajduje się w&nbsp;rozdziale @ref engine_tests "testy".
*
* @section dependency_debugbreak DebugBreak
* DebugBreak@cite DebugBreak to niewielka - jednonagłówkowa - biblioteka zdolna przełączyć punkt zatrzymania za&nbsp;pomocą wstawki assembly (zwykle zaimplementowanej jako
* pułapka bądź przerwanie procesora) na&nbsp;wszystkich wspieranych platformach. W&nbsp;projekcie jest wykorzystana głównie na&nbsp;etapie tworzenia oprogramowania
* lub testów. Dzięki zastosowaniu DebugBreak programista może zobaczyć wykryty problem (np. nadpisanie pamięci) w&nbsp;chwili jest wystąpienia.
*
* DebugBreak *nie* jest włączony do&nbsp;finałowej kompilacji (ze względu na&nbsp;brak dołączanych symbol debuggera, jego obecność nie miałaby sensu),
* dlatego w&nbsp;przypadku zawieszenia aplikacji na&nbsp;komputerze klienckim stosowane są inne mechanizmy zbierania i&nbsp;raportowania błędów.
*
* @section dependency_dia dia
* dia@cite DebugInterfaceAccessSDK (oficjalnie nazwa pisana z&nbsp;małej litery) to framework niezbędny do&nbsp;poprawnego działania LLVM/Clang. Poza zastosowaniem
* w&nbsp;kompilatorze nie jest wykorzystywany bezpośrednio w&nbsp;projekcie.
*
* @section dependency_dot dot
* dot@cite GraphvizGraphVisualizationSoftware (oficjalnie nazwa pisana z&nbsp;małej litery) to niewielkie narzędzie do&nbsp;generowania graficznych diagramów używane wewnętrznie przez
* doxygen. Poza wykorzystaniem przy tworzeniu dokumentacji nie jest wykorzystywane bezpośrednio w&nbsp;projekcie.
*
* @section dependency_doxygen doxygen
* doxygen@cite DoxygenMainPage (oficjalnie nazwa pisana z&nbsp;małej litery) stanowi dojrzały, stabilny system generowania dokumentacji bezpośrednio z&nbsp;kodu źródłowego.
* doxygen wyłuskuje otagowane specjalnymi komentarzami komendy i&nbsp;tworzy z&nbsp;nich opis programu wraz z&nbsp;informacjami wyciągniętymi bezpośrednio
* z&nbsp;kodu źródłowego. Narzędzie jest w&nbsp;stanie wygenerować graf obiektów (także graficzny, z&nbsp;wykorzystaniem narzędzia dot),
* oraz odczytać podstawowe informacje z&nbsp;nagłówków klas, funkcji i&nbsp;metod. doxygen jest udostępniany na&nbsp;licencji GNU GPL, ale w&nbsp;projekcie jest uwzględniona
* wyłącznie jego binarna wersja (bez włączania kodów źródłowych samego doxygena).
*
* Wybór doxygena jako narzędzia generującego dokumentację był dość oczywisty - jest to obecnie najbardziej znane, najpowszechniej stosowane rozwiązanie
* w&nbsp;przypadku tworzenia aplikacji C++. Posiada największą bazę odpowiedzi, najlepszą dokumentację i&nbsp;uchodzi za&nbsp;najbardziej stabilne spośród wszystkich
* narzędzi dostępnych na&nbsp;rynku.
*
* doxygen jest zdolny do&nbsp;generowania dokumentacji w&nbsp;różnych językach i&nbsp;formatach wyjściowych.
*
* @section dependency_fastbuild FastBuild
* Jednym z&nbsp;najbardziej irytujących problemów na&nbsp;etapie tworzenia kodu Pixie Engine były bardzo długie czasy kompilacji projektu (ponad 15 minut na&nbsp;pełen cykl).
* Ze&nbsp;względu na&nbsp;realny wpływ na&nbsp;czas prac nad silnikiem, została powzięta decyzja by&nbsp;znaleźć alternatywny sposób (nie oparty o&nbsp;Apache Ant)
* sposób dokonywania szybkich, iteracyjnych kompilacji testowych, pozostawiając jednocześnie mechanizm pełnej rekompilacji do&nbsp;tworzenia finałowych
* wersji plików wykonywalnych.
*
* Rozważano kilka rozwiązań spośród których wybrano właśnie narzędzie FastBuild@cite FASTBuildHighPerformanceBuildSystem. FastBuild to relatywnie młody, zaawansowany system budujący
* oparty o&nbsp;proces iteracyjny z&nbsp;zachowaniem maksymalnej ilości danych między kompilacjami w&nbsp;pamięci podręcznej. W&nbsp;przeciwieństwie do&nbsp;innych rozważanych
* rozwiązań jest niezależny od&nbsp;wybranego kompilatora i&nbsp;potrafi się dobrze
* integrować z&nbsp;istniejącymi systemami budowania. Fastbuild potrafi rozłożyć kompilację na&nbsp;wiele rdzeni, sprawdzić różnice plików na&nbsp;poziomie makr preprocesora
* i&nbsp;wykryć zmiany bez stosowania mechanizmów prekompilacji nagłówków.
*
* Zastosowanie FastBuild skróciło podstawowy cykl kompilacji na&nbsp;8-rdzeniowym procesorze 6x-7x razy, a&nbsp;przy minimalnej liczbie zmian kodu (budowa iteracyjna)
* pozwolił osiągnąć blisko 20x-25x wzrost prędkości kompilacji.
*
* FastBuild nie zmienia architektury operacji kompilowania i&nbsp;nie wpływa na&nbsp;proces konsolidacji. Z&nbsp;tego powodu jest on transparentny dla ewentualnych
* problemów lub błędów, które mogą się pojawić w&nbsp;kodzie produkcyjnym i&nbsp;które mogłyby pozostać niezauważone do&nbsp;wykonania pełnego cyklu kompilacji.
*
* FastBuild pozwala także na&nbsp;rozłożenie kompilacji na&nbsp;więcej niż jedną fizyczną maszynę, ale taka funkcjonalność nie została wykorzystana w&nbsp;niniejszej pracy.
*
* @section dependency_git git
* Podczas tworzenia jądra Linuxa jego twórcy stanęli przed problemem koordynowania prac w&nbsp;zespole rozproszonym po&nbsp;całym świecie z&nbsp;otwartą możliwością
* kontrybuowania kodów źródłowych z&nbsp;zewnątrz zespołu. W&nbsp;takiej sytuacji oczywistym rozwiązaniem jest użycie systemu kontroli wersji, systemu który
* zapewnia możliwość tworzenia kontrolowalnego repozytorium zdolnego do&nbsp;zapamiętania (i odwrócenia) dowolnej zmiany w&nbsp;produkcie. W&nbsp;owych czasach
* na&nbsp;rynku znajdowało się kilka produktów tej klasy, jednak żaden nie spełniał wymagań twórców Linuxa. Stojąc przed oczywistą blokadą, twórcy Linuxa podjęli
* decyzję o&nbsp;stworzeniu autorskiego, własnego rozwiązania które nazwali właśnie gitem.@cite GitWikipedia Tak powstał obecnie najpopularniejszy - i&nbsp;wykorzystywany także w&nbsp;Pixie Engine -
* zdecentralizowany system kontroli wersji, zdolny do&nbsp;łatwego scalania zmian pochodzących z&nbsp;różnych gałęzi i&nbsp;umożliwiający ocenę i&nbsp;akceptację kodu pochodzącego
* z&nbsp;zewnątrz projektu (w formie tzw. próśb o&nbsp;ściągnięcie).
*
* Analogicznym rozwiązaniem do&nbsp;systemu kontroli git jest Mercurial@cite MercurialSCM (hg), posiadający bardzo podobny zestaw cech, ale w&nbsp;przeciwieństwie do&nbsp;git operujący na
* zmianach (ang. changes) a&nbsp;nie stanach (ang. snapshots). Autor niniejszej pracy preferował by&nbsp;system Mercurial jednak najpopularniejszy
* serwis oprogramowania otwartych-źródeł, github który przechowuje repozytoria kodu źródłowego projektu, nie dopuszczał repozytoriów hg (w momencie rozpoczynania projektu). Migracja z&nbsp;git do&nbsp;hg nie została do&nbsp;dziś
* przeprowadzona
*
* Dodatkowym problemem z&nbsp;gitem jest ilość binarnych plików (np. 4,7 gb zależności) jakie naturalnie pojawiają się w&nbsp;projektach gier. Git rozwiązuje problem
* częściowo za&nbsp;pomocą rozszerzenia Magazyn Dużych Plików (Large File Storage - LFS), jednak jego wsparcie w&nbsp;środowisku produkcyjnym nie jest w&nbsp;pełni
* stabilne (np. wspomniany github dopuszcza jedynie 1 gb plików binarnych). Z&nbsp;tego powodu zależności muszą być utrzymywane osobno; obecnie są przechowywane
* *poza* systemem wersjonowania na&nbsp;serwerze Kosek.com
*
* Oczywiście utrzymywanie plików źródłowych git w&nbsp;projekcie nie miałoby sensu - Pixie Engine używa jedynie binarnej wersji gita.
*
* @section dependency_glew glew
* OpenGL, jeden z&nbsp;dwóch rendererów wspieranych przez Pixie Engine do&nbsp;niedawna był niemal całkowicie zamrożony w&nbsp;produkcji - konsorcjum twórców OpenGL
* nie dopuszczało zmian standardu, poza tworzeniem (np. przez producentów kart graficznych) autorskich rozszerzeń.@cite OpenGLWikipedia O&nbsp;ile sytuacja ta zmieniła się niedawno,
* o&nbsp;tyle wciąż ogromna część elementów OpenGL jest zaimplementowana właśnie jako wspomniane rozszerzenia (z których cześć stała się de facto elementami
* standardu). Sprawdzanie dostępności rozszerzeń na&nbsp;danej architekturze i&nbsp;weryfikowanie możliwości kart graficznych stało się dość niewdzięcznym zadaniem - 
* zwłaszcza iż&nbsp;ta sama funkcjonalność potrafiła być zawarta w&nbsp;różnych rozszerzeniach zależenie od&nbsp;producenta danej karty graficznej (a czasem różnić
* się w&nbsp;detalach implementacyjnych!). GLEW@cite GLEW (OpenGL Extension Wrangler Library), to zestaw nagłówków i&nbsp;bibliotek które uproszczają proces sprawdzania i&nbsp;uruchamiania
* opisanych rozszerzeń.
* 
* @section dependency_googlemock Google Mock
* Google Mock@cite GoogleTestMock to zaawansowana biblioteka do&nbsp;tworzenia makiet (ang. mockups) kodu na&nbsp;potrzeby testowania modułów. Makiety, to niewielkie "wydmuszkowe" klasy
* które nie posiadają faktycznej implementacji, a&nbsp;są zdolne do&nbsp;odnotowania aktu wywołania dowolnej z&nbsp;własnych metod, lub dostępu do&nbsp;akcesorów.
*
* Przykładem zastosowania Google Mock może być następująca sytuacja: rozważmy moduł ustawień. Akt zapamiętania ustawienia powinien wywołać funkcję zapisu
* magazynu ustawień. Możemy wywołać metodę zapamiętania ustawień na&nbsp;faktycznej implementacji menadżera ustawień przy jednoczesnym wstrzyknięciu makiety magazynu.
* Oczywiście makieta powinna odnotować prośbę o&nbsp;zapis - brak takiej prośby powinien być zgłoszony jako błąd.
*
* @section dependency_googletest Google Test
* Google Test@cite GoogleTestMock to rozbudowane narzędzie Google do&nbsp;tworzenia testów jednostkowych. O&nbsp;ile Google Test i&nbsp;opisany powyżej Google Mock zaczęły jako osobne projekty, obecnie
* są wykorzystywane niemal zawsze wspólnie. Google Test pozwala na&nbsp;tworzenie przypadków testowych, ewaluowania środowiska testowego i&nbsp;sprawdzanie warunków korzystając
* z&nbsp;bogatej biblioteki asercji. Testy wykonywane są jeden po&nbsp;drugim na&nbsp;wyczyszczonym środowisku testowym, co pozwala na&nbsp;ewaluacje i&nbsp;późniejszą agregację danych z&nbsp;wielu
* przebiegów testowych
*
* @section dependency_gtest_runner Google Test runner
* Google Test runner@cite GoogleTestRunner to niewielkie narzędzie, wbrew nazwie *nie* tworzone przez Google ani powiązane podmioty. Narzędzie to pozwala na&nbsp;uruchamianie testów
* stworzonych za&nbsp;pomocą Google Test i&nbsp;generowanie na&nbsp;podstawie przebiegów raportów w&nbsp;formacie XML. Pixie Engine wykorzystuje jedynie wariant z&nbsp;linii komend, ale oczywiście
* nic nie uniemożliwia uruchomienia tego narzędzia ręcznie w&nbsp;trybie graficznym
*
* @section dependency_html_help_workshop HTML Help Workshop
* HTML Help Workshop to bezpłatne narzędzie Microsoftu zdolne przygotować skompilowany plik pomocy (chm@cite MicrosoftCompiledHTMLHelpWikipedia ) z&nbsp;zadanych źródeł przygotowanych w&nbsp;formie 
* strony internetowej (html). HTML Help Workshop jest wykorzystywany w&nbsp;Pixie Engine jedynie jako narzędzie z&nbsp;wiersza polecenia przez doxygena
* na&nbsp;potrzeby generowania dokumentacji na&nbsp;platformie Windows.
* 
* W&nbsp;Pixie Engine jest włączona jedynie binarna dystrybucja HTML Help Workshop. 
* 
* @section dependency_levenshtein Kalkulator odległości Levenshteina
* Odległość Levenshteina między dwoma ciągami danych to minimalna ilość operacji jednostkowych (dodania, zmiany, bądź usunięcia) na&nbsp;dowolnym ciągu
* potrzebnych by&nbsp;oba ciągi stały się identyczne.@cite LevenshteindistanceWikipedia
* 
* Przykład, dla danych ciągów:
*  - abcb
*  - ccb
*
* Odległość Levenshteina wynosi 2 (wystarczy usunąć literkę 'a' z&nbsp;przodu pierwszego i&nbsp;zamienić kolejną 'b' na&nbsp;'c').
*
* Kalkulator odległości Levenshteina to prosta biblioteka o&nbsp;otwartych źródłach która oblicza wartość odległości dla dowolnych ciągów o&nbsp;porównywalnych elementach.
* 
* @section dependency_jdk JDK 1.8
* Java Development Kit (JDK)@cite JavaSE to pełne SDK i&nbsp;platforma uruchomieniowa Javy, współczesnego języka zarządzanego. W&nbsp;projekcie jest używana wyłącznie wersja
* prekompilowana w&nbsp;formie binarnej przenośnej - jest to zależność niezbędna do&nbsp;działania Apache Ant i&nbsp;poza nim nie jest wykorzystywana.
*
* @section dependency_llvm LLVM
* Kompilator języka C++ to jeden z&nbsp;najbardziej skomplikowanych programów jaki można współcześnie napisać. Na&nbsp;rynku, do&nbsp;niedawna, istniały tylko trzy
* produkcyjne implementacje: GCC, MSVC i&nbsp;Intel C++ Compiler, spośród których tylko GCC był darmowy i&nbsp;posiadał otwarte źródła. Jedną z&nbsp;najbardziej problematycznych
* cech GCC był styl jego wewnętrznej implementacji. Kompilator był tworzony bez żadnego planu i&nbsp;posiadał bardzo chaotycznie rozrzucony kod źródłowy, pełen
* haków i&nbsp;skrótów utrudniających modyfikacje.@cite Comparingclangtootheropensourcecompilers
*
* W&nbsp;2000 roku na&nbsp;uniwersytecie Illinois założono nowy projekt LLVM (początkowo skrót rozwijał się do&nbsp;Low-Level Virtual Machine)@cite TheLLVMCompilerInfrastructureProject; Celem było sprawdzenie
* metod optymalizacji kodu pośredniego przed etapem asemblacji. Wkrótce okazało się że&nbsp;ze względu na&nbsp;"czystą" implementację projekt posiada znacznie szerszą
* gamę zastosowań.@cite LLVMWikipedia Pięć lat później powstała fasada CLang, kompilująca C++ do&nbsp;postaci pośredniej na&nbsp;potrzeby LLVM. W&nbsp;ten sposób oba projekty stworzyły podwalny
* środowiska pod nową rodzinę kompilatorów. Obecnie LLVM to ponad 20 projektów (wliczając preprocesor, kompilator, konsolidator, linker, debugger, assembler,
* analizator, etc.) i&nbsp;jest produkcyjnie gotowy do&nbsp;kompilowania projektów na&nbsp;wszystkich wspieranych przez Pixie Engine platformach. Zastosowanie jednego kompilatora
* znacząco upraszcza fazę debuggowania i&nbsp;testowania (nie ma np. problemów z&nbsp;miskompilacją generowaną przez różne narzędzia).
* 
* Jako ciekawostkę można zauważyć że&nbsp;od 2017 linker LLVM jest w&nbsp;stanie wygenerować symbole debuggera systemu Window (.pdb)@cite ThePDBFileFormatLLVM7documentation, tym samym można wykorzystać wszystkie
* istniejące narzędzia debuggowania / testowania które nie są zgodne z&nbsp;symbolami stosowanymi w&nbsp;GCC. To pozwoliło m.in. na&nbsp;zastosowanie
* Open C++ Coverage oraz zrzucania informacji o&nbsp;kodzie w&nbsp;przypadku niezłapanego wyjątku.
*
* @section dependency_mathjax MathJax
* MathJax@cite MathJax to zaawansowany parser wyrażeń matematycznych w&nbsp;przeglądarce. W&nbsp;Pixie Engine jest wykorzystywany jako narzędzie
* pomocnicze do&nbsp;generowania wzorów matematycznych w&nbsp;niniejszej dokumentacji. Tylko binarna wersja jest dołączona.
*
* @section dependency_vlc libVLC
* VLC@cite VLCFeaturesVideoLAN to wieloplatformowe środowisko odtwarzacza multimediów, zdolne do&nbsp;wyświetlania filmów w&nbsp;różnych formatach, konwersji i&nbsp;rekompresji strumieni.
* Jedną z&nbsp;najważniejszych cech VLC jest brak założeń co do&nbsp;systemu w&nbsp;którym odtwarzacz jest uruchamiany - podstawa kodowa zawiera wszystkie realnie liczące się
* kompresory i&nbsp;dekompresory (tzw. kodeki), filtry oraz zasoby generujące strumienie wyjściowe.
*
* libVLC@cite libVLCVideoLANWiki to biblioteka C++ na&nbsp;bazie której odtwarzacz VLC jest skonstruowany. Pixie Engine wykorzystuje libVLC do&nbsp;odtwarzania filmów bezpośrednio w&nbsp;grze
* (np. logo producenta, przerywniki filmowe). libVLC wspiera zarówno wyświetlanie filmu bezpośrednio na&nbsp;ekranie jak i&nbsp;strumieniowanie do&nbsp;tekstury (DirectX i&nbsp;OpenGL).
* libVLC pozwala też na&nbsp;zrzucanie filmu bezpośrednio z&nbsp;ekranu (przechwytywanie wideo) i&nbsp;tworzenie zrzutów ekranów.
* 
* @section dependency_minizip miniZip
* Wczytywanie wielu plików z&nbsp;dysku (twardego, DVD, etc.) wiąże się z&nbsp;narzutem potrzebnym systemowi plików na&nbsp;odszukanie ścieżki na&nbsp;urządzeniu, otwarcie
* deskryptora zabezpieczeń i&nbsp;wreszcie otwarcie uchwytu do&nbsp;samego pliku. Z&nbsp;tego powodu łatwo zauważyć że&nbsp;jeden plik o&nbsp;rozmiarze 100 mb będzie kopiowany bądź
* czytany znacznie szybciej niż sto plików o&nbsp;rozmiarze 1 mb każdy.@cite Archive1 @cite Archive2 Ponieważ - jak już zauważono - gra to setki tysięcy zasobów - aby przyspieszyć czas wczytywania
* danych, powiązane zasoby najlepiej jest spakować w&nbsp;jedno (bądź kilka) archiwum. Dla wirtualnego systemu plików (Zobacz rozdział @ref vfs) Pixie Engine archiwum staje się transparentne
* i&nbsp;dostęp do&nbsp;danych w&nbsp;środku jest równie łatwy jak w&nbsp;przypadku niezależnych plików. Badania wykazały, że&nbsp;wczytując dane z&nbsp;archiwum można skrócić czasy ładowania
* o&nbsp;około 50%-60% dla 250+ plików przy założeniu że&nbsp;archiwum jest nieskompresowane.@cite Archive3
*
* Oczywiście kwestią otwartą pozostaje sam format archiwum. W&nbsp;początkowej fazie rozważano archiwum tar, jako najprostszy format zdolny przechować połączone pliki.
* Wiele gier korzysta z&nbsp;własnych formatów opracowanych przez ich twórców, czasem jednak stosuje się popularne formaty maskując je innym rozszerzeniem.
* Np. archiwum PK3 używane w&nbsp;Quake to po&nbsp;prostu archiwum zip.
*
* Opracowanie własnego formatu archiwum przekroczyłoby zakres niniejszej pracy, dlatego po&nbsp;rozważeniu argumentów za&nbsp;i&nbsp;przeciw wybrano jako format archiwum właśnie zip.
* Za&nbsp;tym formatem przemawia:
* - łatwe dodawanie plików bez potrzeby przepisania całego archiwum
* - bardzo szybkie czasy szukania po&nbsp;archiwum
* - wsparcie dla kompresji (zwykle nieużywane ze&nbsp;względu na&nbsp;narzut ładowania, ale przydatne przy przesyłaniu archiwum przez sieć)
* - wsparcie dla szyfrowania
* - szerokie wsparcie narzędzi
*
* miniZip@cite miniZip to otwarta biblioteka umożliwiająca zapisywanie i&nbsp;czytanie archiwum zip (także z&nbsp;wykorzystaniem funkcji zwrotnych).
*
* @section dependency_mingw MinGW
* Opisany powyżej LLVM ma jedno zasadnicze ograniczenie na&nbsp;platformie Windows - nie posiada natywnego kompilatora zasobów
* (pików rc)@cite LLVMrc, niezbędnych by&nbsp;dołączyć dodatkowe informacje do&nbsp;plików wykonywalnych na&nbsp;tej platformie. Potrzebny był zewnętrzny program 
* posiadający taką funkcjonalność. W&nbsp;chwili obecnej na&nbsp;rynku znajdują się tylko dwa kompilatory zasobów: MSVC (komercyjny) i&nbsp;program wchodzący
* w&nbsp;skład MinGW (Minimalistyczne Gnu dla Windows)@cite MinGW. Z&nbsp;oczywistych względów - licencji i&nbsp;przenośności - do&nbsp;Pixie Engine włączono ten drugi.
* 
* Poza kompilatorem zasobów, MinGW *nie* jest wykorzystywane w&nbsp;projekcie - w&nbsp;szczególności nie jest wykorzystywany kompilator GCC który wchodzi w&nbsp;jego skład.
*
* W&nbsp;Pixie Engine jest włączona jedynie binarna wersja MinGW
* 
* @section dependency_msxsl msxsl
* Ponieważ raporty Google Test runner są tworzone w&nbsp;formacie XML, istniała potrzeba konwersji do&nbsp;formatu bardziej czytelnego
* dla użytkownika nietechnicznego. Najprostszą opcją jest zastosowanie transformaty XSLT która generuje dokument HTML, czytelne przez każdą przeglądarkę stron
* internetowych na&nbsp;każdej wspieranej platformie. XSLT mógł zostać uruchomiony bezpośrednio w&nbsp;przeglądarce, ale testy wykazały że&nbsp;przeglądarki posiadają
* spore ograniczenia @cite xsltBrowserSupport (np. MSIE nie pozwoli na&nbsp;uruchomienie transformaty na&nbsp;dokumencie lokalnym). Jedynym sensownym rozwiązaniem było zastosowanie osobnego
* programu który wykona pełen przebieg transformacji. Na&nbsp;platformie Windows wybrano do&nbsp;tego celu msxml (Microsoft XML)@cite msxml, darmowy program uruchamiany z&nbsp;linii
* komend. Jest on obecnie podpięty do&nbsp;systemu testów (zobacz rozdział @ref engine_tests ) i&nbsp;uruchamiany automatycznie po&nbsp;każdym przebiegu testowym.
*
* @section dependency_multiconsoles Multiple Consoles
* Standardowo na&nbsp;platformie Windows nie ma możliwości alokowania więcej niż jednej konsoli dla programu. Istnieje także szereg ograniczeń, których nie da
* się łatwo ominąć, np. po&nbsp;wciśnięciu kombinacji klawiszy ctrl + break w&nbsp;konsoli system Windows zabije aplikację w&nbsp;ciągu 5 sekund, jeżeli ona sama nie
* wyłączy się wcześniej @cite ctrlbreakWinApi (tego mechanizmu nie da się w&nbsp;żaden sposób obejść ani wyłączyć @cite whyctrlbreakWinApi ).
*
* Jedynym sposobem na&nbsp;rozwiązanie tych problemów jest stworzenie zewnętrznej aplikacji (pliku wykonywalnego) która otwierałaby własną konsolę i&nbsp;przyjmowałaby
* polecenia wydruku informacji (np. poprzez gniazdo lub strumień komunikacyjny). Oczywiście taką aplikację można by&nbsp;uruchamiać kilkukrotnie aby
* uzyskać efekt wielu konsoli. Zamknięcie tej aplikacji nie wpływałoby też na&nbsp;zamknięcie głównego programu.
*
* Oczywiście jak przystało na&nbsp;środowisko otwartych źródeł, ktoś zrealizował już taką bibliotekę przed Pixie Engine. Tym samym zamiast tworzyć ponowną
* implementację, po&nbsp;prostu włączono taką bibliotekę - w&nbsp;tym wypadku jest ona autorstwa Zvika Ferentza i&nbsp;udostępniana na&nbsp;wolnej licencji.@cite Multipleconsolesforasingleapplication
*
* @section dependency_nsis NSIS
* Pierwszą rzeczą jaką zobaczy użytkownik danej gry jest jej program instalacyjny. Oczywiście na&nbsp;każdej platformie istnieją już gotowe wzorce
* których użytkownik się spodziewa gdy instaluje nowy program - na&nbsp;platformie Windows takich wzorcowych implementacji jest kilka, poczynając od
* InstallShield (komercyjny), idąc przez InstallMaker, a&nbsp;kończąc właśnie na&nbsp;otwarto źródłowym NSIS@cite NSIS .
*
* Wybrano NSIS ze&nbsp;względu na&nbsp;dużą zdolność konfigurowalności, liberalną licencję, stabilny i&nbsp;dojrzały kod oraz dużą ilość materiałów dostępnych
* w&nbsp;Internecie. Instalator NSIS opiera się o&nbsp;skrypty NSIS i&nbsp;jest zdolny do&nbsp;wykonania wszystkich poleceń potrzebnych do&nbsp;instalacji gry
* (w tym np. rejestracji gry w&nbsp;panelu Gry Windows 7+), oraz do&nbsp;stworzenia interaktywnego deinstalatora. Program jest także zdolny do&nbsp;generowania
* inkrementacyjnych łatek, które pozwalają poprawić pliki gry zawierając tylko niezbędne zmiany (i tym samym znacząco ograniczając rozmiar takiej łatki)
* 
* @image html dependency_nsis_image.jpg "Przykładowy zrzut ekranu NSIS"
* @image latex dependency_nsis_image.jpg "Przykładowy zrzut ekranu NSIS" height=5cm
*
* @section dependency_openal_soft OpenAL (Software)
* W&nbsp;latach 90, na&nbsp;wzór Otwartej Biblioteki Graficznej (OpenGL) powstała także Otwarta Biblioteka Audio (OpenAL). Projekt był rozwijany przez kilka
* firm zajmujących się tworzeniem kart dźwiękowych do&nbsp;komputerów i&nbsp;ostatecznie został porzucony wraz z&nbsp;rozwojem kart zintegrowanych na&nbsp;rynku Koreańskim.@cite OpenAL
*
* Ponieważ interfejs programistyczny OpenAL był w&nbsp;miarę sensownie ułożony i&nbsp;ustandaryzowany, pojawiła się otwarta implementacja oparta całkowicie o
* standardowy procesor (CPU), nie korzystająca ze&nbsp;wsparcia sprzętowego (m.in. SoundBlaster). W&nbsp;ten sposób projekt przetrwał do&nbsp;dnia dzisiejszego a
* biblioteka OpenAL Software posiada pełną funkcjonalność droższych kart dźwiękowych i&nbsp;jest w&nbsp;stanie realizować je całkowicie programowo.
*
* OpenAL został włączony do&nbsp;projektu jako zaplecze renderera dźwięku. Jego przewagą nad konkurencyjnymi rozwiązaniami jest jego pełna przenośność na&nbsp;
* wszystkich wspieranych przez Pixie Engine platfomrach.
*
* @section dependency_open_cpp_coverage Open C++ Coverage
* Elementem testowania aplikacji jest ocena (metryka) pokrycia kodu testami. Najczęściej wykorzystywana metryka określa stosunek linii w&nbsp;kodzie wykonanych przez testy
* do&nbsp;wszystkich linii kodu w&nbsp;programie i&nbsp;najczęściej wyraża się w&nbsp;procentach.
*
* W&nbsp;językach wyposażonych w&nbsp;refleksję (jak np. C# lub Java) taki test jest wyjątkowo łatwy do&nbsp;przeprowadzenia; W&nbsp;C++ potrzeba dodatkowych informacji
* debuggera by&nbsp;móc ustalić dokładną lokalizację wykonywanej linijki. Na&nbsp;szczęście wykorzystywany do&nbsp;kompilacji Pixie Engine LLVM jest w&nbsp;stanie stworzyć
* takie symbole, a&nbsp;program Open C++ Coverage@cite OpenCppCoverage jest w&nbsp;stanie pobrać z&nbsp;nich informacje o&nbsp;pokryciu w&nbsp;formacie XML (bezpośrednio integrującym się z
* Visual Studio Code); Istnieje także możliwość przygotowania raportu HTML z&nbsp;wylistowaniem dokładnie które linijki były testowane
* a&nbsp;które nie.
*
* @image html dependency_open_cpp_coverage_image.jpg "Przykładowy raport Open C++ Coverage"
* @image latex dependency_open_cpp_coverage_image.jpg "Przykładowy raport Open C++ Coverage" height=5cm
*
* @section dependency_pugixml pugixml
* Wybranym metaformatem danych w&nbsp;Pixie Engine jest język XML. Istnieje duża ilość narzędzi i&nbsp;parserów do&nbsp;tego języka, poszukiwania skupiały się więc na
* rozwiązaniu wydajnym i&nbsp;bezpiecznym. Obecnie istnieje ponad 20 parserów, z&nbsp;których najszybszym jest rapidxml. Drugie miejsce w&nbsp;rankingu zajmuje
* pugixml@cite pugixml, który w&nbsp;przeciwieństwie do&nbsp;rapidxml jest parserem dużo bezpieczniejszym (np. nie używa pustych wskaźników jako brakujących pól, co znacząco
* upraszcza kod sprawdzający potencjalne błędy bądź uszkodzone pliki). Oczywiście szybkość parsera wynika z&nbsp;jego podstawowego ogracznienia: jest to parser
* destruktywny in-situ, który znakuje oryginalne dane zerowymi bajtami i&nbsp;tworzy drzewko odniesień do&nbsp;oryginalnego łańcucha znaków. Dzięki takiemu podejściu
* pugixml jest zaledwie 3-4x wolniejszy niż użycie funkcji @c strlen() na&nbsp;tych samych danych.@cite pugixmlBenchmark
*
* @section dependency_python Python 3.5
* Python w&nbsp;wersji 3.5@cite python jest używany wewnętrznie przez debugger LLVM. Poza tym nie jest wykorzystywany bezpośrednio w&nbsp;projekcie.
*
* @section dependency_remotery Remotery
* Remotery@cite remotery to współczesny profiler gier oparty o&nbsp;gniazda sieciowe. Jako biblioteka integruje się z&nbsp;aplikacją i&nbsp;wystawia na&nbsp;gnieździe TCP informacje o
* czasach wykonania i&nbsp;próbkach renderera, a&nbsp;także informacje z&nbsp;dziennika i&nbsp;debuggera. Drugą częścią aplikacji jest strona www, która łączy się z&nbsp;portem
* TCP za&nbsp;pomocą websocket, odbiera dane od&nbsp;aplikacji i&nbsp;przedstawia je w&nbsp;graficznej formie.
*
* Zaletą tego rozwiązania jest to że&nbsp;profiler i&nbsp;kokpit nie muszą być uruchomione na&nbsp;tej samej maszynie - nawet nie musi być to ta sama architektura (!).
* Jedynym wymogiem jest by&nbsp;profiler był osiągalny przez sieć z&nbsp;komputera z&nbsp;kokpitem. W&nbsp;ten sposób można np. profilować konsole do&nbsp;gier, albo komórki.
*
* @image html dependency_remotery_image.jpg "Interfejs przeglądarkowy Remotery"
* @image latex dependency_remotery_image.jpg "Interfejs przeglądarkowy Remotery" height=7.5cm
*
* @section dependency_sdl SDL 2.0
* SDL@cite sdl to dojrzała, wieloplatformowa biblioteka podstawowa do&nbsp;tworzenia gier. Posiada standardowy zestaw funkcji zaimplementowany na&nbsp;każdej ze&nbsp;wspieranych
* przez Pixie Engine platform, takich jak obsługa okien, sterowników sprzętowych, uruchamianie rendera, sprawdzanie platformy, obsługa klawiatury, myszy i&nbsp;
* kontrolerów gier, etc. SDL zostało wybrane ze&nbsp;względu na&nbsp;otwarte źródła, dużą ilość dostępnych materiałów, dojrzałość kodu i&nbsp;stabilność implementacji.
*
* 
* @section dependency_sdl_net SDL NET 2.0
* SDL NET@cite sdl to wtyczka do&nbsp;SDL umożliwiająca programowanie wieloplatformowe gniazd sieciowych (TCP/UDP). W&nbsp;Pixie Engine SDL NET jest wybraną platformą do
* programowania gier siecowych i&nbsp;odpytywania serwera HTTP w&nbsp;celu sprawdzenia dostępności aktualizacji. SDL NET dostarcza warstwę abstrakcji nad
* surowe gniazda sieciowe na&nbsp;danej platformie. Biblioteka jest niskopoziomowa i&nbsp;pozwala na&nbsp;bezpośrednie wysyłanie i&nbsp;odbieranie ramek/pakietów oraz tworzenie
* i&nbsp;przywiązywanie gniazd sieciowych.
*
* Biblioteka jest całkowicie synchroniczna, z&nbsp;tego powodu aby była użyteczna w&nbsp;środowisku growym musi być opakowana w&nbsp;obiekty wielowątkowe.
*
* @section dependency_stb stb
* Biblioteki Sean T. Barrett (STB) to zbiór jedno nagłówkowych narzędzi programistycznych napisanych w&nbsp;czystym C. Cechuje je brak zewnętrznych zależności,
* prostota implementacji i&nbsp;duża wydajność.
*
* Narzędzia stb są opublikowane jako własność publiczna i&nbsp;są otwarte na&nbsp;nowe zgłoszenia kodu.
* 
* <center><table>
* <caption id="dependency_stb_table">Komponenty stb wykorzystywane w&nbsp;Pixie Engine</caption>
* <tr><th>Nazwa<th>Wersja<th>Linie kodu<th>Opis
* <tr><td>stb_image.h <td> 2.19<td>7462<td>Wczytywanie z&nbsp;plików/pamięci: JPG, PNG, TGA, BMP, PSD, GIF, HDR, PIC
* <tr><td>stb_truetype.h <td> 1.19<td>4853<td>Parser, rasteryzer i&nbsp;dekoder fontów truetype
* <tr><td>stb_image_write.h <td> 1.09<td>1568<td>Zapisuje obrazki na&nbsp;dysk w&nbsp;formatach: PNG, TGA, BMP
* <tr><td>stb_image_resize.h <td> 0.95<td>2627<td>Skaluje obrazki w&nbsp;pamięci w&nbsp;dobrej jakości
* <tr><td>stb_rect_pack.h <td> 0.11<td>624<td>Generator dynamicznych atlasów tekstur
* <tr><td>stb_textedit.h <td> 1.12<td>1404<td>Podstawy pod implementację własnego edytora tekstowego
* <tr><td>stb_perlin.h <td> 0.3<td>316<td>Generator szumu perlina
* <tr><td>stb_easy_font.h <td> 1.0<td>303<td>Prosty font do&nbsp;drukowania napisów debuggera
* </table>źródło: https://github.com/nothings/stb#stb_libs</center>
*
* @section dependency_texlive texLIVE
* texLIVE@cite texlive to darmowa, prosta wersja LaTeXa na&nbsp;platformę Windows służąca do&nbsp;generowania dokumentacji pdf bezpośrednio z&nbsp;kodu źródłowego.
* Poza włączeniem w&nbsp;system budowania dokumentacji nie jest używana w&nbsp;projekcie.
*
* @section dependency_thttpd thttpd
* thttpd@cite thttpd (oficjalnie nazwa pisana z&nbsp;małej litery) to bardzo prosty serwer HTTP używany w&nbsp;projekcie wyłącznie do&nbsp;testowania funkcjonalności aktualizacji.
* W&nbsp;wersji finałowej aplikacja odpytuje serwer twórcy danej gry szukając potencjalnych aktualizacji; Na&nbsp;potrzeby testów zapytania zewnętrzne byłyby kłopotliwe
* (np. trudno jest symulować awarię zdalnego serwera), dlatego na&nbsp;potrzeby testów adres serwera jest przepięty na&nbsp;adres thttpd, uruchamianego
* lokalnie. Dzięki takiemu podejściu można kontrolować i&nbsp;debuggować oba końce połączenia.
*
* Nie jest łatwo znaleźć dobry, pozbawiony zależności serwer HTTP. Thttpd został wybrany ze&nbsp;względu na&nbsp;jego nieinwazyjność (nie wymaga instalacji usługi,
* pozwala na&nbsp;wybór powiązanego portu i&nbsp;nie wykonuje aktywnie żadnego ze&nbsp;skryptów CGI).
*
* @section dependency_upx UPX
* Ponieważ pliki binarne Pixie Engine potrafią osiągnąć spore rozmiary (w tej chwili ponad 55 mb głównego pliku exe) aby przyspieszyć ich ładowanie do
* pamięci i&nbsp;ułatwić proces łatkowania potrzebne było narzędzie które skompresuje je dynamicznie.
* 
* W&nbsp;chwili obecnej na&nbsp;rynku znajduje się tylko jeden system pakujący pliki wykonywalne - właśnie UPX@cite upx (Universal Packer for eXecutables), zdolny pakować
* pliki wykonywalne wszystkich wspieranych przez Pixie Engine platform. UPX jest uruchomiony na&nbsp;ostatnim etapie budowania wersji końcowych gry
* i&nbsp;w wariancie BRUTAL zmniejszył rozmiar wspomnianego pliku exe prawie dziesięciokrotnie (do 6 mb).
*
* Pliki spakowane UPXem są wciąż poprawnymi plikami wykonywalnymi danej platformy i&nbsp;zawierają prosty program rozruchowy, który wypakowuje je do&nbsp;pamięci
* dynamicznej, po&nbsp;czym uruchamia właściwy program. Cały proces jest transparentny dla użytkownika.
*
* UPX powoduje też że&nbsp;próby wstecznej inżynierii kodu są nieco trudniejsze niż przy zastosowaniu "czystych" plików wykonywalnych.
*
* @section dependency_vscode Visual Studio Code
* Poprzednie wersje Pixie Engine powstały w&nbsp;Visual Studio Community Edition@cite msvc (MSVC). Z&nbsp;takim podejściem wiązało się sporo problemów: po&nbsp;pierwsze MSVC jest dostępny
* jedynie na&nbsp;platformę Windows, po&nbsp;drugie jest nieprzenośny (w znaczeniu: wymaga instalacji), a&nbsp;po trzecie ma bardzo wysokie wymagania sprzętowe, szczególnie
* jeżeli chodzi o&nbsp;ilość zajmowanego miejsca (pełna instalacja MSVC zabiera 130 gb@cite msvcdrivespace ).
* 
* Z&nbsp;chwilą upowszechnienia się wersji stabilnej VS Code@cite vscode pojawił się pomysł zmiany środowiska; Za&nbsp;VS Code przemawiało przede wszystkim możliwość
* stworzenia wersji przenośnej dołączonej bezpośrednio do&nbsp;projektu, ogromną bazę darmowych wtyczek, otwarte źródła i&nbsp;wsparcie jednej z&nbsp;największych firm sektora IT,
* czyli Microsoft. Zmiana środowiska nastąpiła pod koniec 2017 roku i&nbsp;okazała się strzałem w&nbsp;dziesiątkę. W&nbsp;tej chwili VS Code stanowi podstawowy edytor
* kodu na&nbsp;wszystkich wspieranych przez Pixie Engine platformach (sam edytor jest w&nbsp;pełni wieloplatformowy).
*
* @section dependency_wxwidgets wxWidgets
* wxWidgets to wieloplatforma biblioteka GUI, służąca w&nbsp;Pixie Engine za&nbsp;podstawę interfejsu aplikacji nie związanej z&nbsp;grą (np. edytora plansz, interfejsu startowego).
* 
* W&nbsp;chwili obecnej na&nbsp;rynku znajdują się dwie podstawowe biblioteki GUI: wxWidgets@cite wxwidgets i&nbsp;qt@cite qt. Porównanie obu znajduje się poniżej:
* 
* <center><table>
* <caption id="dependency_wxwidgets_table">Analiza porównawcza wxWidgets i&nbsp;qt metodą wag (-10/+10)</caption>
* <tr><th>wxWidgets<th>qt
* <tr><td>
* - Bardzo dobra jakość aplikacji końcowej (+9)
* - Natywny wygląd na&nbsp;każdej ze&nbsp;wspieranych platform (+3)
* - Łatwość integracji z&nbsp;biblioteką (+7)
* - Modułowość (+2)
* - Słabe API (szczególnie rozmiarowe, ang. sizer) (-5)
* </td><td>
* - Ujednolicony wygląd aplikacji na&nbsp;każdej ze&nbsp;wspieranych platform (-1)
* - Problemy z&nbsp;integracją w&nbsp;C++ (-9)
* - Modułowość (+2)
* - Dedykowany edytor okienek (+5)
* - Lepsze API (+2)
* </td></tr>
* <tr><td>+16<td>-1
* </table>Wynik analizy: wxWidets wygrywa przewagą 17 punktów</center>
*
* wxWidgets jest zintegrowany bezpośrednio z&nbsp;głównym modułem gry (więcej informacji znajduje się w&nbsp;rozdziale @ref engine_architecture "architektura silnika")
*
* @section dependency_windbg WinDBG (Debugging Tools for Windows)
* WinDBG@cite winDBG to prosty, darmowy debugger C++ integrujący się z&nbsp;VS Code. Poza debuggowaniem na&nbsp;platformie Windows nie jest używany w&nbsp;projekcie.
*
* WinDBG został wybrany ponad LLDB (debuggerem LLVM) ze&nbsp;względu na&nbsp;brakującą implementację tego drugiego; W&nbsp;chwili obecnej LLDB nie jest w&nbsp;stanie
* realnie pracować nad aplikacją Windowsa skompilowaną w&nbsp;architekturze x64. Twórcy LLVM znają ten problem od&nbsp;Marca 2017
* (https://bugs.llvm.org/show_bug.cgi?id=32343) jednak do&nbsp;dziś nie powstała implementacja naprawiająca.
* 
*
*/