#ifndef ENGINE_COMMON_UTILITY_MANIFEST_OS_HPP
#define ENGINE_COMMON_UTILITY_MANIFEST_OS_HPP

#include "utility/text/ustring.hpp"

#pragma once

namespace engine
{

	class manifest_os_t
	{

	public:

#define ENGINE_OS_DEF(...) DEFINE_ENUM_ONLY_1ST_TYPE(os_t, __VA_ARGS__)
#include "def/os.def"

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

#define ENGINE_OS_DEF(...) DEFINE_ENUM_ONLY_1ST_TO_STRING(engine::manifest_os_t::os_t, __VA_ARGS__)
#include "def/os.def"

#endif