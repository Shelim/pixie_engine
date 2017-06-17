#include "common/platform.hpp"

#if PIXIE_WINDOWS

#include "platform_pimpl.hpp"
#include "common/filesystem.hpp"

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

engine::platform_t::implementation_t * engine::platform_t::implementation_t::impl = nullptr;

/*
void engine::engine_t::platform_on_init()
{
	platform->get_pimpl()->ensure_single_instance();
}

void engine::engine_t::platform_on_shutdown()
{
	platform->get_pimpl()->free_single_instance();
}
*/
engine::platform_t::platform_t(std::string arg0, std::shared_ptr<manifest_app_t> manifest_app) : pimpl(std::make_unique<implementation_t>(arg0, this, nullptr, manifest_app.get())), manifest_app(manifest_app)
{
	std::setlocale(LC_ALL, "C");
	pimpl->enable_crashing_on_crashes();
	SetUnhandledExceptionFilter(engine::platform_t::implementation_t::unhandled_handler);
	reset_working_directory();

	CoInitialize(0);

#if PIXIE_IS_DEBUG_BUILD
	pimpl->open_console();
#endif

	std::error_code ec;

#define GAME_VIRTUAL_PATH_STD(name, type, path) std::filesystem::create_directories(pimpl->get_save_path(engine::virtual_path_t::type_t::name), ec);
#include "common/std/virtual_path_std.hpp"

#if !PIXIE_IS_PORTABLE_BUILD

	pimpl->copy_engine_dir(engine::virtual_path_t::type_t::config, "config");
	pimpl->copy_engine_dir(engine::virtual_path_t::type_t::keybinding, "keybinding");

#endif

	/*
	#if !PIXIE_IS_PORTABLE_BUILD

	OSVERSIONINFO OS;
	OS.dwOSVersionInfoSize = sizeof(OS);
	GetVersionEx(&OS);

	if (OS.dwPlatformId < 6)
	{
		HKEY hKey;
		LONG lRes = RegOpenKeyExW(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders", 0, KEY_READ, &hKey);
		WCHAR szBuffer[MAX_PATH + 1];
		DWORD dwBufferSize = sizeof(szBuffer);
		ULONG nError;
		nError = RegQueryValueExW(hKey, L"Local AppData", 0, NULL, (LPBYTE)szBuffer, &dwBufferSize);
		if (ERROR_SUCCESS == nError)
		{
			static WCHAR variableBuffer[MAX_PATH + 1];
			wcscpy(variableBuffer, L"LocalAppData=");
			wcscat(variableBuffer, szBuffer);
			_wputenv(variableBuffer);
		}
	}
	{
		std::error_code ec;
		filesystem::path path1 = get_path(path_special_t::saves);
		filesystem::path path2 = get_exe_path(); path2 /= "saves";
		engine::create_directories(path1, ec);
		filesystem::create_directory_symlink(path1, path2, ec);
	}
	{
		std::error_code ec;
		filesystem::path path1 = get_path(path_special_t::config);
		filesystem::path path2 = get_exe_path(); path2 /= "engine/config";
		filesystem::path path3 = path1; path3 /= "config.xml";
		filesystem::path path4 = get_exe_path(); path4 /= "config";
		engine::create_directories(path1, ec);
		filesystem::copy(path2, path1, filesystem::copy_options::recursive, ec);
		filesystem::create_symlink(path3, path4, ec);
	}
	{
		std::error_code ec;
		filesystem::path path1 = get_path(path_special_t::keybinding);
		filesystem::path path2 = get_exe_path(); path2 /= "engine/keybinding";
		filesystem::path path3 = path1; path3 /= "keybinding.xml";
		filesystem::path path4 = get_exe_path(); path4 /= "keybinding";
		engine::create_directories(path1, ec);
		filesystem::copy(path2, path1, filesystem::copy_options::recursive, ec);
		filesystem::create_symlink(path3, path4, ec);
	}
	{
		std::error_code ec;
		filesystem::path path1 = get_path(path_special_t::log);
		engine::create_directories(path1, ec);
		filesystem::path path2 = get_exe_path(); path2 /= "engine/log";
		filesystem::path path3 = get_exe_path(); path3 /= "log";
		filesystem::copy(path2, path1, filesystem::copy_options::recursive, ec);
		filesystem::create_directory_symlink(path1, path3, ec);
	}
#else
	std::error_code ec;
	filesystem::path path = get_path(path_special_t::saves);
	std::filesystem::create_directories(path, ec);
#endif
	*/
}

void engine::platform_t::set_logger(engine::logger_t * logger)
{
	pimpl->set_logger(logger);
	this->logger = logger;
}

engine::platform_t::~platform_t()
{
#if PIXIE_IS_DEBUG_BUILD
	pimpl->close_console();
#endif

	CoUninitialize();
}

void engine::platform_t::copy_to_clipboard(const ustring_t & message)
{
	std::wstring msg = message.to_wide();

	const size_t len = (wcslen(msg.c_str()) + 1) * sizeof(wchar_t);
	HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, len);

	memcpy(GlobalLock(hMem), msg.c_str(), len);
	GlobalUnlock(hMem);

	OpenClipboard(0);

	EmptyClipboard();
	SetClipboardData(CF_UNICODETEXT, hMem);
	CloseClipboard();
}

void engine::platform_t::reset_working_directory()
{
	std::wstring path_final = pimpl->get_exe_path().wstring();
	SetCurrentDirectoryW(path_final.c_str());
}

engine::ustring_t engine::platform_t::get_platform_version() const
{
	OSVERSIONINFO osvi;
	BOOL bIsWindowsXPorLater;

	ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

	GetVersionEx(&osvi);

	return engine::format_utf8(u8"#1#.#2# (Build #3#) #4#", (uint32_t)osvi.dwMajorVersion, (uint32_t)osvi.dwMinorVersion, (uint32_t)osvi.dwBuildNumber, ustring_t::from_wide(osvi.szCSDVersion));
}

void engine::platform_t::show_splashscreen(const virtual_path_t & virtual_path)
{

}

std::string engine::engine_container_t::platform_get_executable_filename()
{
	CHAR file_name[MAX_PATH];
	GetModuleFileNameA(NULL, file_name, MAX_PATH);

	return file_name;
}

#endif