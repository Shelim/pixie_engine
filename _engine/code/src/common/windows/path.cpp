#include "platform/path.hpp"

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


engine::ustring_t engine::platform::get_self_path()
{
	wchar_t module_filename[MAX_PATH];
	GetModuleFileNameW(NULL, module_filename, MAX_PATH);
	std::filesystem::path path = module_filename;
	if (path.has_filename()) path.remove_filename();
	std::string path_utf8 = path.u8string();

	return ustring_t::from_utf8(path_utf8.c_str());
}

void engine::platform::ensure_directory_exits(std::filesystem::path path)
{
	std::error_code ec;
	std::filesystem::create_directories(path, ec);
}

engine::ustring_t engine::platform::resolve_directory(engine::platform::directory_special_t directory)
{
	engine::ustring_t ret;

	switch (directory)
	{
	case engine::platform::directory_special_t::local_app_data: ret = engine::ustring_t::from_wide(_wgetenv(L"LOCALAPPDATA")); ret.append('\\').append(engine::manifest_app_t::get_manifest_windows_game_app_dir()).append('\\'); break;
	case engine::platform::directory_special_t::roaming_app_data: ret = engine::ustring_t::from_wide(_wgetenv(L"APPDATA")); ret.append('\\').append(engine::manifest_app_t::get_manifest_windows_game_app_dir()).append('\\'); break;
	case engine::platform::directory_special_t::saves: ret = engine::ustring_t::from_wide(_wgetenv(L"USERPROFILE")); ret.append("\\Saved Games\\"_u).append(engine::manifest_app_t::get_manifest_full_name()).append('\\'); break;
	}

	return ret;
}

/*

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
	ret.append('\\');
	if (dir == directory_special_t::saves)
		ret.append(get_manifest_app()->get_manifest_full_name());
	else
		ret.append(get_manifest_app()->get_manifest_windows_game_app_dir());
	ret.append('\\');

	return ret.get_cstring();

}

*/

#endif