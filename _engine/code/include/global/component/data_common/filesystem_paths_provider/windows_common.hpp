
#ifndef ENGINE_COMPONENT_DATA_COMMON_FILESYSTEM_PATHS_PROVIDER_WINDOWS_COMMON_HPP
#define ENGINE_COMPONENT_DATA_COMMON_FILESYSTEM_PATHS_PROVIDER_WINDOWS_COMMON_HPP
#pragma once

#if PIXIE_WINDOWS

#include "global/component/data_common/filesystem_paths_provider.hpp"
#include "global/component/program_args.hpp"
#include "global/core/manifest/manifest.hpp"
#include <windows.h>
#undef min
#undef max
namespace engine
{

	class filesystem_paths_windows_common_t
	{

	public:

        filesystem_paths_windows_common_t(std::shared_ptr<program_args_t> program_args, std::shared_ptr<manifest_windows_t> manifest_windows, std::shared_ptr<manifest_common_t> manifest_common) : program_args(program_args), manifest_windows(manifest_windows), manifest_common(manifest_common)
        {

        }

        std::filesystem::path get_self_path()
        {
            ustring_t path = program_args->get_program_args().get_executable_path();
            return path.get_cstring();
        }

        enum class directory_special_t
        {
            local_app_data,
            roaming_app_data,
            saves
        };
        
        std::filesystem::path resolve_path(directory_special_t directory)
        {
            ustring_t ret;

            switch (directory)
            {
            case directory_special_t::local_app_data: ret = engine::ustring_t::from_wide(_wgetenv(L"LOCALAPPDATA")); ret.append('\\').append(manifest_windows->get_app_data_sub_dir()).append('\\'); break;
            case directory_special_t::roaming_app_data: ret = engine::ustring_t::from_wide(_wgetenv(L"APPDATA")); ret.append('\\').append(manifest_windows->get_app_data_sub_dir()).append('\\'); break;
            case directory_special_t::saves: ret = engine::ustring_t::from_wide(_wgetenv(L"USERPROFILE")); ret.append("\\Saved Games\\"_u).append(manifest_common->get_full_name()).append('\\'); break;
            }

            return ret.get_cstring();
        }

        std::filesystem::path get_path_portable(const virtual_path_t & filename)
        {
            auto ret = get_self_path_with_type(filename.get_type());
            ret /= filename.get_path().get_cstring();
            return ret;
        }  

        std::filesystem::path get_path_user(const virtual_path_t & filename)
        {
            std::filesystem::path ret;

            switch(filename.get_type())
            {
                case virtual_path_t::type_t::config: ret = resolve_path(directory_special_t::local_app_data); ret /= "config"; break;
                case virtual_path_t::type_t::common: ret = resolve_path(directory_special_t::local_app_data); ret /= "content/common"; break;
                case virtual_path_t::type_t::crash_dumps: ret = resolve_path(directory_special_t::local_app_data); ret /= "crash_dump"; break;
                case virtual_path_t::type_t::keybinding: ret = resolve_path(directory_special_t::roaming_app_data); ret /= "keybinding"; break;
                case virtual_path_t::type_t::saves: ret = resolve_path(directory_special_t::saves); break;
                case virtual_path_t::type_t::logs: ret = resolve_path(directory_special_t::local_app_data); ret /= "logs"; break;

#define GAME_APP_IMPL(app) case virtual_path_t::type_t::app_##app: ret = resolve_path(directory_special_t::local_app_data); ret /= "content/" #app; break;		
#define GAME_APP_DEF(...) DEFINE_TYPE_PASS(GAME_APP_IMPL, __VA_ARGS__)
#include "def/app.def"	

                default: ret = resolve_path(directory_special_t::local_app_data); ret /= "unknown"; break;
            }

            ret /= filename.get_path().get_cstring();

            return ret;
        }      

    private:

        std::shared_ptr<program_args_t> program_args;
        std::shared_ptr<manifest_windows_t> manifest_windows;
        std::shared_ptr<manifest_common_t> manifest_common;

        std::filesystem::path get_self_path_with_type(virtual_path_t::type_t type)
        {
            std::filesystem::path ret = get_self_path();
            switch(type)
            {
                case virtual_path_t::type_t::config: ret /= "config"; break;
                case virtual_path_t::type_t::common: ret /= "content/common"; break;
                case virtual_path_t::type_t::crash_dumps: ret /= "crash_dump"; break;
                case virtual_path_t::type_t::keybinding: ret /= "keybinding"; break;
                case virtual_path_t::type_t::saves: ret /= "saves"; break;
                case virtual_path_t::type_t::logs: ret /= "logs"; break;

#define GAME_APP_IMPL(app) case virtual_path_t::type_t::app_##app: ret /= "content/" #app; break;		
#define GAME_APP_DEF(...) DEFINE_TYPE_PASS(GAME_APP_IMPL, __VA_ARGS__)
#include "def/app.def"	

                default: ret /= "unknown"; break;
            }
            return ret;
        }


	};

}

#endif
#endif