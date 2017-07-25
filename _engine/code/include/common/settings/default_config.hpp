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
		SETTINGS_SET(bool, game_has_console, true)
	SETTINGS_END()

	SETTINGS_START(config_t, release)
		SETTINGS_SET(bool, game_has_console, false)
	SETTINGS_END()
}

#endif