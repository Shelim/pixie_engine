
/**
* \~Polish @page ustring_format UString formatting
* @tableofcontents
* @section ustring_format_intro Introduction
*		Pixie uses custom way to format unicode string.
*		This way is much more flexible than original formatting.
*		It can be extended in next versions, but will be always kept
*		backward compatible.
*
*		The most important change done to standard C-like `printf` is
*		the ability to reorder arguments, as needed for translations.
*		Other changes includes full unicode support and ability to repeat
*		arguments more then once.
* @section ustring_format_tutorial How to use it? (Quick Start)
*		@code{.cpp}
*			engine::format_string("Hello my #2# #1# world!"_u, "nice"_u, "really"_u);
*			// Outputs: "Hello my really nice world!"
*
*			engine::format_string("You can have #1:what?# in tags too!"_u, "comments"_u);
*			// Outputs: "You can have comments in tags too!"
*
*			engine::format_string("Argument type is detected on compile time: #1# (#3#)"_u, true, 1.0F, "As you can see you can skip the numbers!"_u);
*			// Outputs: "Argument type is detected on compile time: True (As you can see you can skip the numbers!)"
*
*			engine::format_string("Jump to #1# #>32#column!"_u, "given"_u);
*			// Outputs: "Jump to given                  column!"
*
*			engine::format_string("Draw #>10:-#> #1#"_u, "arrow!"_u);
*			// Outputs: "Draw -----> arrow!"
*		@endcode
* @section ustring_format_reference Reference
*		@subsection ustring_format_reference_structure Structure
*			@code
*				#order#
*				#order:comment#
*				#>column#
*				#>column:character#
*			@endcode
*		@subsection ustring_format_reference_order Order
*			Order is a positive number starting from `1`. There is no upper limit for this value.
*			Tag with given order can be repeated (ie the same argument used more than once)
*			@warning There must be no spaces between # and order!
*		@subsection ustring_format_reference_comment Comment
*			Comment starts with `:` then span till ending `#` is reached. It will be completely
*			ignored by parser, but can give hint to the translators what will be substituted
*			during formatting pass. Can contain any unicode characters
*		@subsection ustring_format_reference_column Column
*			Jump current cursor position to given column (right jump only), padding with characters
*          (if no character is specified it will pad with spaces).
*          @warning If string contains @ref terminal_format "terminal formatting" tags (like $error$), they will be *IGNORED* for character count (this way, you can mix terminal and ustring formatting and still get reasonable results in terminal).
*          So: if you write `"$error$ #>4:-#>"` you will get: `"$error$ --->"`.
*          @warning Terminal tags will be removed only by terminal formatter! If you try to use them in other places, they *will* be printed as plain text.
*		@subsection ustring_format_reference_escape Escape hash
*			You can use ## every time you wish to escape given hash (## will be replaced by single # and ignored by parser)
*		@subsection ustring_format_reference_error Error handling
*			Formatting is safe for arguments having incorect values
*			In such case the formatting code will be safely substituted for empty string
*
* @see engine::ustring_t, engine::terminal_writer_t, engine::format_string
*/