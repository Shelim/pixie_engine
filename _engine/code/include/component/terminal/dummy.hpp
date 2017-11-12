#ifndef ENGINE_COMPONENT_TERMINAL_DUMMY_HPP
#define ENGINE_COMPONENT_TERMINAL_DUMMY_HPP
#pragma once

#include "component/terminal.hpp"


namespace engine
{

	class terminal_dummy_t : public terminal_t
	{

	public:

		void set_terminal_colors(instance_t instance, platform::terminal_t::terminal_color_t foreground, platform::terminal_t::terminal_color_t background) final
		{

		}
		void output_terminal_text(instance_t instance, const ustring_t & text, platform::terminal_t::terminal_color_t foreground, platform::terminal_t::terminal_color_t background) final
		{

		}
		void output_terminal_new_line(instance_t instance) final
		{

		}

		void force_reopen_terminals() final
		{

		}

	};
}


#endif