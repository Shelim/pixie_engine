#ifndef ENGINE_COMMON_UTILITY_PATTERN_BUILD_CONFIGURATAION_HPP
#define ENGINE_COMMON_UTILITY_PATTERN_BUILD_CONFIGURATAION_HPP
#pragma once


#define XSTR(a) STR(a)
#define STR(a) #a

namespace engine
{
	class compilation_t
	{

	public:

		constexpr static bool is_debug_build()
		{
#if PIXIE_IS_DEBUG_BUILD
			return true;
#else
			return false;
#endif
		}

		constexpr static bool is_portable_build()
		{
#if PIXIE_IS_PORTABLE_BUILD
			return true;
#else
			return false;
#endif
		}

		constexpr static bool is_deploy_build()
		{
#if PIXIE_IS_DEPLOY_BUILD
			return true;
#else
			return false;
#endif
		}

		constexpr static bool is_ready_build()
		{
#if PIXIE_IS_READY_BUILD
			return true;
#else
			return false;
#endif
		}

		constexpr static bool is_windows_build()
		{
#if PIXIE_WINDOWS
			return true;
#else
			return false;
#endif
		}

		enum class platform_t
		{
#define ENGINE_PLATFORM_STD(platform) platform,
#include "std/platforms_std.hpp"
			count
		};

		constexpr static platform_t get_platform()
		{
#if PIXIE_WINDOWS
			return platform_t::windows;
#endif
		}

		constexpr static const char * output_unix_name()
		{
			return XSTR(PIXIE_OUTPUT_UNIX_NAME);
		}

		constexpr static const char * app_unix_name()
		{
			return XSTR(PIXIE_APP_UNIX_NAME);
		}
	};
}

#endif