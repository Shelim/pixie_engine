#ifndef ENGINE_COMPONENT_TERMINAL_OUTPUT_REAL_HPP
#define ENGINE_COMPONENT_TERMINAL_OUTPUT_REAL_HPP
#pragma once

#include "component/terminal_output.hpp"

namespace engine
{

	class terminal_output_real_t : public terminal_output_t
	{

	public:

		terminal_output_real_t(PTR_TO_SETTINGS_FOR(terminal_output_colors_t) terminal_output_colors) : current_window_state(window_state_t::close), terminal_output_colors(std::move(terminal_output_colors))
		{

		}

		void write(const richtext_t & richtext) final;
		void update_window(window_state_t next_window_state) final;

	private:

		mutable std::recursive_mutex window_state_mutex;
		mutable std::recursive_mutex write_mutex;
		window_state_t current_window_state;

		void write_local(const richtext_t & richtext);

		std::vector<richtext_t> buffer;

		PTR_TO_SETTINGS_FOR(terminal_output_colors_t) terminal_output_colors;

	};
}

#endif