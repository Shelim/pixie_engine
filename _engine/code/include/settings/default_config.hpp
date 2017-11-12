#ifndef ENGINE_COMMON_DEFAULT_COFNIG_HPP
#define ENGINE_COMMON_DEFAULT_COFNIG_HPP
#pragma once

#include <SDL.h>
#include <cstdio>
#include <cstdlib>
#include "component/logger.hpp"
#include "component/renderer_status.hpp"
#include "core/vfs/filesystem.hpp"
#include "utility/text/ustring.hpp"
#include "core/vfs/virtual_path.hpp"
#include "component/config.hpp"

namespace engine
{
	SETTINGS_START(config_t, debug)
		SETTINGS_SET(bool, cfg_has_terminal, true)
	SETTINGS_END()

	SETTINGS_START(config_t, release)
		SETTINGS_SET(bool, cfg_has_terminal, false)
	SETTINGS_END()
}

#endif