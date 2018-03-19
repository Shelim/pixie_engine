#include "global/core/platform/info.hpp"

#if PIXIE_WINDOWS

#include "utility/text/ustring.hpp"
#include "utility/text/ustring.hpp"

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

uint32_t engine::platform::get_total_ram_in_mb()
{
	return SDL_GetSystemRAM();
}

engine::cpu_features_t engine::platform::get_cpu_features()
{
	engine::cpu_features_t::builder_t builder;

	if (SDL_Has3DNow()) 	builder.set_feature(cpu_features_t::feature_t::_3d_now, true);
	if (SDL_HasAVX())		builder.set_feature(cpu_features_t::feature_t::avx, true);
	if (SDL_HasAltiVec()) 	builder.set_feature(cpu_features_t::feature_t::alti_vec, true);
	if (SDL_HasMMX())		builder.set_feature(cpu_features_t::feature_t::mmx, true);
	if (SDL_HasRDTSC())		builder.set_feature(cpu_features_t::feature_t::rdtsc, true);
	if (SDL_HasSSE())		builder.set_feature(cpu_features_t::feature_t::sse, true);
	if (SDL_HasSSE2())		builder.set_feature(cpu_features_t::feature_t::sse2, true);
	if (SDL_HasSSE3()) 		builder.set_feature(cpu_features_t::feature_t::sse3, true);
	if (SDL_HasSSE41()) 	builder.set_feature(cpu_features_t::feature_t::sse41, true);
	if (SDL_HasSSE42()) 	builder.set_feature(cpu_features_t::feature_t::sse42, true);

	return builder.build();
}

#endif