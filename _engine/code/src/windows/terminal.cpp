#include "component/terminal/provider/windows_console.hpp"

#if PIXIE_WINDOWS

engine::terminal_provider_windows_console_t::instance_t::instance_t(const ustring_t & name, terminal_t::color_t background, terminal_t::closing_callback_t on_closing)
    : on_closing(on_closing), background(background)
{
    console.Create(name.get_cstring());
    console.cls(color_to_background(background));
}

engine::terminal_provider_windows_console_t::instance_t::~instance_t()
{
    close();
}

bool engine::terminal_provider_windows_console_t::instance_t::is_closed()
{
    std::lock_guard<std::recursive_mutex> guard(recursive_mutex);
    return flags.is_flag(flag_t::is_closed);
}

void engine::terminal_provider_windows_console_t::instance_t::write(const ustring_t & text, terminal_t::color_t foreground)
{
    write(text, foreground, this->background);
}

void engine::terminal_provider_windows_console_t::instance_t::write(const ustring_t & text, terminal_t::color_t foreground, terminal_t::color_t background)
{
    std::lock_guard<std::recursive_mutex> guard(recursive_mutex);
    if(console.cprintf(color_to_foreground(foreground) | color_to_background(background), "%s", text.get_cstring()) < 0)
        close();
}
void engine::terminal_provider_windows_console_t::instance_t::write_new_line()
{
    std::lock_guard<std::recursive_mutex> guard(recursive_mutex);
    if(console.print("\n") < 0)
        close();
}

void engine::terminal_provider_windows_console_t::instance_t::close()
{
    std::lock_guard<std::recursive_mutex> guard(recursive_mutex);
    if(!flags.is_flag(flag_t::is_closed))
    {
        flags.set_flag(flag_t::is_closed, true);
        on_closing(this);
        console.Close();
    }
}

DWORD engine::terminal_provider_windows_console_t::instance_t::color_to_foreground(terminal_t::color_t color)
{
    switch(color)
    {
        case terminal_t::color_t::black : return CConsoleLoggerEx::COLOR_BLACK;
        case terminal_t::color_t::blue_dark : return CConsoleLoggerEx::COLOR_BLUE;
        case terminal_t::color_t::blue_light : return CConsoleLoggerEx::COLOR_BLUE | CConsoleLoggerEx::COLOR_INTENSITY;
        case terminal_t::color_t::gray_dark : return CConsoleLoggerEx::COLOR_BLACK | CConsoleLoggerEx::COLOR_INTENSITY;
        case terminal_t::color_t::gray_light : return CConsoleLoggerEx::COLOR_WHITE;
        case terminal_t::color_t::green_dark : return CConsoleLoggerEx::COLOR_GREEN;
        case terminal_t::color_t::green_light : return CConsoleLoggerEx::COLOR_GREEN | CConsoleLoggerEx::COLOR_INTENSITY;
        case terminal_t::color_t::pink_dark : return CConsoleLoggerEx::COLOR_RED | CConsoleLoggerEx::COLOR_BLUE;
        case terminal_t::color_t::pink_light : return CConsoleLoggerEx::COLOR_RED | CConsoleLoggerEx::COLOR_BLUE | CConsoleLoggerEx::COLOR_INTENSITY;
        case terminal_t::color_t::red_dark : return CConsoleLoggerEx::COLOR_RED;
        case terminal_t::color_t::red_light : return CConsoleLoggerEx::COLOR_RED | CConsoleLoggerEx::COLOR_INTENSITY;
        case terminal_t::color_t::teal_dark : return CConsoleLoggerEx::COLOR_GREEN | CConsoleLoggerEx::COLOR_BLUE;
        case terminal_t::color_t::teal_light : return CConsoleLoggerEx::COLOR_GREEN | CConsoleLoggerEx::COLOR_BLUE | CConsoleLoggerEx::COLOR_INTENSITY;
        case terminal_t::color_t::white : return CConsoleLoggerEx::COLOR_WHITE | CConsoleLoggerEx::COLOR_INTENSITY;
        case terminal_t::color_t::yellow_dark : return CConsoleLoggerEx::COLOR_RED | CConsoleLoggerEx::COLOR_GREEN;
        case terminal_t::color_t::yellow_light : return CConsoleLoggerEx::COLOR_RED | CConsoleLoggerEx::COLOR_GREEN | CConsoleLoggerEx::COLOR_INTENSITY;
    }
    return 0;
}

DWORD engine::terminal_provider_windows_console_t::instance_t::color_to_background(terminal_t::color_t color)
{
    switch(color)
    {
        case terminal_t::color_t::black : return CConsoleLoggerEx::COLOR_BACKGROUND_BLACK;
        case terminal_t::color_t::blue_dark : return CConsoleLoggerEx::COLOR_BACKGROUND_BLUE;
        case terminal_t::color_t::blue_light : return CConsoleLoggerEx::COLOR_BACKGROUND_BLUE | CConsoleLoggerEx::COLOR_BACKGROUND_INTENSITY;
        case terminal_t::color_t::gray_dark : return CConsoleLoggerEx::COLOR_BACKGROUND_BLACK | CConsoleLoggerEx::COLOR_BACKGROUND_INTENSITY;
        case terminal_t::color_t::gray_light : return CConsoleLoggerEx::COLOR_BACKGROUND_WHITE;
        case terminal_t::color_t::green_dark : return CConsoleLoggerEx::COLOR_BACKGROUND_GREEN;
        case terminal_t::color_t::green_light : return CConsoleLoggerEx::COLOR_BACKGROUND_GREEN | CConsoleLoggerEx::COLOR_BACKGROUND_INTENSITY;
        case terminal_t::color_t::pink_dark : return CConsoleLoggerEx::COLOR_BACKGROUND_RED | CConsoleLoggerEx::COLOR_BACKGROUND_BLUE;
        case terminal_t::color_t::pink_light : return CConsoleLoggerEx::COLOR_BACKGROUND_RED | CConsoleLoggerEx::COLOR_BACKGROUND_BLUE | CConsoleLoggerEx::COLOR_BACKGROUND_INTENSITY;
        case terminal_t::color_t::red_dark : return CConsoleLoggerEx::COLOR_BACKGROUND_RED;
        case terminal_t::color_t::red_light : return CConsoleLoggerEx::COLOR_BACKGROUND_RED | CConsoleLoggerEx::COLOR_BACKGROUND_INTENSITY;
        case terminal_t::color_t::teal_dark : return CConsoleLoggerEx::COLOR_BACKGROUND_GREEN | CConsoleLoggerEx::COLOR_BACKGROUND_BLUE;
        case terminal_t::color_t::teal_light : return CConsoleLoggerEx::COLOR_BACKGROUND_GREEN | CConsoleLoggerEx::COLOR_BACKGROUND_BLUE | CConsoleLoggerEx::COLOR_BACKGROUND_INTENSITY;
        case terminal_t::color_t::white : return CConsoleLoggerEx::COLOR_BACKGROUND_WHITE | CConsoleLoggerEx::COLOR_BACKGROUND_INTENSITY;
        case terminal_t::color_t::yellow_dark : return CConsoleLoggerEx::COLOR_BACKGROUND_RED | CConsoleLoggerEx::COLOR_BACKGROUND_GREEN;
        case terminal_t::color_t::yellow_light : return CConsoleLoggerEx::COLOR_BACKGROUND_RED | CConsoleLoggerEx::COLOR_BACKGROUND_GREEN | CConsoleLoggerEx::COLOR_BACKGROUND_INTENSITY;
    }
    return 0;
}


#endif