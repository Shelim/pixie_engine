#include "common/platform.hpp"

#if PIXIE_WINDOWS

#include "common/logger_container.hpp"
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

void engine::logger_output::provider_console_t::platform_open_console()
{
	AllocConsole();

#ifdef _MSC_VER
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);
#else
	HANDLE handle_out = GetStdHandle(STD_OUTPUT_HANDLE);
	int hCrt = _open_osfhandle((long)handle_out, _O_TEXT);
	FILE* hf_out = _fdopen(hCrt, "w");
	setvbuf(hf_out, NULL, _IONBF, 1);
	*stdout = *hf_out;

	HANDLE handle_in = GetStdHandle(STD_INPUT_HANDLE);
	hCrt = _open_osfhandle((long)handle_in, _O_TEXT);
	FILE* hf_in = _fdopen(hCrt, "r");
	setvbuf(hf_in, NULL, _IONBF, 128);
	*stdin = *hf_in;
#endif

	platform->get_pimpl()->set_console_colors(FOREGROUND_GREEN | FOREGROUND_INTENSITY);

	SetConsoleOutputCP(CP_UTF8);

	HWND hwnd = GetConsoleWindow();
	HMENU hmenu = GetSystemMenu(hwnd, FALSE);
	EnableMenuItem(hmenu, SC_CLOSE, MF_GRAYED);

	ustring_t game_name = ustring_t::from_utf8(manifest->game_name());

	ustring_t app_data_str = _U(PIXIE_OUTPUT_TYPE_STR);
	ustring_t app_data_canonized = app_data_str.substr(0, 1).to_upper();
	app_data_canonized.append(app_data_str.substr(1));

	game_name.append(_U(" ("));
	game_name.append(app_data_canonized);
	game_name.append(_U(")"));

	std::wstring game_name_w = game_name.to_wide();
	SetConsoleTitleW(game_name_w.c_str());

	std::size_t size = get_logger()->get_items().size();
	for(std::size_t i = 0 ; i < size; i++)
	{
		platform_output(i);
	}
}

void engine::logger_output::provider_console_t::platform_close_console()
{
	FreeConsole();
}

void engine::logger_output::provider_console_t::platform_output(const std::size_t item_changed)
{
	auto & items = get_logger()->get_items();

	if (item_changed <= items.size())
	{

		const engine::logger_t::item_t * item = &items[item_changed];
		switch (item->get_level())
		{
		case engine::logger_t::item_t::level_t::task: platform->get_pimpl()->set_console_colors_for_print(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY); printf(" @ "); break;
		case engine::logger_t::item_t::level_t::task_done: platform->get_pimpl()->set_console_colors_for_print(FOREGROUND_GREEN | FOREGROUND_INTENSITY); printf(" v "); break;
		case engine::logger_t::item_t::level_t::task_failed: platform->get_pimpl()->set_console_colors_for_print(FOREGROUND_RED | FOREGROUND_INTENSITY); printf("!! "); break;
		case engine::logger_t::item_t::level_t::message: platform->get_pimpl()->set_console_colors_for_print(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY); printf(" - "); break;
		case engine::logger_t::item_t::level_t::warning: platform->get_pimpl()->set_console_colors_for_print(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY); printf(" ! "); break;
		case engine::logger_t::item_t::level_t::error: platform->get_pimpl()->set_console_colors_for_print(FOREGROUND_RED | FOREGROUND_INTENSITY); printf("!! "); break;
		}

		std::string msg = item->get_message().to_utf8();
		printf(msg.c_str());

		switch (item->get_level())
		{
		case engine::logger_t::item_t::level_t::task: platform->get_pimpl()->set_console_colors_for_print(FOREGROUND_GREEN | FOREGROUND_BLUE); printf("..."); break;
		case engine::logger_t::item_t::level_t::task_done: platform->get_pimpl()->set_console_colors_for_print(FOREGROUND_GREEN); printf("..."); platform->get_pimpl()->set_console_colors_for_print(FOREGROUND_GREEN | FOREGROUND_INTENSITY); printf(" Done!"); platform->get_pimpl()->set_console_colors_for_print(FOREGROUND_GREEN); break;
		case engine::logger_t::item_t::level_t::task_failed: platform->get_pimpl()->set_console_colors_for_print(FOREGROUND_RED); printf("..."); platform->get_pimpl()->set_console_colors_for_print(FOREGROUND_RED | FOREGROUND_INTENSITY); printf(" FAILED!"); platform->get_pimpl()->set_console_colors_for_print(FOREGROUND_RED); break;
		case engine::logger_t::item_t::level_t::message: platform->get_pimpl()->set_console_colors_for_print(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); break;
		case engine::logger_t::item_t::level_t::warning: platform->get_pimpl()->set_console_colors_for_print(FOREGROUND_RED | FOREGROUND_GREEN); break;
		case engine::logger_t::item_t::level_t::error: platform->get_pimpl()->set_console_colors_for_print(FOREGROUND_RED); break;
		}

		std::string func = item->get_function().to_utf8();
		printf(" <- %s(), line %lu\n", func.c_str(), item->get_line());
	}
}

#include "debugbreak.h"

void engine::logger_t::platform_fatal()
{
	ustring_t raported;
	ustring_t clipboard;
	ustring_t buffer;

#ifdef _DEBUG
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
			case engine::logger_t::item_t::level_t::task: raported.append_utf8(u8"... INTERRUPTED!"); break;
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

	auto output_providers_shard = output_providers.lock();
	if(output_providers_shard)
		output_providers_shard->force_resave();

	exit(1);
}


#endif