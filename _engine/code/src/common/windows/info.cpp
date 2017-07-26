#include "platform/info.hpp"

#if PIXIE_WINDOWS

#include "utility/text/ustring.hpp"
#include "utility/text/parser.hpp"
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

engine::ustring_t engine::platform::get_os_version()
{
	OSVERSIONINFO osvi;
	BOOL bIsWindowsXPorLater;

	ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

	GetVersionEx(&osvi);

	return engine::format_string("Windows #1#.#2# (Build #3#) #4#"_u, (uint32_t)osvi.dwMajorVersion, (uint32_t)osvi.dwMinorVersion, (uint32_t)osvi.dwBuildNumber, ustring_t::from_wide(osvi.szCSDVersion));
}

#endif