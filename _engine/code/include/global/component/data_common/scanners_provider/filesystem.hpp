
#ifndef ENGINE_COMPONENT_DATA_COMMON_SCANNERS_PROVIDER_FILESYSTEM_HPP
#define ENGINE_COMPONENT_DATA_COMMON_SCANNERS_PROVIDER_FILESYSTEM_HPP
#pragma once

#if PIXIE_WINDOWS

#include "global/component/data_common/scanners_provider.hpp"
#include "global/component/data_common/filesystem_paths_provider.hpp"
#include "global/core/data/provider_actual.hpp"
#include "global/core/data/scanner/directory.hpp"
#include "utility/pattern/class_settings.hpp"

namespace engine
{
    class filesystem_scanners_t
    {

    };
	
	SETTINGS_TABLE_START(filesystem_scanners_t)

	#define ENGINE_VIRTUAL_PATH_IMPL(type) SETTINGS_TABLE_ENTRY(bool, is_##type##_using_safe_output)
	#define ENGINE_VIRTUAL_PATH_DEF(...) DEFINE_TYPE_PASS(ENGINE_VIRTUAL_PATH_IMPL, __VA_ARGS__)
	#include "def/virtual_path.def"

	SETTINGS_TABLE_END()

	class data_scanners_provider_filesystem_t : public data_scanners_provider_base_t
	{

	public:

        data_scanners_provider_filesystem_t(std::shared_ptr<filesystem_t> filesystem, std::unique_ptr<holder_t<filesystem_paths_t> > filesystem_paths_provider, std::unique_ptr<settings_t<filesystem_scanners_t>> settings) : filesystem(filesystem), filesystem_paths_provider(std::move(filesystem_paths_provider)), settings(std::move(settings))
        {

        }

        void add_scanners(data::scanners_t & scanners) final
        {  
            for(std::size_t i = 0; i < value_of(virtual_path_t::type_t::count); i++)
            {
                virtual_path_t vp(""_u, static_cast<virtual_path_t::type_t>(i));

                auto read_path = filesystem_paths_provider->get_provider()->get_read_paths(vp);
                auto write_path = filesystem_paths_provider->get_provider()->get_write_paths(vp);

                for(auto & path : read_path)
                {
                    data::provider_actual_file_t::read_only_t read_only = data::provider_actual_file_t::read_only_t::yes;
                    if(std::find(write_path.begin(), write_path.end(), path) != write_path.end())
                        read_only = data::provider_actual_file_t::read_only_t::no;

                    switch(static_cast<virtual_path_t::type_t>(i))
                    {

#define ENGINE_VIRTUAL_PATH_IMPL(type) case virtual_path_t::type_t::type: scanners.push_back(std::make_unique<data::scanner_directory_t>(filesystem, vp, path, read_only, settings->get()->is_##type##_using_safe_output() ? data::provider_actual_file_t::use_safe_output_t::yes : data::provider_actual_file_t::use_safe_output_t::no)); break;		
#define ENGINE_VIRTUAL_PATH_DEF(...) DEFINE_TYPE_PASS(ENGINE_VIRTUAL_PATH_IMPL, __VA_ARGS__)
#include "def/virtual_path.def"	

                    }
                }
            }
        }

    private:

        std::shared_ptr<filesystem_t> filesystem;
        std::unique_ptr<holder_t<filesystem_paths_t> > filesystem_paths_provider;
        std::unique_ptr<settings_t<filesystem_scanners_t>> settings;
	};

}

#endif
#endif