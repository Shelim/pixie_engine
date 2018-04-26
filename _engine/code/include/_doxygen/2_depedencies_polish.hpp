/**
* \~Polish @page depedencies Zależności
* @section dependency_dashboard_into Wprowadzenie
* Wszystkie zależności są umieszczone w katalogu <tt>_engine/dependency</tt> w jednym z dwóch podkatalogów (@c library albo @c other)
*
* Jeżeli twój katalog <tt>_engine/dependency</tt> jest pusty, oznacza to najprawdopodobniej wykonanie klona z publicznego repozytorium bez ściągnięcia zależności.
* Dzieje się tak dlatego iż zależności liczą sobie - w chwili gdy piszę te słowa - około 4,5 gb i nie są ściągane domyślnie. Użyj następujących komend
* aby pobrać je z serwera Kosek.com:
*
* - Na Windowsie: użyj skryptu z katalogu <tt>_command/windows/download_dependencies.bat</tt>. Potwierdź czas oczekiwania.
* - Na Linuxe: TBD
* - Na OS X: TBD
*
* Lista zależności znajduje się poniżej:
*
* @section dependency_dashboard Pełna lista
* <center><table>
* <caption id="dependency_dashboard_table">Lista wszystkich zależności</caption>
* <tr><th>Nazwa<th>Rodzaj<th>Rola w projekcie<th>Licencja<th>Twórca
* <tr><td>@ref dependency_angelcode "AngelCode"<td>Biblioteka<td>Dostawca języka skryptowego<td>zlib<td>Andreas J&ouml;nsson
* <tr><td>@ref dependency_anttweakbar "AntTweakBar"<td>Biblioteka<td>Dostawca podstawowych narzędzi poprawiania UI<td>zlib/libpng<td>Philippe Decaudin
* <tr><td>@ref dependency_apacheant "Apache Ant"<td>Inne<td>Wieloplaformowy system kompilacji<td>Licencja Apache, wersja 2.0<td>The Apache Software Foundation
* <tr><td>@ref dependency_blowfish "Blowfish"<td>Biblioteka<td>Szybki moduł szyfrujący<td>The Microsoft Public License (Ms-PL)<td>George Anescu
* <tr><td>@ref dependency_boost "Boost"<td>Biblioteka<td>Zestaw nagłówków wspierających tworzenie aplikacji C++<td>Licencja Boost Software, wersja 1.0<td>Open Source Contributors
* <tr><td>@ref dependency_di "Boost Dependency Injection"<td>Biblioteka<td>Zestaw narzędzi wstrzykujący zależności<td>Licencja Boost Software, wersja 1.0<td>Kris Jusiak
* <tr><td>@ref dependency_cereal "cereal"<td>Biblioteka<td>Narzędzia serializacji<td>Licencja BSD<td>Randolph Voorhies, Shane Grant
* <tr><td>@ref dependency_cg "cg"<td>Biblioteka<td>Język jednostki ceniującej GPU<td>Wykorzystanie klienckie NVIDIA Software<td>NVIDIA
* <tr><td>@ref dependency_cppcheck "C++ Check"<td>Inne<td>Statyczny walidator kodu C++<td>GNU GPL (Bez włączenia kodów źródłowych do projektu)<td>Daniel Marjam&auml;ki
* <tr><td>@ref dependency_debugbreak "debugbreak"<td>Biblioteka<td>Wieloplatformowe programowe instrukcje przerwania<td>MIT/X11<td>Scott Tsai
* <tr><td>@ref dependency_dia "dia"<td>Inne<td>(Używany wewnętrznie przez LLVM)<td>GNU GPL (Bez włączenia kodów źródłowych do projektu)<td>Open Source Contributors
* <tr><td>@ref dependency_dot "dot"<td>Inne<td>(Używany wewnętrznie przez doxygen)<td>The Common Public License (Bez włączenia kodów źródłowych do projektu)<td>AT &amp;T
* <tr><td>@ref dependency_doxygen "doxygen"<td>Inne<td>Narzędzia do generowania dokumentacji<td>GNU GPL (Bez włączenia kodów źródłowych do projektu)<td>Dimitri van Heesch
* <tr><td>@ref dependency_fastbuild "FastBuild"<td>Inne<td>Akcelelator systemu budowania C++<td>Wariant BSD<td>Franta Fulin 
* <tr><td>@ref dependency_git "git"<td>Inne<td>System kontroli wersji<td>GNU LGPL v2.1<td>Linus Torvalds, Junio Hamano and others
* <tr><td>@ref dependency_glew "glew"<td>Biblioteka<td>Rozszerzenia dla OpenGL<td>Licencja Khronos (MIT)<td>Lev Povalahev and others
* <tr><td>@ref dependency_googlemock "Google Mock"<td>Biblioteka<td>Automatyczne testy makietowe<td>BSD 3-punktowa<td>Google Inc.
* <tr><td>@ref dependency_googletest "Google Test"<td>Biblioteka<td>Automatyczne testy jednostkowe<td>BSD 3-punktowa<td>Google Inc.
* <tr><td>@ref dependency_gtest_runner "Google Test runner"<td>Inne<td>Host automatycznych testów jednostkowych<td>MIT/X11<td>Nic Holthaus
* <tr><td>@ref dependency_html_help_workshop "HTML Help Workshop"<td>Inne<td>Narzędzie do generowania dokumentacji chm dla narzędzia @ref dependency_doxygen "doxygen"<td>Licencja końcowego użytkownika HTML Help<td>Microsoft
* <tr><td>@ref dependency_levenshtein "Levenshtein distance calculator"<td>Biblioteka<td>Obliczanie odległości Levenshteina między dwoma dowolnymi kolekcjami<td>???<td>Keith Schwarz
* <tr><td>@ref dependency_jdk "JDK 1.8"<td>Inne<td>Wymagane przez @ref dependency_apacheant "Apache Ant"<td>Licencja Sun<td>Oracle Corporation
* <tr><td>@ref dependency_llvm "LLVM"<td>Inne<td>LLVM: środowisko kompilujące Pixie Engine (kompilator, linker, debugger)<td>Licencja LLVM (Bez włączenia kodów źródłowych do projektu)<td>LLVM Team
* <tr><td>@ref dependency_mathjax "mathjax"<td>Inne<td>Renderer wyrażeń matematycznych w przeglądarce<td>Licencja Apache, wersja 2.0<td>American Mathematical Society &amp; Society for Industrial and Applied Mathematics
* <tr><td>@ref dependency_vlc "libVLC"<td>Biblioteka<td>Odtwarzanie filmów<td>GNU LGPL 2<td>VideoLAN
* <tr><td>@ref dependency_minizip "miniZip"<td>Biblioteka<td>Kompresor/dekompresor archiwów<td>Wariant BSD<td>Nathan Moinvaziri
* <tr><td>@ref dependency_mingw "MinGW"<td>Inne<td>Kompilator plików zasobów na platformie Windows<td>MIT/X11<td>Open Source Contributors
* <tr><td>@ref dependency_msxsl "msxsl"<td>Inne<td>Transformator XML / XSLT działający z wiersza polecenia<td>???<td>msxsl
* <tr><td>@ref dependency_multiconsoles "Multiple Consoles"<td>Biblioteka<td>Pomocnicza aplikacja umożliwiająca alokację więcej niż jednego okna konsoli dla jednego procesu<td>Domena publiczna<td>Zvika Ferentz
* <tr><td>@ref dependency_nsis "NSIS"<td>Inne<td>Instalator na platformie Windows<td>zlib/libpng<td>Open Source Contributors
* <tr><td>@ref dependency_openal_soft "OpenAL (Software)"<td>Biblioteka<td>Moduł wspierający odtwarzanie dźwięków<td>GNU LPGL<td>kcat
* <tr><td>@ref dependency_open_cpp_coverage "Open C++ Coverage"<td>Inne<td>Aplikacja wyliczająca pokrycie kodu testami na podstawie symboli odpluskwiacza PDB<td>>GNU GPL (Bez włączenia kodów źródłowych do projektu)<td>OpenCppCoverage Team
* <tr><td>@ref dependency_pugixml "pugixml"<td>Biblioteka<td>Szybki wczytywacz i parser dokumentów XML<td>MIT/X11<td>Arseny Kapoulkine
* <tr><td>@ref dependency_python "Python 3.5"<td>Inne<td>Używany przez @ref dependency_llvm "LLVM" na potrzeby odpluskwiania aplikacji<td>PSF license agreement for Python<td>Python Software Foundation
* <tr><td>@ref dependency_remotery "Remotery"<td>Biblioteka<td>Zewnętrzny profiler w formie strony www<td>Licencja Apache, wersja 2.0<td>Don Williamson
* <tr><td>@ref dependency_sdl "SDL 2.0"<td>Biblioteka<td>Wieloplatformowa baza aplikacji<td>zlib<td>Open Source Contributors
* <tr><td>@ref dependency_sdl_net "SDL NET 2.0"<td>Biblioteka<td>Wieloplatformowe wsparcie dla sieci<td>GNU LGPL<td>Open Source Contributors
* <tr><td>@ref dependency_stb "stb"<td>Biblioteka<td>Proste pojedynczo nagłówkowe narzędzia<td>Domena publiczna<td>Sean Barrett
* <tr><td>@ref dependency_texlive "texLIVE"<td>Inne<td>Kompilator dokumentacji do formatu PDF<td>Licencja TL<td>Karl Berry
* <tr><td>@ref dependency_thttpd "thttpd"<td>Inne<td>Prosty serwer HTTP do testowania funkcjonalności aktualizacji<td>Wariant BSD<td>Jef Poskanzer
* <tr><td>@ref dependency_upx "UPX"<td>Inne<td>Uniwersalny kompresor plików binarnych, zmniejszający rozmiar plików wykonywalnych<td>Wariant BSD<td>Markus F.X.J. Oberhumer, L&aacute;szl&oacute; Moln&aacute;r & John F. Reiser.
* <tr><td>@ref dependency_vscode "Visual Studio Code"<td>Inne<td>Główny edytor w którym powstaje Pixie Engine<td>MIT/X11<td>Microsoft
* <tr><td>@ref dependency_wxwidgets "wxWidgets"<td>Biblioteka<td>GUI dla narzędzi<td>Licencja wxWindows<td>Open Source Contributors
* <tr><td>@ref dependency_windbg "WinDBG (Debugging Tools for Windows)"<td>Inne<td>Debugger wiersza poleceń podpięty do projektu Visual Studio Code<td>???<td>Microsoft
* </table></center>
*
* @section dependency_angelcode AngelCode
* Angelscript (albo Biblioteka Skryptowa AngelCode) to dojrzały, aktywnie rozwijany język skryptowy o składni opartej częściowo
* o C++ udostępniany na wolnej licencji zlib.
* 
* Wybór konkretnego języka skryptowego nie jest łatwy w dzisiejszych czasach z racji obecności na rynku wielu
* dojrzałych i gotowych-do-produkcji rozwiązań. W szczególności mogłoby zostać zadane pytanie czemu nie została wybrana Lua albo Python,
* najpopularniejsze obecnie języki skryptowe stosowane w grach; Prawdą jest, że o ile wydajnościowo Lua nieznacznie sprawuje
* się lepiej niż AngelScript, Angelscript z racji swojej C++ składni jest nieporównywalnie łatwiejszy do podpięcia do natywnego
* systemu klas (zwykle także z pominięciem konieczności istnienia obiektów proxy). Python jako język w tym samym zestawieniu
* wydajności sprawdza się o wiele gorzej a i jego zdolność do odwzorowania konstrukcji C++ jest dużo bardziej ograniczona.
* 
* <center><table>
* <caption id="dependency_angelcode_benchmark_table">Wydajność języków skryptowych w przebiegu testowym</caption>
* <tr><th>Nazwa<th>Czas wykonania<th>Względem Lua
* <tr><td>Lua<td>1,341 s.<td>100%
* <tr><td>Angelscript<td>1,859 s.<td>72%
* <tr><td>Python<td>2,842 s.<td>47%
* </table>źródło: https://github.com/r-lyeh-archived/scriptorium</center>
*
* @section dependency_anttweakbar AntTweakBar
* AntTweakBar to niewielka, łatwa do wykorzystania biblioteka która pozwala programistom i twórcom zawartości dodać prosty interfejs użytkownika
* na potrzeby interaktywnego dobierania parametrów. Biblioteka pozwala podpiąć dowolną zmienną C/C++ i umożliwia mechanizmy prostej modyfikacji
* wartości, na podstawie rozpoznanego typu (liczba, test, wektor, kolor, macierz, kwaternion). Biblioteka ta wspiera wszystkie współczesne stosowane
* renderery (OpenGL, DirectX, SDL)
* 
* Motywacją za włączeniem AntTweakBar było udostępnienie prostego narzędzia, które pozwoli parametryzować rozgrywkę (głównie do wykorzystania
* przy tworzeniu fanowski modyfikacji) bez opuszczania gry.
* @section dependency_apacheant Apache Ant
* Apache Ant to system budujący oparty o język XML zaimplementowany w Javie z wykorzystaniem komend języka skryptowego JavaScript. Posiada rozbudowaną
* bazę zadań (np. "wykonaj komendę na pliku", "skopiuj plik", "wgraj plik na serwer FTP", etc.), które można parametryzować z wykorzystaniem zmiennych.
*
* Istnieje na rynku wiele komercyjnie-gotowych systemów budujących, poczynając od makefile i cmake - bardzo dojrzałych narzędzi operujących na skryptach
* budowy, idąc przez SCONS oparty o pełnoprawny język skryptowy Python, a kończąc na Fastbuild (wykorzystany w Pixie Engine do budowania iteracyjnego)
* i rozwiązaniach wewnętrznych. Rozwiązania różnią się filozofią tworzenia plików skryptowych, ale jako że różnice są bardziej kosmetyczne niż efektywne,
* wybór konkretnego rozwiązania był podyktowany preferencją autora i względami historycznymi (wszystkie wersje Pixie Engine opierały się na Apache Ant,
* z czego wynikała istniejąca baza skryptów którą można było po prostu zaadoptować do obecnego projektu)
* 
* W Pixie Engine jest włączona jedynie binarna dystrybucja Apache Ant.  
*
* @section dependency_blowfish Blowfish
* System szyfrujący Blowfish jest uznanym algorytmem blokowym operującym na 8-bajtowych blokach danych (64 bity).
* Algorytm opiera się o dwa przebiegi: rozwijanie klucza i szyfrowanie danych. Klucz może zawierać od 1 do 56 znaków i jest rozwijany do 4168 bajtów.
* Blowfish opiera się na 16 rundach kryptograficznych, a siła algorytmu bierze się z zastosowania nieliniowej operacji S-Block
*
* W Pixie Engine blowfish został włączony ze względu na potrzebę szybkiego, symetrycznego algorytmu szyfrującego dane, niezbędne do przesłania
* przez sieć, bądź zapisania na dysku. Wybór konkretnego algorytmu był podyktowany względami dostępności - praktycznie każdy współczesny symetryczny szyfr blokowy
* spełniałby wymogi stawiane przed algorytmem, ale istnieje prosta, efektywna i oparta o wolną licencję Ms-PL implementacja Blowfisha
*
* @todo Dokończyć sekcję zależności
*
*/