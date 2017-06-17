#include "common/platform.hpp"
#include "common/logger.hpp"
#include "common/manifest_app.hpp"
#include "common/logger_output/provider_base.hpp"
#include "common/engine.hpp"

#if PIXIE_WINDOWS

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

class engine::platform_t::implementation_t final
{
public:

	implementation_t(std::string arg0, engine::platform_t * platform, engine::logger_t * logger, engine::manifest_app_t * manifest_app): arg0(arg0), platform(platform), logger(logger), manifest_app(manifest_app)
	{
		impl = this;
	}

	implementation_t()
	{
		impl = nullptr;
	}

	static implementation_t * get_static()
	{
		return impl;
	}


	enum class console_color_t
	{
		red_dark,
		red_light,

		green_dark,
		green_light,

		blue_dark,
		blue_light,

		gray_dark,
		gray_light,

		white,
		black,

		yellow_dark,
		yellow_light,

		pink_dark,
		pink_light,

		teal_dark,
		teal_light
	};

private:

	std::string arg0;
	engine::platform_t * platform;
	engine::logger_t * logger;
	engine::manifest_app_t * manifest_app;

	HANDLE handle_single_instance;

	static implementation_t * impl;

	WORD color_to_attribs(console_color_t color)
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

	WORD background_to_attribs(console_color_t background)
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

	WORD color_pair_to_attribs(console_color_t color, console_color_t background)
	{
		return color_to_attribs(color) | background_to_attribs(background);
	}
	
public:

	void ensure_single_instance()
	{
		handle_single_instance = CreateMutexA(NULL, TRUE, "Pixie" PIXIE_OUTPUT_TYPE_STR);
		if (handle_single_instance == NULL || GetLastError() == ERROR_ALREADY_EXISTS)
		{
			HWND existingApp = FindWindowA("PixieGameWndClass", 0);
			if (existingApp) SetForegroundWindow(existingApp);
			exit(EXIT_FAILURE);
		}
	}

	void free_single_instance()
	{
		ReleaseMutex(handle_single_instance);
		CloseHandle(handle_single_instance);
	}

	enum class console_default_color_t
	{
#define GAME_RICHTEXT_COLOR_WINDOWS_CONSOLE_STD(key_name, key_color, key_backround) foreground_##key_name = static_cast<uint32_t>(console_color_t::key_color), background_##key_name = static_cast<uint32_t>(console_color_t::key_backround),
#include "common/std/richtext_std.hpp"
		foreground_default = static_cast<uint32_t>(console_color_t::gray_light),
		background_default = static_cast<uint32_t>(console_color_t::black)
	};

	static BOOL WINAPI console_signal_handler(DWORD signal)
	{
		if (signal == CTRL_C_EVENT || signal == CTRL_BREAK_EVENT)
		{
			engine::engine_t::get()->shutdown();
			exit(EXIT_FAILURE);
			return TRUE;
		}

		return FALSE;
	}

	void open_console()
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

		set_console_colors(color_pair_to_attribs(static_cast<console_color_t>(console_default_color_t::foreground_default), static_cast<console_color_t>(console_default_color_t::background_default)));

		SetConsoleOutputCP(CP_UTF8);

		/*
		DWORD mode;
		GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), &mode);
		mode = (mode & (~ENABLE_PROCESSED_INPUT));
		SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), mode);
		*/

		SetConsoleCtrlHandler(console_signal_handler, TRUE);

		HWND hwnd = GetConsoleWindow();
		HMENU hmenu = GetSystemMenu(hwnd, FALSE);
		EnableMenuItem(hmenu, SC_CLOSE, MF_GRAYED);

		ustring_t game_name = ustring_t::from_utf8(platform->get_manifest_app()->game_name());

		ustring_t app_data_str = _U(PIXIE_OUTPUT_TYPE_STR);
		ustring_t app_data_canonized = app_data_str.substr(0, 1).to_upper();
		app_data_canonized.append(app_data_str.substr(1));

