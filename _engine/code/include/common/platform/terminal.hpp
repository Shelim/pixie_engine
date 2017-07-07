#ifndef ENGINE_COMMON_PLATFORM_TERMINAL_HPP
#define ENGINE_COMMON_PLATFORM_TERMINAL_HPP

#include "utility/text/ustring.hpp"

namespace engine
{

	namespace platform
	{

		enum terminal_color_t
		{
			red_dark,
			red_light,

			green_dark,
			green_light,

			blue_dark,
			blue_light,

			gray_dark,
			gray_light,

			white,
			black,

			yellow_dark,
			yellow_light,

			pink_dark,
			pink_light,

			teal_dark,
			teal_light
		};

		void open_terminal(terminal_color_t foreground, terminal_color_t background);

		void set_terminal_colors(terminal_color_t foreground, terminal_color_t background);
		void output_terminal_text(const ustring_t & text, terminal_color_t foreground, terminal_color_t background);
		void output_terminal_new_line();
		
		void close_terminal();

	}

}

#endif