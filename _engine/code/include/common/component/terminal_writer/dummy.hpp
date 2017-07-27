#ifndef ENGINE_COMPONENT_TERMINAL_OUTPUT_DUMMY_HPP
#define ENGINE_COMPONENT_TERMINAL_OUTPUT_DUMMY_HPP
#pragma once

#include "component/terminal_writer.hpp"

namespace engine
{

	class terminal_writer_dummy_t : public terminal_writer_t
	{

	public:

		void write(const terminal_writer_string_t & terminal_string) final
		{
			// Do nothing, it is dummy.
		}
		void update_window(window_state_t next_window_state) final
		{
			// Do nothing, it is dummy.
		}

	private:

	};
}

#endif