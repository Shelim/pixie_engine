#ifndef ENGINE_COMMON_DEFAULT_COFNIG_HPP
#define ENGINE_COMMON_DEFAULT_COFNIG_HPP
#pragma once

#include <SDL.h>
#include <cstdio>
#include <cstdlib>
#include "global/component/logger.hpp"
#include "global/core/vfs/filesystem.hpp"
#include "utility/text/ustring.hpp"
#include "global/core/vfs/virtual_path.hpp"
#include "global/component/config.hpp"

namespace engine
{
	SETTINGS_START(config_t, debug)
		SETTINGS_SET(bool, local_test_setting, true)
	SETTINGS_END()

	SETTINGS_START(config_t, release)
		SETTINGS_SET(bool, local_test_setting, false)
	SETTINGS_END()

	SETTINGS_START(config_storage_t, normal)
		SETTINGS_SET(ustring_t, key_for_local_test_setting, "app_#1#_test_setting"_u)
	SETTINGS_END()
}

#endif