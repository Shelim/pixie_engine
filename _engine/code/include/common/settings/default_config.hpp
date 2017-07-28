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
#define APP_STD(app_name) SETTINGS_SET(bool, app_name##_has_terminal, true)
#include "std/app_list_std.hpp"
	SETTINGS_END()

	SETTINGS_START(config_t, release)
#define APP_STD(app_name) SETTINGS_SET(bool, app_name##_has_terminal, true)
#include "std/app_list_std.hpp"
	SETTINGS_END()
}

#endif