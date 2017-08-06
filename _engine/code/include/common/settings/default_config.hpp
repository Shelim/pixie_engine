#ifndef ENGINE_COMMON_DEFAULT_COFNIG_HPP
#define ENGINE_COMMON_DEFAULT_COFNIG_HPP
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
#include "component/config.hpp"

namespace engine
{

	SETTINGS_START(config_t, debug)
		SETTINGS_SET(bool, cfg_has_terminal, true)

#define APP_CONFIG_GLOBAL_STD(type, name, def_debug, def_release) SETTINGS_SET(type, global_##name, def_debug)
#define APP_CONFIG_LOCAL_STD(type, app, name, def_debug, def_release) SETTINGS_SET(type, app_##app##_##name, def_debug)
#define APP_CONFIG_STD(type, name, def_debug, def_release) SETTINGS_SET(type, cfg_##name, def_debug)
#include "std/app_config_std.hpp"

	SETTINGS_END()

	SETTINGS_START(config_t, release)
		SETTINGS_SET(bool, cfg_has_terminal, false)

#define APP_CONFIG_GLOBAL_STD(type, name, def_debug, def_release) SETTINGS_SET(type, global_##name, def_release)
#define APP_CONFIG_LOCAL_STD(type, app, name, def_debug, def_release) SETTINGS_SET(type, app_##app##_##name, def_release)
#define APP_CONFIG_STD(type, name, def_debug, def_release) SETTINGS_SET(type, cfg_##name, def_release)
#include "std/app_config_std.hpp"

	SETTINGS_END()
}

#endif