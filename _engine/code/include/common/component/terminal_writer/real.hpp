#ifndef ENGINE_COMPONENT_TERMINAL_OUTPUT_REAL_HPP
#define ENGINE_COMPONENT_TERMINAL_OUTPUT_REAL_HPP
#pragma once

#include "component/terminal_writer.hpp"
#include "utility/messenger/messenger.hpp"
#include "utility/messenger/msg_config_updated.hpp"
#include "utility/text/parser.hpp"

namespace engine
{

	class terminal_writer_real_t : public terminal_writer_t
	{

	public:

		terminal_writer_real_t(std::shared_ptr<messenger_t> messenger, std::unique_ptr<settings_t<terminal_writer_colors_t>> terminal_writer_colors);

		~terminal_writer_real_t()
		{

		}

		void write(const terminal_writer_string_t & terminal_string) final;
		void update_window(window_state_t next_window_state) final;


	private:

		void on_config_update(msg_base_t * msg);

		mutable std::recursive_mutex window_state_mutex;
		mutable std::recursive_mutex write_mutex;
		window_state_t current_window_state;

		void write_local(const terminal_writer_string_t & terminal_string);

		std::vector<terminal_writer_string_t> buffer;

		std::unique_ptr<settings_t<terminal_writer_colors_t>> terminal_writer_colors;
		std::shared_ptr<messenger_t> messenger;

		callback_container_t callbacks_container;
	};
}

#endif