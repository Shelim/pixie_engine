#ifndef ENGINE_COMMON_UTILITY_PLATFORM_INFO_HPP
#define ENGINE_COMMON_UTILITY_PLATFORM_INFO_HPP
#pragma once

#include<memory>
#include "utility/text/ustring.hpp"

namespace engine
{


	namespace platform
	{

		ustring_t get_os_version();
		uint32_t get_cache_line_size();
		ustring_t get_cpu_features();

	}


}

#include "utility/debug/callstack.hpp"

#endif