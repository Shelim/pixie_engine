#ifndef ENGINE_COMMON_PLATFORM_DEBUG_HPP
#define ENGINE_COMMON_PLATFORM_DEBUG_HPP

#include "utility/vfs/virtual_path.hpp"
#include "provider/save_location.hpp"
#include "utility/data/output.hpp"
#include "utility/debug/callstack.hpp"
#include <memory>

namespace engine
{

	namespace platform
	{

		void breakpoint();

		ustring_t canonize_debug_filename(const ustring_t & filename);

		::engine::callstack_t dump_callstack(std::size_t skip_top = 0);
		
	}

}

#endif