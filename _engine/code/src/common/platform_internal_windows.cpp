#include "component/platform_internal/windows.hpp"
#include "utility/text/stringify.hpp"

#if PIXIE_WINDOWS

engine::platform_internal_windows_t::platform_internal_windows_t(std::shared_ptr<manifest_app_t> manifest_app) : manifest_app(manifest_app)
{

}

std::filesystem::path engine::platform_internal_windows_t::get_self_path()
{
	wchar_t module_filename[MAX_PATH];
	GetModuleFileNameW(NULL, module_filename, MAX_PATH);
	std::filesystem::path path = module_filename;
	if (path.has_filename()) path.remove_filename();
	return path;
}

std::filesystem::path engine::platform_internal_windows_t::get_special_path(directory_special_t dir)
{
	ustring_t ret;

	switch (dir)
	{
	case directory_special_t::local_app_data: ret = ustring_t::from_wide(_wgetenv(L"LOCALAPPDATA")); break;
	case directory_special_t::roaming_app_data: ret = ustring_t::from_wide(_wgetenv(L"APPDATA")); break;
	case directory_special_t::saves: ret = ustring_t::from_wide(_wgetenv(L"USERPROFILE")); ret.append("\\Saved Games"_u); break;
	case directory_special_t::crash_dumps: ret = ustring_t::from_wide(_wgetenv(L"LOCALAPPDATA")); break;
	}
	ret.append("\\"_u);
	if (dir == directory_special_t::saves)
		ret.append(manifest_app->get_manifest_full_name());
	else
		ret.append(manifest_app->get_manifest_windows_game_app_dir());
	ret.append("\\"_u);

	return ret.get_cstring();

}

std::filesystem::path engine::platform_internal_t::get_save_path(const virtual_path_t & path)
{
#if PIXIE_IS_PORTABLE_BUILD

#define GAME_VIRTUAL_PATH_STD(name, type_id, dirname) if(t == virtual_path_t::type_t::name) { std::filesystem::path ret = get_self_path(); if(strlen(dirname) > 0) ret /= dirname; else if(t == virtual_path_t::type_t::saves) ret /= "saves"; ret /= path.get_path().get_cstring(); return ret; }
#include "std/virtual_path_std.hpp"

#else

#define GAME_VIRTUAL_PATH_STD(name, type_id, dirname) if(t == virtual_path_t::type_t::name) { std::filesystem::path ret = get_special_path(directory_special_t::type_id); if(strlen(dirname) > 0) ret /= dirname; ret /= path.get_path().get_cstring(); return ret; }
#include "std/virtual_path_std.hpp"

#endif

	std::filesystem::path ret = get_self_path();
	ret /= path.get_path().get_cstring();
	return ret;
}

void engine::platform_internal_windows_t::open_console()
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

	set_console_colors(static_cast<console_color_t>(console_default_color_t::foreground_default), static_cast<console_color_t>(console_default_color_t::background_default));
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

	ustring_t app_name = manifest_app->get_manifest_full_name();

	ustring_t app_data_str = manifest_app->get_local_manifest_name();
	ustring_t app_data_canonized = app_data_str.substr(0, 1).to_upper();
	app_data_canonized.append(app_data_str.substr(1));

	app_name.append(" ("_u);
	app_name.append(app_data_canonized);
	app_name.append(")"_u);

	std::wstring app_name_w = app_name.to_wide();
	SetConsoleTitleW(app_name_w.c_str());
}

VOID WINAPI engine::platform_internal_windows_t::set_console_colors_for_print(console_color_t color, console_color_t background)
{
	HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hOutput, color_pair_to_attribs(color, background));
}

VOID WINAPI engine::platform_internal_windows_t::set_console_colors(console_color_t color, console_color_t background)
{
	WORD attribs = color_pair_to_attribs(color, background);
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

	set_console_colors_for_print(color, background);
}

BOOL WINAPI engine::platform_internal_windows_t::console_signal_handler(DWORD signal)
{
	if (signal == CTRL_C_EVENT || signal == CTRL_BREAK_EVENT)
	{
		//		engine::engine_t::get()->shutdown(); // ToDo!
		exit(EXIT_FAILURE);
		return TRUE;
	}

	return FALSE;
}

void engine::platform_internal_windows_t::close_console()
{
	FreeConsole();
}

void engine::platform_internal_windows_t::console_write(const richtext_t & richtext)
{

}

WORD engine::platform_internal_windows_t::color_to_attribs(console_color_t color)
{
	switch (color)
	{
	case console_color_t::red_dark: return FOREGROUND_RED;
	case console_color_t::red_light: return FOREGROUND_RED | FOREGROUND_INTENSITY;
	case console_color_t::green_dark: return FOREGROUND_GREEN;
	case console_color_t::green_light: return FOREGROUND_GREEN | FOREGROUND_INTENSITY;
	case console_color_t::blue_dark: return FOREGROUND_BLUE;
	case console_color_t::blue_light: return FOREGROUND_BLUE | FOREGROUND_INTENSITY;
	case console_color_t::gray_dark: return FOREGROUND_INTENSITY;
	case console_color_t::gray_light: return FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
	case console_color_t::white: return FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
	case console_color_t::black: return 0;
	case console_color_t::yellow_dark: return FOREGROUND_RED | FOREGROUND_GREEN;
	case console_color_t::yellow_light: return FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
	case console_color_t::pink_dark: return FOREGROUND_RED | FOREGROUND_BLUE;
	case console_color_t::pink_light: return FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
	case console_color_t::teal_dark: return FOREGROUND_GREEN | FOREGROUND_BLUE;
	case console_color_t::teal_light: return FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
	}
	return (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

WORD engine::platform_internal_windows_t::background_to_attribs(console_color_t background)
{
	switch (background)
	{
	case console_color_t::red_dark: return BACKGROUND_RED;
	case console_color_t::red_light: return BACKGROUND_RED | BACKGROUND_INTENSITY;
	case console_color_t::green_dark: return BACKGROUND_GREEN;
	case console_color_t::green_light: return BACKGROUND_GREEN | BACKGROUND_INTENSITY;
	case console_color_t::blue_dark: return BACKGROUND_BLUE;
	case console_color_t::blue_light: return BACKGROUND_BLUE | BACKGROUND_INTENSITY;
	case console_color_t::gray_dark: return BACKGROUND_INTENSITY;
	case console_color_t::gray_light: return BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE;
	case console_color_t::white: return BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY;
	case console_color_t::black: return 0;
	case console_color_t::yellow_dark: return BACKGROUND_RED | BACKGROUND_GREEN;
	case console_color_t::yellow_light: return BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY;
	case console_color_t::pink_dark: return BACKGROUND_RED | BACKGROUND_BLUE;
	case console_color_t::pink_light: return BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_INTENSITY;
	case console_color_t::teal_dark: return BACKGROUND_GREEN | BACKGROUND_BLUE;
	case console_color_t::teal_light: return BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY;
	}
	return 0;
}

WORD engine::platform_internal_windows_t::color_pair_to_attribs(console_color_t color, console_color_t background)
{
	return color_to_attribs(color) | background_to_attribs(background);
}


#endif