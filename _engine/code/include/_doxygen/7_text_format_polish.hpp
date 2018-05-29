
/**
* \~Polish @page string_format Formatowanie tekstu
* @tableofcontents
* @section string_format_intro Wprowadzenie
* Pixie Engine używa własnego, unikalnego sposobu formatowania tekstów unikodowych. Metoda ta jest znacznie
* bardziej adaptacyjna niż zwykłe, C++ formatowanie `printf`. Możliwe jest rozszerzenie formatera
* w przyszłości o nowe funkcje, ale sposób formatowania pozostanie zawsze zgodny wstecz.
* 
* Najważniejszą przewagą nad `printf` jest możliwość zmiany kolejności argumentów (np. jest inna
* kolejność wyrażeń w angielskim i niemieckim), pominięcia bądź powtórzenia danego argumentu oraz
* pełne wsparcie dla systemu unicode. Typy argumentów nie mają znaczenia (jak długo posiadają przeładowaną
* funkcję @c engine::to_string() ), podobnie jak ich liczba (wspierana jest każda ilość). System
* formatowania wspiera także przekazywanie dodatkowej informacji do wspomnianej metody `to_string`
* Przykład:
* @code{.cpp}
*    engine::format_string("Witaj mój #2# #1# świecie!"_u, "miły"_u, "naprawdę"_u);
*    // Tworzy: "Witaj mój naprawdę miły świecie!"
*
*    engine::format_string("W tagach mogą się znaleźć #1@co może być w tagach?#!"_u, "także komentarze"_u);
*    // Tworzy: "W tagach mogą się znaleźć także komentarze"
*
*    engine::format_string("Typy argumentów są wykryte w czasie kompilacji: #1# (#3#)"_u, true, 1.0F, "Jak widać, można także pominąć wartości!"_u);
*    // Tworzy: "Typy argumentów są wykryte w czasie kompilacji: True (Jak widać, można także pominąć wartości!)"
*
*    engine::format_string("Przeskocz do #1# #0:= 32:#kolumny!"_u, "zadanej"_u);
*    // Tworzy: "Przeskocz do zadanej                kolumny!"
*
*    engine::format_string("Rysuje #0:>-10:#> #1#"_u, "strzałkę!"_u);
*    // Tworzy: "Rysuje ---------> strzałkę!"
* @endcode
* @section string_format_reference Referencja
*		@subsection string_format_reference_structure Podstawowa struktura
*			@code
*				#numer#
*				#numer:formatowanie#
*				#numer@komentarz#
*				#numer:formatowanie@komentarz#
*			@endcode
*		@subsection string_format_reference_number Numer
*           Numer to naturalna liczba określająca argument, bądź 0 określające brak argumentu (pusty tekst).
*           Wartości 0 można użyć żeby wymusić podanie formatowania. Nie ma górnej wartości numeru.
*           Argument o danym numerze może się powtarzać. Nie można umieścić spacji między # a numerem.
*		@subsection string_format_reference_format Formatowanie
*           Formatowanie, które zostanie przekazane jako drugi argument funkcji @c engine::to_string()
*           W przypadku argumentu 0 zostanie przekazane do funkcji przeładowanej dla @c engine::ustring_t
*           i przyjmującej pusty argument.
*		@subsection string_format_reference_comment Komentarz
*           Każdy komentarz zaczyna się znakiem `@` i trwa aż do kończącego znaku `#`.
*           Komentarz zostanie zignorowany przez parser, ale może służyć jako podpowiedź dla tłumacza
*           jakiego rodzaju tekst zostanie podstawiony podczas formatowania. Komentarz może zawierać
*           dowolne znaki unicode
*		@subsection string_format_reference_escape Sekwencja ucieczki
*			Jeżeli potrzeba wstawić symbol `#` należy użyć go podwójnie, tzn. wpisać `##`.
*		@subsection string_format_reference_error Obsługa błędów
*			Próba użycia zbyt dużego numeru (w stosunku do liczby argumentów) spowoduje substytucję
*           pustym tekstem. Podobnie niepoprawnie zakończony tag zostanie potraktowany jako zwykły tekst.
*
*/