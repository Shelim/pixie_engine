#ifndef ENGINE_COMMON_DEFAULT_TERMINAL_HPP
#define ENGINE_COMMON_DEFAULT_TERMINAL_HPP
#pragma once

#include <SDL.h>
#include <cstdio>
#include <cstdlib>
#include "core/bootstrapper.hpp"
#include "global/component/logger.hpp"
#include "global/core/vfs/filesystem.hpp"
#include "utility/text/ustring.hpp"
#include "global/core/vfs/virtual_path.hpp"
#include "global/component/terminal_writer.hpp"

namespace engine
{

	SETTINGS_START(terminal_writer_colors_t, normal)

		SETTINGS_SET(engine::platform::terminal_t::color_t, foreground_default, engine::platform::terminal_t::color_t::gray_light)
		SETTINGS_SET(engine::platform::terminal_t::color_t, background_default, engine::platform::terminal_t::color_t::black)

		SETTINGS_SET(engine::platform::terminal_t::color_t, foreground_section_header, engine::platform::terminal_t::color_t::pink_dark)
		SETTINGS_SET(engine::platform::terminal_t::color_t, background_section_header, engine::platform::terminal_t::color_t::black)

		SETTINGS_SET(engine::platform::terminal_t::color_t, foreground_section_header_text, engine::platform::terminal_t::color_t::pink_light)
		SETTINGS_SET(engine::platform::terminal_t::color_t, background_section_header_text, engine::platform::terminal_t::color_t::black)
		
		SETTINGS_SET(engine::platform::terminal_t::color_t, foreground_info, engine::platform::terminal_t::color_t::teal_light)
		SETTINGS_SET(engine::platform::terminal_t::color_t, background_info, engine::platform::terminal_t::color_t::black)
		SETTINGS_SET(engine::platform::terminal_t::color_t, foreground_info_meta, engine::platform::terminal_t::color_t::teal_dark)
		SETTINGS_SET(engine::platform::terminal_t::color_t, background_info_meta, engine::platform::terminal_t::color_t::black)

		SETTINGS_SET(engine::platform::terminal_t::color_t, foreground_note, engine::platform::terminal_t::color_t::gray_light)
		SETTINGS_SET(engine::platform::terminal_t::color_t, background_note, engine::platform::terminal_t::color_t::black)
		SETTINGS_SET(engine::platform::terminal_t::color_t, foreground_note_meta, engine::platform::terminal_t::color_t::gray_dark)
		SETTINGS_SET(engine::platform::terminal_t::color_t, background_note_meta, engine::platform::terminal_t::color_t::black)

		SETTINGS_SET(engine::platform::terminal_t::color_t, foreground_success, engine::platform::terminal_t::color_t::green_light)
		SETTINGS_SET(engine::platform::terminal_t::color_t, background_success, engine::platform::terminal_t::color_t::black)
		SETTINGS_SET(engine::platform::terminal_t::color_t, foreground_success_meta, engine::platform::terminal_t::color_t::green_dark)
		SETTINGS_SET(engine::platform::terminal_t::color_t, background_success_meta, engine::platform::terminal_t::color_t::black)

		SETTINGS_SET(engine::platform::terminal_t::color_t, foreground_pending, engine::platform::terminal_t::color_t::teal_light)
		SETTINGS_SET(engine::platform::terminal_t::color_t, background_pending, engine::platform::terminal_t::color_t::black)
		SETTINGS_SET(engine::platform::terminal_t::color_t, foreground_pending_meta, engine::platform::terminal_t::color_t::teal_dark)
		SETTINGS_SET(engine::platform::terminal_t::color_t, background_pending_meta, engine::platform::terminal_t::color_t::black)

		SETTINGS_SET(engine::platform::terminal_t::color_t, foreground_warning, engine::platform::terminal_t::color_t::yellow_light)
		SETTINGS_SET(engine::platform::terminal_t::color_t, background_warning, engine::platform::terminal_t::color_t::black)
		SETTINGS_SET(engine::platform::terminal_t::color_t, foreground_warning_meta, engine::platform::terminal_t::color_t::yellow_dark)
		SETTINGS_SET(engine::platform::terminal_t::color_t, background_warning_meta, engine::platform::terminal_t::color_t::black)

		SETTINGS_SET(engine::platform::terminal_t::color_t, foreground_error, engine::platform::terminal_t::color_t::red_light)
		SETTINGS_SET(engine::platform::terminal_t::color_t, background_error, engine::platform::terminal_t::color_t::black)
		SETTINGS_SET(engine::platform::terminal_t::color_t, foreground_error_meta, engine::platform::terminal_t::color_t::red_dark)
		SETTINGS_SET(engine::platform::terminal_t::color_t, background_error_meta, engine::platform::terminal_t::color_t::black)


	SETTINGS_END()

}

#endif