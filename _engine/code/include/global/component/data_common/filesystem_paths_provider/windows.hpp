
#ifndef ENGINE_COMPONENT_DATA_COMMON_FILESYSTEM_PATHS_PROVIDER_WINDOWS_HPP
#define ENGINE_COMPONENT_DATA_COMMON_FILESYSTEM_PATHS_sPROVIDER_WINDOWS_HPP
#pragma once

#if PIXIE_WINDOWS

#include "global/component/data_common/filesystem_paths_provider.hpp"
#include "global/component/program_args.hpp"
#include "global/core/manifest/manifest.hpp"
#include "global/component/data_common/filesystem_paths_provider/windows_common.hpp"

namespace engine
{

	class filesystem_paths_provider_windows_t : public filesystem_paths_provider_base_t
	{

	public:

        filesystem_paths_provider_windows_t(std::shared_ptr<filesystem_paths_windows_common_t> common) : common(common)
        {

        }

        paths_t get_read_paths(const virtual_path_t & filename) final
        {
            paths_t ret;
            ret.push_back(common->get_path_portable(filename));
            ret.push_back(common->get_path_user(filename));
            return ret;
        }

        paths_t get_write_paths(const virtual_path_t & filename) final
        {
            paths_t ret;
            ret.push_back(common->get_path_user(filename));
            return ret;
        }

        std::filesystem::path get_path_for_new_file(const virtual_path_t & filename) final
        {
            return common->get_path_user(filename);
        }

    private:

        std::shared_ptr<filesystem_paths_windows_common_t> common;

	};

}

#endif

#endif