#ifndef ENGINE_COMPONENT_CONSOLE_OUTPUT_HPP
#define ENGINE_COMPONENT_CONSOLE_OUTPUT_HPP
#pragma once

#include "component/terminal_output/string.hpp"
#include "utility/vfs/filesystem.hpp"
#include "utility/pattern/class_settings.hpp"
#include "utility/vfs/virtual_path.hpp"
#include "platform/terminal.hpp""
#include <mutex>

namespace engine
{
	struct terminal_output_colors_t
	{

	};

	SETTINGS_TABLE_START(terminal_output_colors_t)

		SETTINGS_TABLE_ENTRY(engine::platform::terminal_color_t, foreground_default)
		SETTINGS_TABLE_ENTRY(engine::platform::terminal_color_t, background_default)

#define ENGINE_TERMINAL_OUTPUT_TAG_STD(tag) SETTINGS_TABLE_ENTRY(engine::platform::terminal_color_t, foreground_##tag) SETTINGS_TABLE_ENTRY(engine::platform::terminal_color_t, background_##tag)
#include "std/terminal_output_std.hpp"

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

		virtual void write(const terminal_output_string_t & terminal_string) = 0;
		virtual void update_window(window_state_t next_window_state) = 0;

	private:

	};
}
#include "component/terminal_output/dummy.hpp"
#include "component/terminal_output/real.hpp"


#endif