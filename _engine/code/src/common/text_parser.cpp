#include "utility/text/parser.hpp"


const engine::id_t engine::parser::token_text_t::id = engine::make_id_t<'t', 'e', 'x', 't'>::value;
const engine::id_t engine::parser::token_terminal_writer_helper_t::id = engine::make_id_t<'t', 'h', 'l', 'p'>::value;
const engine::id_t engine::parser::token_escape_t::id = engine::make_id_t<'e', 's', 'c', 'p'>::value;
const engine::id_t engine::parser::token_format_number_t::id = engine::make_id_t<'f', 'r', 'm', 't'>::value;
const engine::id_t engine::parser::token_aligment_t::id = engine::make_id_t<'a', 'l', 'g', 'n'>::value;