#ifndef ENGINE_COMPONENT_TERMINAL_REAL_HPP
#define ENGINE_COMPONENT_TERMINAL_REAL_HPP
#pragma once

#include "component/terminal.hpp"
#include "utility/pattern/provider.hpp"
#include "utility/pattern/enum.hpp"
#include "utility/platform/terminal.hpp"
#include "utility/pattern/class_settings.hpp"
#include <array>

namespace engine
{
	struct terminal_colors_t
	{

	};

	SETTINGS_TABLE_START(terminal_colors_t)

		SETTINGS_TABLE_ENTRY(engine::platform::terminal_t::terminal_color_t, foreground_default)
		SETTINGS_TABLE_ENTRY(engine::platform::terminal_t::terminal_color_t, background_default)

	SETTINGS_TABLE_END()

	class terminal_real_t : public terminal_t
	{

	public:

		terminal_real_t(std::shared_ptr<platform::terminal_t> terminal, std::shared_ptr<messenger_t> messenger, std::unique_ptr<settings_t<terminal_colors_t>> terminal_colors);
		~terminal_real_t();

		void set_terminal_colors(instance_t instance, platform::terminal_t::terminal_color_t foreground, platform::terminal_t::terminal_color_t background) final;
		void output_terminal_text(instance_t instance, const ustring_t & text, platform::terminal_t::terminal_color_t foreground, platform::terminal_t::terminal_color_t background) final;
		void output_terminal_new_line(instance_t instance) final;

		void force_reopen_terminals() final;

	private:

		enum class window_state_t
		{
			open,
			close
		};

		void update_window(instance_t instance, window_state_t next_window_state);
		void on_config_update(msg_base_t * msg);

		std::shared_ptr<platform::terminal_t> terminal;
		std::unique_ptr<settings_t<terminal_colors_t>> terminal_colors;

		class terminal_meta_t
		{

		public:

			engine::platform::terminal_t::terminal_color_t get_foreground()
			{
				return foreground;
			}

			engine::platform::terminal_t::terminal_color_t get_background()
			{
				return background;
			}

			void set_colors(engine::platform::terminal_t::terminal_color_t foreground, engine::platform::terminal_t::terminal_color_t background)
			{
				this->foreground = foreground;
				this->background = background;
			}

		private:

			engine::platform::terminal_t::terminal_color_t foreground;
			engine::platform::terminal_t::terminal_color_t background;

		};

		std::array<std::shared_ptr<platform::terminal_t::instance_t>, value_of(instance_t::count)> terminals;
		std::array<terminal_meta_t, value_of(instance_t::count)> terminals_metadata;

		callback_container_t callbacks_container;
	};
}


#endif