#include "global/core/console/msg/logger.hpp"
#include "global/core/console/msg/meta.hpp"
#include "global/core/console/filter.hpp"
#include "global/core/console/formatter.hpp"
#include "global/core/console/instance.hpp"
#include "global/core/console/string.hpp"
#include "global/core/console/writer.hpp"

engine::console::logger_item_t::logger_item_t(std::size_t id, kind_t kind, app_t::kind_t app, app_t::instance_id_t app_instance_id, source_t source, const ustring_t & message, const ustring_t & function, const ustring_t & file, uint32_t line, std::chrono::seconds time, std::thread::id thread, std::size_t link) :
				actual_t(type_t::logger, source, kind), id(id), app(app), app_instance_id(app_instance_id), message(message), function(function), file_raw(file), line(line), time(time), thread(thread), link(link)
{
    this->file = platform::canonize_debug_filename(file_raw);
}
std::size_t engine::console::logger_item_t::get_id() const
{
    return id;
}
engine::app_t::kind_t engine::console::logger_item_t::get_app() const
{
    return app;
}
engine::app_t::instance_id_t engine::console::logger_item_t::get_app_instance_id() const
{
    return app_instance_id;
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

engine::messenger::msg_console_t::actual_t::type_t engine::console::meta_item_t::get_meta_type_type() const
{
    return type;
}

engine::console_writer_t::console_writer_t(std::shared_ptr<messenger_console_t> console) : console(console)
{

}

void engine::console_writer_t::write_local(std::unique_ptr<messenger::msg_console_t::actual_t> msg)
{
    console->write(std::make_shared<messenger::msg_console_t>(std::move(msg)));
}