		game_name.append(_U(" ("));
		game_name.append(app_data_canonized);
		game_name.append(_U(")"));

		std::wstring game_name_w = game_name.to_wide();
		SetConsoleTitleW(game_name_w.c_str());

	}

	VOID WINAPI set_console_colors_for_print(console_color_t color, console_color_t background)
	{
		HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hOutput, color_pair_to_attribs(color, background));
	}

	void close_console()
	{
		FreeConsole();
	}

	void copy_engine_dir(engine::virtual_path_t::type_t type, const char * type_str)
	{
		std::error_code ec;

		auto path_dst = get_save_path(type);
		path_dst /= "engine";
		path_dst /= type_str;
		std::filesystem::create_directories(path_dst, ec);

		auto path_src = get_exe_path();
		path_src /= "engine";
		path_src /= type_str;

		std::filesystem::copy(path_src, path_dst, std::filesystem::copy_options::recursive | std::filesystem::copy_options::skip_existing, ec);

		copy_engine_base(type);
	}

	void copy_engine_base(engine::virtual_path_t::type_t type)
	{
		std::error_code ec;

		auto path_dst = get_save_path(type);
		path_dst /= "engine";
		std::filesystem::create_directories(path_dst, ec);

		auto path_src = get_exe_path();
		path_src /= "engine";

		for (auto & iter : std::filesystem::directory_iterator(path_src))
		{
			if (std::filesystem::is_regular_file(iter))
			{
				auto dst = path_dst;
				dst /= iter.path().filename();
				copy_file(iter.path(), dst, std::filesystem::copy_options::skip_existing, ec);
			}
		}
	}

	engine::platform_t * get_platform() const
	{
		return platform;
	}

	engine::logger_t * get_logger() const
	{
		return logger;
	}

	void set_logger(engine::logger_t * logger)
	{
		this->logger = logger;
	}
	
	std::filesystem::path get_exe_path()
	{
		wchar_t module_filename[MAX_PATH];
		GetModuleFileNameW(NULL, module_filename, MAX_PATH);
		std::filesystem::path path = module_filename;
		if (path.has_filename()) path.remove_filename();
		return path;
	}
	
	enum class directory_special_t
	{
		local_app_data,
		roaming_app_data,
		saves,
		crash_dumps
	};

	std::filesystem::path get_special_path(directory_special_t dir)
	{
		ustring_t ret;

		switch (dir)
		{
		case directory_special_t::local_app_data: ret = ustring_t::from_wide(_wgetenv(L"LOCALAPPDATA")); break;
		case directory_special_t::roaming_app_data: ret = ustring_t::from_wide(_wgetenv(L"APPDATA")); break;
		case directory_special_t::saves: ret = ustring_t::from_wide(_wgetenv(L"USERPROFILE")); ret.append_utf8(u8"\\Saved Games"); break;
		case directory_special_t::crash_dumps: ret = ustring_t::from_wide(_wgetenv(L"LOCALAPPDATA")); break;
		}
		ret.append_utf8(u8"\\");
		if(dir == directory_special_t::saves)
			ret.append_utf8(manifest_app->game_name());
		else
			ret.append_utf8(manifest_app->windows_game_app_dir());
		ret.append_utf8(u8"\\");

		return ret.get_cstring();
	}

	std::filesystem::path get_save_path(virtual_path_t::type_t t)
	{
#if !PIXIE_IS_PORTABLE_BUILD

#define GAME_VIRTUAL_PATH_STD(name, type_id, dirname) if(t == virtual_path_t::type_t::name) { std::filesystem::path ret = get_special_path(directory_special_t::type_id); if(strlen(dirname) > 0) ret /= dirname; return ret; }
#include "common/std/virtual_path_std.hpp"

#else

#define GAME_VIRTUAL_PATH_STD(name, type_id, dirname) if(t == virtual_path_t::type_t::name) { std::filesystem::path ret = get_exe_path(); if(strlen(dirname) > 0) ret /= dirname; else if(t == virtual_path_t::type_t::saves) ret /= "saves"; return ret; }
#include "common/std/virtual_path_std.hpp"

#endif

		return std::filesystem::path();
	}
	
	void enable_crashing_on_crashes() // Seriously...
	{
		typedef BOOL(WINAPI *tGetPolicy)(LPDWORD lpFlags);
		typedef BOOL(WINAPI *tSetPolicy)(DWORD dwFlags);
		const DWORD EXCEPTION_SWALLOWING = 0x1;

		HMODULE kernel32 = LoadLibraryA("kernel32.dll");
		tGetPolicy pGetPolicy = (tGetPolicy)GetProcAddress(kernel32,
			"GetProcessUserModeExceptionPolicy");
		tSetPolicy pSetPolicy = (tSetPolicy)GetProcAddress(kernel32,
			"SetProcessUserModeExceptionPolicy");
		if (pGetPolicy && pSetPolicy)
		{
			DWORD dwFlags;
			if (pGetPolicy(&dwFlags))
			{
				// Turn off the filter
				pSetPolicy(dwFlags & ~EXCEPTION_SWALLOWING);
			}
		}
	}

	// Returns path to minidump
	std::filesystem::path make_minidump(EXCEPTION_POINTERS* e)
	{
		auto hDbgHelp = LoadLibraryA("dbghelp");
		if (hDbgHelp == nullptr)
			return filesystem::path();
		auto pMiniDumpWriteDump = (decltype(&MiniDumpWriteDump))GetProcAddress(hDbgHelp, "MiniDumpWriteDump");
		if (pMiniDumpWriteDump == nullptr)
			return filesystem::path();

		wchar_t name[MAX_PATH];
		SYSTEMTIME t;
		GetLocalTime(&t);

		ustring_t unix_name = ustring_t::from_utf8(platform->get_manifest_app()->unix_name());
		std::wstring unix_name_w = unix_name.to_wide();
		wsprintfW(name, L"%s_%s_build_%d_at_%4d%02d%02d_%02d%02d%02d.dmp", unix_name_w.c_str(), _L(PIXIE_OUTPUT_TYPE_STR), impl->get_platform()->get_manifest_app()->version_build(), t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond);

		std::filesystem::path path = platform->get_pimpl()->get_save_path(virtual_path_t::type_t::crash_dumps);
		std::error_code ec;
		create_directories(path, ec);
		path /= name;

		auto hFile = CreateFileW(path.c_str(), GENERIC_WRITE, FILE_SHARE_READ, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
		if (hFile == INVALID_HANDLE_VALUE)
			return std::filesystem::path();

		MINIDUMP_EXCEPTION_INFORMATION exceptionInfo;
		exceptionInfo.ThreadId = GetCurrentThreadId();
		exceptionInfo.ExceptionPointers = e;
		exceptionInfo.ClientPointers = FALSE;

		auto dumped = pMiniDumpWriteDump(
			GetCurrentProcess(),
			GetCurrentProcessId(),
			hFile,
			MINIDUMP_TYPE(MiniDumpWithIndirectlyReferencedMemory | MiniDumpScanMemory),
			e ? &exceptionInfo : nullptr,
			nullptr,
			nullptr);

		CloseHandle(hFile);

		return path;
	}

	static LONG CALLBACK unhandled_handler(EXCEPTION_POINTERS* e)
	{
		static int  CurrentlyInTheFaultHandler = 0;
		if (CurrentlyInTheFaultHandler == 1)
		{
			if (impl && impl->logger)
				impl->logger->p_err_raport(_U("Crashed during handling previous crash (most likely callstack walking). Aborting..."));
			return EXCEPTION_CONTINUE_SEARCH;
		}
		CurrentlyInTheFaultHandler = 1;

		if (impl)
		{
			char  *FaultTx = "";

			switch (e->ExceptionRecord->ExceptionCode)
			{
			case EXCEPTION_ACCESS_VIOLATION: FaultTx = "ACCESS VIOLATION"; break;
			case EXCEPTION_DATATYPE_MISALIGNMENT: FaultTx = "DATATYPE MISALIGNMENT"; break;
			case EXCEPTION_BREAKPOINT: FaultTx = "BREAKPOINT"; break;
			case EXCEPTION_SINGLE_STEP: FaultTx = "SINGLE STEP"; break;
			case EXCEPTION_ARRAY_BOUNDS_EXCEEDED: FaultTx = "ARRAY BOUNDS EXCEEDED"; break;
			case EXCEPTION_FLT_DENORMAL_OPERAND: FaultTx = "FLT DENORMAL OPERAND"; break;
			case EXCEPTION_FLT_DIVIDE_BY_ZERO: FaultTx = "FLT DIVIDE BY ZERO"; break;
			case EXCEPTION_FLT_INEXACT_RESULT: FaultTx = "FLT INEXACT RESULT"; break;
			case EXCEPTION_FLT_INVALID_OPERATION: FaultTx = "FLT INVALID OPERATION"; break;
			case EXCEPTION_FLT_OVERFLOW: FaultTx = "FLT OVERFLOW"; break;
			case EXCEPTION_FLT_STACK_CHECK: FaultTx = "FLT STACK CHECK"; break;
			case EXCEPTION_FLT_UNDERFLOW: FaultTx = "FLT UNDERFLOW"; break;
			case EXCEPTION_INT_DIVIDE_BY_ZERO: FaultTx = "INT DIVIDE BY ZERO"; break;
			case EXCEPTION_INT_OVERFLOW: FaultTx = "INT OVERFLOW"; break;
			case EXCEPTION_PRIV_INSTRUCTION: FaultTx = "PRIV INSTRUCTION"; break;
			case EXCEPTION_IN_PAGE_ERROR: FaultTx = "IN PAGE ERROR"; break;
			case EXCEPTION_ILLEGAL_INSTRUCTION: FaultTx = "ILLEGAL INSTRUCTION"; break;
			case EXCEPTION_NONCONTINUABLE_EXCEPTION: FaultTx = "NONCONTINUABLE EXCEPTION"; break;
			case EXCEPTION_STACK_OVERFLOW: FaultTx = "STACK OVERFLOW"; break;
			case EXCEPTION_INVALID_DISPOSITION: FaultTx = "INVALID DISPOSITION"; break;
			case EXCEPTION_GUARD_PAGE: FaultTx = "GUARD PAGE"; break;
			default: FaultTx = "(unknown)";           break;
			}
			uint32_t    wsFault = e->ExceptionRecord->ExceptionCode;
			void * CodeAdress = e->ExceptionRecord->ExceptionAddress;

			filesystem::path path = impl->make_minidump(e);
			ustring_t upath = ustring_t::from_wide(path.wstring().c_str());
			ustring_t ufile = ustring_t::from_wide(path.filename().wstring().c_str());

			engine::ustring_t cmd = engine::format_utf8(u8"explorer /n,/select,\"#1#\"", upath);

			system(cmd.get_cstring());
			if (impl->logger)
			{
				impl->logger->p_err_raport(_U("Crashed with #1# (Ex. #2# at #3#)!"), engine::ustring_t::from_utf8(FaultTx), wsFault, CodeAdress);
				impl->logger->p_fatal_raport(_U("Crash dump has been saved to '#1#', please e-mail it to the developers!"), path.filename());
			}

			CurrentlyInTheFaultHandler = 0;

			return EXCEPTION_EXECUTE_HANDLER;
		}
		return EXCEPTION_CONTINUE_SEARCH;
	}

	VOID WINAPI set_console_colors(WORD attribs)
	{
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
	}

};

#endif