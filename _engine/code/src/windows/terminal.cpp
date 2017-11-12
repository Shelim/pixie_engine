#include "component/terminal/provider/windows_console.hpp"

#if PIXIE_WINDOWS

engine::terminal_provider_windows_console_t::instance_t::instance_t(terminal_t::terminal_color_t foreground, terminal_t::terminal_color_t background, terminal_t::closing_callback_t on_closing)
{

}
engine::terminal_provider_windows_console_t::instance_t::~instance_t()
{

}

bool engine::terminal_provider_windows_console_t::instance_t::is_closed()
{
    return false;
}

void engine::terminal_provider_windows_console_t::instance_t::write(const ustring_t & text, terminal_t::terminal_color_t foreground, terminal_t::terminal_color_t background)
{


}
void engine::terminal_provider_windows_console_t::instance_t::write_new_line()
{

}


#endif