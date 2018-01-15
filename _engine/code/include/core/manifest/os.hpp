#ifndef ENGINE_COMMON_UTILITY_MANIFEST_OS_HPP
#define ENGINE_COMMON_UTILITY_MANIFEST_OS_HPP

#include "utility/text/ustring.hpp"

#pragma once

namespace engine
{

	class manifest_os_t
	{

	public:

#include "def/enum/os.def"

        constexpr os_t get_os()
        {
#if PIXIE_WINDOWS
            return os_t::windows;
#else
#error "Please, define this platform!"
#endif
        }

	};

}

#define ENGINE_ENUM_HEADER_TO_USE "def/enum/os.def"
#include "core/utility/enum_to_string.hpp"

#endif