
#ifndef ENGINE_COMPONENT_DATA_COMMON_FILESYSTEM_PATHS_PROVIDER_HPP
#define ENGINE_COMPONENT_DATA_COMMON_FILESYSTEM_PATHS_PROVIDER_HPP
#pragma once

#include "utility/pattern/provider.hpp"
#include "utility/text/ustring.hpp"
#include "utility/pattern/class_settings.hpp"
#include "global/core/vfs/filesystem.hpp"
#include "global/core/vfs/virtual_path.hpp"

namespace engine
{

	class filesystem_paths_provider_base_t
	{

	public:

		virtual ~filesystem_paths_provider_base_t()
		{

		}

        typedef std::vector<std::filesystem::path> paths_t;

        virtual paths_t get_read_paths(const virtual_path_t & filename) = 0;
        virtual paths_t get_write_paths(const virtual_path_t & filename) = 0;
        virtual std::filesystem::path get_path_for_new_file(const virtual_path_t & filename) = 0;

	};

	class filesystem_paths_t
	{
		
	};

	REGISTER_PROVIDER_BASE_TYPE(filesystem_paths_t, filesystem_paths_provider_base_t)

}

#include "global/component/data_common/filesystem_paths_provider/mockup.hpp"
#include "global/component/data_common/filesystem_paths_provider/windows_portable.hpp"
#include "global/component/data_common/filesystem_paths_provider/windows.hpp"

#endif