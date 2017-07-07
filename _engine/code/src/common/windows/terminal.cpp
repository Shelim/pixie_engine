#include "platform/terminal.hpp"

#if PIXIE_WINDOWS

#include "utility/text/ustring.hpp"
#include "manifest_app.hpp"

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

namespace
{
	BOOL WINAPI console_signal_handler(DWORD signal)
	{
		if (signal == CTRL_C_EVENT || signal == CTRL_BREAK_EVENT)
		{
			//		engine::engine_t::get()->shutdown(); // ToDo!
			exit(EXIT_FAILURE);
			return TRUE;
		}

		return FALSE;
	}


	WORD foreground_to_attribs(engine::platform::terminal_color_t color)
	{
		switch (color)
		{
		case engine::platform::terminal_color_t::red_dark: return FOREGROUND_RED;
		case engine::platform::terminal_color_t::red_light: return FOREGROUND_RED | FOREGROUND_INTENSITY;
		case engine::platform::terminal_color_t::green_dark: return FOREGROUND_GREEN;
		case engine::platform::terminal_color_t::green_light: return FOREGROUND_GREEN | FOREGROUND_INTENSITY;
		case engine::platform::terminal_color_t::blue_dark: return FOREGROUND_BLUE;
		case engine::platform::terminal_color_t::blue_light: return FOREGROUND_BLUE | FOREGROUND_INTENSITY;
		case engine::platform::terminal_color_t::gray_dark: return FOREGROUND_INTENSITY;
		case engine::platform::terminal_color_t::gray_light: return FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
		case engine::platform::terminal_color_t::white: return FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
		case engine::platform::terminal_color_t::black: return 0;
		case engine::platform::terminal_color_t::yellow_dark: return FOREGROUND_RED | FOREGROUND_GREEN;
		case engine::platform::terminal_color_t::yellow_light: return FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
		case engine::platform::terminal_color_t::pink_dark: return FOREGROUND_RED | FOREGROUND_BLUE;
		case engine::platform::terminal_color_t::pink_light: return FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
		case engine::platform::terminal_color_t::teal_dark: return FOREGROUND_GREEN | FOREGROUND_BLUE;
		case engine::platform::terminal_color_t::teal_light: return FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
		}
		return (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	}

	WORD background_to_attribs(engine::platform::terminal_color_t background)
	{
		switch (background)
		{
		case engine::platform::terminal_color_t::red_dark: return BACKGROUND_RED;
		case engine::platform::terminal_color_t::red_light: return BACKGROUND_RED | BACKGROUND_INTENSITY;
		case engine::platform::terminal_color_t::green_dark: return BACKGROUND_GREEN;
		case engine::platform::terminal_color_t::green_light: return BACKGROUND_GREEN | BACKGROUND_INTENSITY;
		case engine::platform::terminal_color_t::blue_dark: return BACKGROUND_BLUE;
		case engine::platform::terminal_color_t::blue_light: return BACKGROUND_BLUE | BACKGROUND_INTENSITY;
		case engine::platform::terminal_color_t::gray_dark: return BACKGROUND_INTENSITY;
		case engine::platform::terminal_color_t::gray_light: return BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE;
		case engine::platform::terminal_color_t::white: return BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY;
		case engine::platform::terminal_color_t::black: return 0;
		case engine::platform::terminal_color_t::yellow_dark: return BACKGROUND_RED | BACKGROUND_GREEN;
		case engine::platform::terminal_color_t::yellow_light: return BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY;
		case engine::platform::terminal_color_t::pink_dark: return BACKGROUND_RED | BACKGROUND_BLUE;
		case engine::platform::terminal_color_t::pink_light: return BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_INTENSITY;
		case engine::platform::terminal_color_t::teal_dark: return BACKGROUND_GREEN | BACKGROUND_BLUE;
		case engine::platform::terminal_color_t::teal_light: return BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY;
		}
		return 0;
	}

	WORD color_pair_to_attribs(engine::platform::terminal_color_t foreground, engine::platform::terminal_color_t background)
	{
		return foreground_to_attribs(foreground) | background_to_attribs(background);
	}

	void set_console_colors_for_print(engine::platform::terminal_color_t foreground, engine::platform::terminal_color_t background)
	{
		HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hOutput, color_pair_to_attribs(foreground, background));
	}
}

void engine::platform::open_terminal(terminal_color_t foreground, terminal_color_t background)
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

	set_terminal_colors(foreground, background);
	SetConsoleOutputCP(CP_UTF8);

	DWORD mode;
	GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), &mode);
	mode = (mode & (~ENABLE_PROCESSED_INPUT));
	SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), mode);

	SetConsoleCtrlHandler(console_signal_handler, TRUE);

	HWND hwnd = GetConsoleWindow();
	HMENU hmenu = GetSystemMenu(hwnd, FALSE);
	EnableMenuItem(hmenu, SC_CLOSE, MF_GRAYED);

	LPARAM icon = reinterpret_cast<LPARAM>(LoadIcon(GetModuleHandle(nullptr), MAKEINTRESOURCE(1)));
	SendMessage(hwnd, WM_SETICON, ICON_BIG, icon);
	SendMessage(hwnd, WM_SETICON, ICON_SMALL, icon);

	ustring_t app_name = engine::manifest_app_t::get_manifest_full_name();

	ustring_t app_data_str = engine::manifest_app_t::get_local_manifest_name();
	ustring_t app_data_canonized = app_data_str.substr(0, 1).to_upper();
	app_data_canonized.append(app_data_str.substr(1));

	app_name.append(" ("_u);
	app_name.append(app_data_canonized);
	app_name.append(")"_u);

	std::wstring app_name_w = app_name.to_wide();
	SetConsoleTitleW(app_name_w.c_str());
}

void engine::platform::set_terminal_colors(engine::platform::terminal_color_t foreground, engine::platform::terminal_color_t background)
{
	WORD attribs = color_pair_to_attribs(foreground, background);
	HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hOutput, &csbi);

	COORD coordScreen = { 0, 0 };
	DWORD dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
	DWORD cCharsWritten;

	FillConsoleOutputAttribute(hOutput, attribs, dwConSize, coordScreen, &cCharsWritten);

	CONSOLE_FONT_INFOEX info;
	memset(&info, 0, sizeof(CONSOLE_FONT_INFOEX));
	info.cbSize = sizeof(CONSOLE_FONT_INFOEX);
	if (GetCurrentConsoleFontEx(hOutput, FALSE, &info))
	{
		info.FontFamily = FF_DONTCARE;
		info.dwFontSize.X = 0;  // leave X as zero
		info.dwFontSize.Y = 16;
		info.FontWeight = 400;
		wcscpy(info.FaceName, L"Lucida Console");
		if (SetCurrentConsoleFontEx(hOutput, FALSE, &info));
	}

	set_console_colors_for_print(foreground, background);
}

void engine::platform::output_terminal_text(const ustring_t & text, engine::platform::terminal_color_t foreground, engine::platform::terminal_color_t background)
{
	set_console_colors_for_print(foreground, background);
	fputs(text.get_cstring(), stdout);
}

void engine::platform::output_terminal_new_line()
{
	puts("");
}

void engine::platform::close_terminal()
{
	FreeConsole();
}

#endif