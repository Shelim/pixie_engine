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

uint32_t engine::platform::get_cache_line_size()
{
	return SDL_GetCPUCacheLineSize();
}

engine::ustring_t engine::platform::get_cpu_features()
{
	ustring_t features;
	if (SDL_Has3DNow()) features.append_ascii("3DNow ");
	if (SDL_HasAltiVec()) features.append_ascii("AltiVec ");
	if (SDL_HasMMX()) features.append_ascii("MMX ");
	if (SDL_HasRDTSC()) features.append_ascii("RDTSC ");
	if (SDL_HasSSE()) features.append_ascii("SSE ");
	if (SDL_HasSSE2()) features.append_ascii("SSE2 ");
	if (SDL_HasSSE3()) features.append_ascii("SSE3 ");
	if (SDL_HasSSE41()) features.append_ascii("SSE41 ");
	if (SDL_HasSSE42()) features.append_ascii("SSE42 ");

	if (features.len() > 0)
		return features.substr(0, features.len() - 1);

	return features;
}

#endif