#ifndef ENGINE_COMMON_UTILITY_PLATFORM_DEBUG_HPP
#define ENGINE_COMMON_UTILITY_PLATFORM_DEBUG_HPP
#pragma once

#include<memory>
#include "utility/text/ustring.hpp"

namespace engine
{

	class callstack_t;

	namespace platform
	{

		void trigger_breakpoint();

		ustring_t canonize_debug_filename(const ustring_t & filename);

		callstack_t dump_callstack(std::size_t skip_top = 0);
			
	}


}

#include "utility/debug/callstack.hpp"

#endif