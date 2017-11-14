#include "core/console/msg/logger.hpp"
#include "core/console/msg/meta.hpp"
#include "core/console/filter.hpp"
#include "core/console/formatter.hpp"
#include "core/console/instance.hpp"
#include "core/console/string.hpp"
#include "core/console/writer.hpp"

engine::console::logger_item_t::logger_item_t(std::size_t id, kind_t kind, source_t source, const ustring_t & message, const ustring_t & function, const ustring_t & file, uint32_t line, uint64_t frame, std::chrono::seconds time, std::thread::id thread, std::size_t link) :
				actual_t(type_t::logger, source, kind), id(id), message(message), function(function), file_raw(file), line(line), frame(frame), time(time), thread(thread), link(link)
{
    this->file = platform::canonize_debug_filename(file_raw);
}
std::size_t engine::console::logger_item_t::get_id() const
{
    return id;
}
engine::ustring_t engine::console::logger_item_t::get_source_as_string() const
{
    return to_string(get_source());
}
const engine::ustring_t & engine::console::logger_item_t::get_message() const
{
    return message;
}
const engine::ustring_t & engine::console::logger_item_t::get_function() const
{
    return function;
}
const engine::ustring_t & engine::console::logger_item_t::get_file() const
{
    return file;
}
const engine::ustring_t & engine::console::logger_item_t::get_file_raw() const
{
    return file_raw;
}
uint32_t engine::console::logger_item_t::get_line() const
{
    return line;
}
uint64_t engine::console::logger_item_t::get_frame() const
{
    return frame;
}
std::chrono::seconds engine::console::logger_item_t::get_time() const
{
    return time;
}
std::thread::id engine::console::logger_item_t::get_thread() const
{
    return thread;
}

std::size_t engine::console::logger_item_t::get_link() const
{
    return link;
}

bool engine::console::logger_item_t::is_linked() const
{
    return link != -1;
}

engine::console::meta_item_t::meta_item_t(meta_type_t meta_type, type_t type) :
                actual_t(type_t::meta, source_t::core, kind_t::meta), meta_type(meta_type), type(type)
{

}

engine::console::meta_item_t::meta_type_t engine::console::meta_item_t::get_meta_type() const
{
    return meta_type;
}

messenger::msg_console_t::actual_t::type_t engine::console::meta_item_t::get_meta_type_type() const
{
    return type;
}

engine::parser::token_console_escape_t::token_console_escape_t() : token_base_t(id)
{

}

std::unique_ptr<engine::parser::token_base_t> engine::parser::token_console_escape_t::create(stream_t & stream)
{
    if (stream.peek() == '$')
    {
        stream.advance();
        if (stream.peek() == '$')
        {
            stream.advance();
            return std::make_unique<token_console_escape_t>();
        }
    }

    return nullptr;
}

std::unique_ptr<engine::parser::token_base_t> engine::parser::token_console_escape_t::clone() const
{
    return std::make_unique<token_console_escape_t>(*this);
}


engine::parser::token_console_eof_t::token_console_eof_t() : token_base_t(id)
{

}

std::unique_ptr<engine::parser::token_base_t> engine::parser::token_console_eof_t::clone() const
{
    return std::make_unique<token_console_eof_t>(*this);
}

std::unique_ptr<engine::parser::token_base_t> engine::parser::token_console_eof_t::create(stream_t & stream)
{
    if (stream.is_eof())
    {
        return std::make_unique<token_console_eof_t>();
    }

    return nullptr;
}

engine::parser::token_console_format_t::token_console_format_t(console_tag_t console_tag) : token_base_t(id), console_tag(console_tag)
{

}

engine::parser::token_console_format_t::console_tag_t engine::parser::token_console_format_t::get_console_tag() const
{
    return console_tag;
}

std::unique_ptr<engine::parser::token_base_t> engine::parser::token_console_format_t::create(stream_t & stream)
{
    if (stream.peek() == '$')
    {
        ustring_t key;

        stream.advance();

        while (stream.peek() != '$' && !stream.is_eof())
        {
            key.append(stream.peek());
            stream.advance();
        }

        if (!stream.is_eof())
        {
            stream.advance();

            if (key == "0"_u) return std::make_unique<token_console_format_t>(console_tag_t::def);
        
            console_tag_t tag = from_string<console_tag_t>(key);
            if(tag != console_tag_t::count)
                return std::make_unique<token_console_format_t>(tag);

        }
    }

    return nullptr;
}

std::unique_ptr<engine::parser::token_base_t> engine::parser::token_console_format_t::clone() const
{
    return std::make_unique<token_console_format_t>(*this);
}

engine::parser::resolver_console_t::resolver_console_t(std::function<void(const ustring_t &, console_tag_t)> output_text) : output_text(output_text)
{
    output_tag = engine::console_tag_t::def;
}

void engine::parser::resolver_console_t::resolve(const token_base_t * token, resolver_output_t * output)
{
    if (token->get_id() == token_console_escape_t::id)
    {
        output->append('$');
    }
    else if (token->get_id() == token_console_format_t::id)
    {
        engine::console_tag_t new_tag = static_cast<const token_console_format_t*>(token)->get_console_tag();

        if (new_tag != output_tag && output->non_empty_since_last_truncate())
            output_text(output->get_result(), output_tag);

        output->truncate_result();

        output_tag = new_tag;
    }
    else if (token->get_id() == token_console_eof_t::id)
    {
        if (output->non_empty_since_last_truncate())
            output_text(output->get_result(), output_tag);
    }
}

engine::console_writer_t::console_writer_t(std::shared_ptr<messenger_console_t> console) : console(console)
{

}

void engine::console_writer_t::write_local(std::unique_ptr<messenger::msg_console_t::actual_t> msg) final
{
    console->write(std::make_shared<messenger::msg_console_t>(std::move(msg)));
}