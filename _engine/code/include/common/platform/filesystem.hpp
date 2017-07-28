#ifndef ENGINE_COMMON_PLATFORM_FILESYSTEM_HPP
#define ENGINE_COMMON_PLATFORM_FILESYSTEM_HPP

#include "utility/vfs/virtual_path.hpp"
#include "utility/vfs/filesystem.hpp"
#include "utility/text/ustring.hpp"
#include "provider/save_location.hpp"
#include "utility/data/output.hpp"
#include <memory>

namespace engine
{

	namespace platform
	{

		std::unique_ptr<engine::data::output_t> create_new(const virtual_path_t & path, std::shared_ptr<save_location_provider_t> save_location_provider);
	}

}

#endif