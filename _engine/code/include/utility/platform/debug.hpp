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

		/**
		 * @brief Triggers platform-specific breakpoint
		 * 
		 * @warning this function may result in hard-crash if debugger is not present!
		 */
		void trigger_breakpoint();

		/**
		 * @brief This function canonize @c __FILE__ into something that can be safely displayed to end-user
		 * 
		 * It is strictly platform-specific; You can assume that it will (at least):
		 *  - Remove leading path that may contain personal directory name or directory structure
		 *  - Canonize slashes
		 * 
		 * @param[in] filename You need to pass @c __FILE__ here
		 * @return Canonized name
		 */
		ustring_t canonize_debug_filename(const ustring_t & filename);

		/**
		 * @brief Dumps current callstack, removing optionally some frames from top
		 * 
		 * This function is intended for debugging hard-crashes only. Each function call occupy one frame in resulting callstack_t
		 * 
		 * @warning This function may be slow to execute. You should always remove it in production-wise code
		 * 
		 * @param[in] skip_top How many stack frames from top must be removed
		 * @return Generated callstack
		 */
		callstack_t dump_callstack(std::size_t skip_top = 0);
			
	}


}

#include "utility/debug/callstack.hpp"

#endif