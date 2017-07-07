#ifndef ENGINE_COMPONENT_CONSOLE_OUTPUT_HPP
#define ENGINE_COMPONENT_CONSOLE_OUTPUT_HPP
#pragma once

#include "utility/text/richtext.hpp"
#include "utility/vfs/filesystem.hpp"
#include "utility/pattern/class_settings.hpp"
#include "utility/vfs/virtual_path.hpp"
#include "platform/terminal.hpp"
#include <mutex>

namespace engine
{
	struct terminal_output_colors_t
	{

	};

	SETTINGS_TABLE_START(terminal_output_colors_t)

		SETTINGS_TABLE_ENTRY(engine::platform::terminal_color_t, foreground_default, engine::platform::terminal_color_t::gray_light)
		SETTINGS_TABLE_ENTRY(engine::platform::terminal_color_t, background_default, engine::platform::terminal_color_t::black)

		SETTINGS_TABLE_ENTRY(engine::platform::terminal_color_t, foreground_section_header, engine::platform::terminal_color_t::pink_dark)
		SETTINGS_TABLE_ENTRY(engine::platform::terminal_color_t, background_section_header, engine::platform::terminal_color_t::black)

		SETTINGS_TABLE_ENTRY(engine::platform::terminal_color_t, foreground_section_header_text, engine::platform::terminal_color_t::pink_light)
		SETTINGS_TABLE_ENTRY(engine::platform::terminal_color_t, background_section_header_text, engine::platform::terminal_color_t::black)
		
		SETTINGS_TABLE_ENTRY(engine::platform::terminal_color_t, foreground_info, engine::platform::terminal_color_t::teal_light)
		SETTINGS_TABLE_ENTRY(engine::platform::terminal_color_t, background_info, engine::platform::terminal_color_t::black)
		SETTINGS_TABLE_ENTRY(engine::platform::terminal_color_t, foreground_info_meta, engine::platform::terminal_color_t::teal_dark)
		SETTINGS_TABLE_ENTRY(engine::platform::terminal_color_t, background_info_meta, engine::platform::terminal_color_t::black)

		SETTINGS_TABLE_ENTRY(engine::platform::terminal_color_t, foreground_note, engine::platform::terminal_color_t::gray_light)
		SETTINGS_TABLE_ENTRY(engine::platform::terminal_color_t, background_note, engine::platform::terminal_color_t::black)
		SETTINGS_TABLE_ENTRY(engine::platform::terminal_color_t, foreground_note_meta, engine::platform::terminal_color_t::gray_dark)
		SETTINGS_TABLE_ENTRY(engine::platform::terminal_color_t, background_note_meta, engine::platform::terminal_color_t::black)

		SETTINGS_TABLE_ENTRY(engine::platform::terminal_color_t, foreground_success, engine::platform::terminal_color_t::green_light)
		SETTINGS_TABLE_ENTRY(engine::platform::terminal_color_t, background_success, engine::platform::terminal_color_t::black)
		SETTINGS_TABLE_ENTRY(engine::platform::terminal_color_t, foreground_success_meta, engine::platform::terminal_color_t::green_dark)
		SETTINGS_TABLE_ENTRY(engine::platform::terminal_color_t, background_success_meta, engine::platform::terminal_color_t::black)

		SETTINGS_TABLE_ENTRY(engine::platform::terminal_color_t, foreground_pending, engine::platform::terminal_color_t::teal_light)
		SETTINGS_TABLE_ENTRY(engine::platform::terminal_color_t, background_pending, engine::platform::terminal_color_t::black)
		SETTINGS_TABLE_ENTRY(engine::platform::terminal_color_t, foreground_pending_meta, engine::platform::terminal_color_t::teal_dark)
		SETTINGS_TABLE_ENTRY(engine::platform::terminal_color_t, background_pending_meta, engine::platform::terminal_color_t::black)

		SETTINGS_TABLE_ENTRY(engine::platform::terminal_color_t, foreground_warning, engine::platform::terminal_color_t::yellow_light)
		SETTINGS_TABLE_ENTRY(engine::platform::terminal_color_t, background_warning, engine::platform::terminal_color_t::black)
		SETTINGS_TABLE_ENTRY(engine::platform::terminal_color_t, foreground_warning_meta, engine::platform::terminal_color_t::yellow_dark)
		SETTINGS_TABLE_ENTRY(engine::platform::terminal_color_t, background_warning_meta, engine::platform::terminal_color_t::black)

		SETTINGS_TABLE_ENTRY(engine::platform::terminal_color_t, foreground_error, engine::platform::terminal_color_t::red_light)
		SETTINGS_TABLE_ENTRY(engine::platform::terminal_color_t, background_error, engine::platform::terminal_color_t::black)
		SETTINGS_TABLE_ENTRY(engine::platform::terminal_color_t, foreground_error_meta, engine::platform::terminal_color_t::red_dark)
		SETTINGS_TABLE_ENTRY(engine::platform::terminal_color_t, background_error_meta, engine::platform::terminal_color_t::black)

	SETTINGS_TABLE_END()

	class terminal_output_t
	{

	public:

		terminal_output_t()
		{

		}

		virtual ~terminal_output_t()
		{

		}

		enum class window_state_t
		{
			open,
			close
		};

		virtual void write(const richtext_t & richtext) = 0;
		virtual void update_window(window_state_t next_window_state) = 0;

	private:

	};
}
#include "component/terminal_output/dummy.hpp"
#include "component/terminal_output/real.hpp"


#endif