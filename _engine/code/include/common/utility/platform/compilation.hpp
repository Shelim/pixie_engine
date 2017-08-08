#ifndef ENGINE_COMMON_UTILITY_PLATFORM_COMPILATION_HPP
#define ENGINE_COMMON_UTILITY_PLATFORM_COMPILATION_HPP
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

		enum class os_t
		{
#define ENGINE_OS_STD(os) os,
#include "std/os_std.hpp"
			count
		};

		constexpr static os_t get_os()
		{
#if PIXIE_WINDOWS
			return os_t::windows;
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