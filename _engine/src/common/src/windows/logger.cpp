#include "common/platform.hpp"

#if PIXIE_WINDOWS

#include "common/logger_container.hpp"
#include "common/environment_info.hpp"
#include "platform_pimpl.hpp"

#include <clocale>
#include <string>
#include <stack>

#include <SDL.h>
#include <SDL_syswm.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shlwapi.h>
#include <shlobj.h>
#include <shellapi.h>
#include <dbghelp.h>

#include <cstdio>
#include <cstdlib>
#include <io.h>
#include <fcntl.h>

void engine::logger_output::provider_stdout_t::platform_color_output(const richtext_t::part_t & part)
{

#define GAME_RICHTEXT_TAG_STD(key) if(part.is_flag(richtext_t::flag_t::key)) platform->get_pimpl()->set_console_colors_for_print(static_cast<engine::platform_t::implementation_t::console_color_t>(engine::platform_t::implementation_t::console_default_color_t::foreground_##key), static_cast<engine::platform_t::implementation_t::console_color_t>(engine::platform_t::implementation_t::console_default_color_t::background_##key));
#include "common/std/richtext_std.hpp"

}

#include "debugbreak.h"

void engine::logger_t::platform_fatal()
{
	ustring_t raported;
	ustring_t clipboard;
	ustring_t buffer;

#if PIXIE_IS_DEBUG_BUILD
	if (IsDebuggerPresent())
		debug_break();
#endif

	for (auto & item : platform->get_pimpl()->get_logger()->items)
	{
		if(item.is_flag(engine::logger_t::item_t::flag_t::raport))
		{
			raported.append_utf8(u8"\n\n");

			raported.append(item.get_message());
			switch (item.get_level())
			{
			case engine::logger_t::item_t::level_t::task_started: raported.append_utf8(u8"..."); break;
			case engine::logger_t::item_t::level_t::task_done: raported.append_utf8(u8"... Done!"); break;
			case engine::logger_t::item_t::level_t::task_failed: raported.append_utf8(u8"... FAILED!"); break;
			}
		}
	}

	clipboard = engine::format_utf8(u8"There is a critical error, and the game cannot continue:\n~~~#1#", raported);

	buffer = clipboard;
	buffer.append_utf8(u8"\n\n___________________\n(This info is now in your clipboard, so you can paste it anywhere)");

	std::wstring bufferW = buffer.to_wide();

	platform->copy_to_clipboard(clipboard);
	MessageBoxW(0, bufferW.c_str(), 0, MB_OK | MB_ICONSTOP);

	exit(1);
}


#endif