#ifndef ENGINE_COMMON_DEFAULT_SCANNERS_HPP
#define ENGINE_COMMON_DEFAULT_SCANNERS_HPP
#pragma once

#include <cstdio>
#include <cstdlib>
#include "global/component/data_common/scanners_provider.hpp"

namespace engine
{

	SETTINGS_START(filesystem_scanners_t, normal)

		SETTINGS_SET(bool, is_config_using_safe_output, true)
		SETTINGS_SET(bool, is_common_using_safe_output, true)
		SETTINGS_SET(bool, is_crash_dumps_using_safe_output, false)
		SETTINGS_SET(bool, is_keybinding_using_safe_output, true)
		SETTINGS_SET(bool, is_saves_using_safe_output, true)
		SETTINGS_SET(bool, is_logs_using_safe_output, true)
		SETTINGS_SET(bool, is_unknown_using_safe_output, false)

#define GAME_APP_IMPL(app) SETTINGS_SET(bool, is_app_##app##_using_safe_output, true)
#define GAME_APP_DEF(...) DEFINE_TYPE_PASS(GAME_APP_IMPL, __VA_ARGS__)
#include "def/app.def"	

	SETTINGS_END()

}

#endif