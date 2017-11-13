
/**
* @page terminal_format Terminal output tagging
* @tableofcontents
* @section terminal_format_intro Introduction
*       Many of end platform system consoles have a way of
*       tagging terminal output (for example with different
*       colors). Pixie allows generic tags to be passed to
*       @ref engine::terminal_writer_t terminal_writer_t
*       converting to colored output on given platform
*       (colors for each tag can be easily adjutsed in
*       terminal settings, the one you pass to bootstrapper).
* @section terminal_format_tutorial How to use it? (Quick Start)
*		@code{.cpp}
*			terminal_writer->write("$error$This is an error message $error_meta$with some less important stuff..."_u);
*			// Outputs: "This is an error message with some less important stuff..."
*
*			terminal_writer->write("$success$Completed! $0$Back to default color..."_u, "comments"_u);
*			// Outputs: "Completed! Back to default color..."
*		@endcode 
* @section terminal_format_reference Reference
*		@subsection terminal_format_reference_structure Structure
*			@code
* $tag$
* $0$
*			@endcode
*		@subsection terminal_format_reference_tag Tag
*           One of predefined tags or your own (defined in `std` file)
*           For current list of tags see `_engine/code/include/common/def/terminal_writer.def`
*           
*           Tags replace each other. Therefore only one tag can be active at given time.
*   
*           Tags works from point of use till the end of given string. Therefore next call to
*           @ref engine::terminal_writer_t::write write will be back to defaul.
*
*           The special `0` tag, cancels the effect and returns to console default.
*
*           @note Terminal tags *does not* count for column position (when using in conjunction with @ref ustring_format utsring formatting), therefore tags for ustring and terminal *can be* safely mixed up.
*/