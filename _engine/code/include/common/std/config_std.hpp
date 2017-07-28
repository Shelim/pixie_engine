#ifndef GAME_CONFIG_STD
#define GAME_CONFIG_STD(...)
#endif

#define APP_STD(app_name) GAME_CONFIG_STD(bool, app_name##_has_terminal)
#include "std/app_list_std.hpp"

#include "std/app_config_std.hpp"

#undef GAME_CONFIG_STD