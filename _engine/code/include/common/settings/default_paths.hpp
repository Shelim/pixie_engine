#ifndef ENGINE_COMMON_DEFAULT_PATHS_HPP
#define ENGINE_COMMON_DEFAULT_PATHS_HPP
#pragma once

#include <SDL.h>
#include <cstdio>
#include <cstdlib>
#include "bootstrapper.hpp"
#include "component/logger.hpp"
#include "component/frame_notifier.hpp"
#include "utility/vfs/filesystem.hpp"
#include "utility/text/ustring.hpp"
#include "utility/vfs/virtual_path.hpp"

namespace engine
{
	SETTINGS_START(save_locatin_resolver_t, windows_normal)
		SETTINGS_SET(ustring_t, save_path_for_config, "*local_app_data*"_u)
		SETTINGS_SET(ustring_t, save_path_for_log, "*local_app_data*"_u)
		SETTINGS_SET(ustring_t, save_path_for_keybinding, "*roaming_app_data*"_u)
		SETTINGS_SET(ustring_t, save_path_for_common, "*local_app_data*/common"_u)
		SETTINGS_SET(ustring_t, save_path_for_crash_dumps, "*local_app_data*/crash_dump"_u)
		SETTINGS_SET(ustring_t, save_path_for_editor, "*local_app_data*/editor"_u)
		SETTINGS_SET(ustring_t, save_path_for_launcher, "*local_app_data*/launcher"_u)
		SETTINGS_SET(ustring_t, save_path_for_modules, "*local_app_data*/modules"_u)
		SETTINGS_SET(ustring_t, save_path_for_saves, "*saves*"_u)
		SETTINGS_SET(ustring_t, save_path_for_submodules, "*local_app_data*/submodules"_u)
	SETTINGS_END()

	SETTINGS_START(save_locatin_resolver_t, windows_portable)
		SETTINGS_SET(ustring_t, save_path_for_config, "*exe_path*"_u)
		SETTINGS_SET(ustring_t, save_path_for_log, "*exe_path*"_u)
		SETTINGS_SET(ustring_t, save_path_for_keybinding, "*exe_path*"_u)
		SETTINGS_SET(ustring_t, save_path_for_common, "*exe_path*/common"_u)
		SETTINGS_SET(ustring_t, save_path_for_crash_dumps, "*exe_path*/crash_dump"_u)
		SETTINGS_SET(ustring_t, save_path_for_editor, "*exe_path*/editor"_u)
		SETTINGS_SET(ustring_t, save_path_for_launcher, "*exe_path*/launcher"_u)
		SETTINGS_SET(ustring_t, save_path_for_modules, "*exe_path*/modules"_u)
		SETTINGS_SET(ustring_t, save_path_for_saves, "*exe_path*/saves"_u)
		SETTINGS_SET(ustring_t, save_path_for_submodules, "*exe_path*/submodules"_u)
	SETTINGS_END()

	SETTINGS_START(common_filenames_t, normal)
		SETTINGS_SET(std::filesystem::path, logger, "log_" XSTR(PIXIE_OUTPUT_UNIX_NAME) ".log")
	SETTINGS_END()
}

#endif