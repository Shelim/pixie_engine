#include "common/platform.hpp"
#include "common/logger.hpp"
#include "common/manifest_app.hpp"

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


private:

	std::string arg0;
	engine::platform_t * platform;
	engine::logger_t * logger;
	engine::manifest_app_t * manifest_app;

	static implementation_t * impl;

public:

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

	VOID WINAPI set_console_colors_for_print(WORD attribs)
	{
		HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hOutput, attribs);
	}
};

#endif