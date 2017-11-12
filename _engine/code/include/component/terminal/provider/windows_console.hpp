#ifndef ENGINE_COMPONENT_TERMINAL_PROVIDER_WINDOWS_CONSOLE_HPP
#define ENGINE_COMPONENT_TERMINAL_PROVIDER_WINDOWS_CONSOLE_HPP
#pragma once

#if PIXIE_WINDOWS

#include "component/terminal/real.hpp"
#include "utility/pattern/flags.hpp"
#include <multiple_consoles.h>

namespace engine
{
    class terminal_provider_windows_console_t : public terminal_provider_base_t
	{

	public:

        std::shared_ptr<terminal_t::instance_t> open(const ustring_t & name, terminal_t::color_t background, terminal_t::closing_callback_t on_closing = [](terminal_t::instance_t*){}) final
        {
            return std::make_shared<instance_t>(name, background, on_closing);
        }

    private:

        class instance_t : public terminal_t::instance_t
		{
			
		public:

            instance_t(const ustring_t & name, terminal_t::color_t background, terminal_t::closing_callback_t on_closing = [](terminal_t::instance_t*){});
            ~instance_t();

			bool is_closed() final;
			void write(const ustring_t & text, terminal_t::color_t foreground) final;
			void write(const ustring_t & text, terminal_t::color_t foreground, terminal_t::color_t background) final;
			void write_new_line() final;

            enum class flag_t
            {
                is_closed,
                count
            };

		private:

            CConsoleLoggerEx console;

            void close();

            DWORD color_to_foreground(terminal_t::color_t color);
            DWORD color_to_background(terminal_t::color_t color);

            std::recursive_mutex recursive_mutex;
            terminal_t::color_t background;
            terminal_t::closing_callback_t on_closing;
            flags_t<flag_t> flags;

		};
    };


}

#endif
#endif