#ifndef ENGINE_COMMON_PLATFORM_INFO_HPP
#define ENGINE_COMMON_PLATFORM_INFO_HPP

#include "utility/text/ustring.hpp"
#include "utility/pattern/compilation.hpp"

namespace engine
{

	namespace platform
	{
		static ustring_t get_os()
		{
			compilation_t::os_t os_local = compilation_t::get_os();

#define ENGINE_OS_STD(os) if(os_local == compilation_t::os_t::os) return #os##_u;
#include "std/os_std.hpp"

			return "unknown"_u;
		}

		ustring_t get_os_version();

	}

}

#endif