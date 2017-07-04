#ifndef ENGINE_COMPONENT_PLATFORM_INTERNAL_WINDOWS_HPP
#define ENGINE_COMPONENT_PLATFORM_INTERNAL_WINDOWS_HPP
#pragma once
#if PIXIE_WINDOWS

#include "component/platform_internal.hpp"

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

namespace engine
{

	class platform_internal_windows_t : public platform_internal_t
	{

	public:

		platform_internal_windows_t();

		void console_write(const richtext_t & richtext) final;

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

		enum class console_default_color_t
		{
#define ENGINE_RICHTEXT_COLOR_WINDOWS_CONSOLE_STD(key_name, key_color, key_backround) foreground_##key_name = static_cast<uint32_t>(console_color_t::key_color), background_##key_name = static_cast<uint32_t>(console_color_t::key_backround),
#include "std/richtext_std.hpp"

			foreground_default = static_cast<uint32_t>(console_color_t::gray_light),
			background_default = static_cast<uint32_t>(console_color_t::black)
		};
		
		WORD color_to_attribs(console_color_t color);
		WORD background_to_attribs(console_color_t background);
		WORD color_pair_to_attribs(console_color_t color, console_color_t background);
		VOID WINAPI set_console_colors_for_print(console_color_t color, console_color_t background);
		VOID WINAPI set_console_colors(WORD attribs);

		static BOOL WINAPI console_signal_handler(DWORD signal);

		void open_console() final;
		void close_console() final;
	};
}

#endif
#endif