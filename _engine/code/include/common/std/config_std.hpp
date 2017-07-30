#ifndef GAME_CONFIG_GLOBAL_STD
#define GAME_CONFIG_GLOBAL_STD(...)
#endif

#ifndef GAME_CONFIG_LOCAL_STD
#define GAME_CONFIG_LOCAL_STD(...)
#endif

#ifndef GAME_CONFIG_STD
#define GAME_CONFIG_STD(...)
#endif

GAME_CONFIG_STD(bool, has_terminal)

#define APP_CONFIG_GLOBAL_STD(type, name, def_debug, def_release) GAME_CONFIG_GLOBAL_STD(type, name)
#define APP_CONFIG_LOCAL_STD(type, app, name, def_debug, def_release) GAME_CONFIG_LOCAL_STD(type, app, name)
#define APP_CONFIG_STD(type, name, def_debug, def_release) GAME_CONFIG_STD(type, name)
#include "std/app_config_std.hpp"

#undef GAME_CONFIG_STD
#undef GAME_CONFIG_LOCAL_STD
#undef GAME_CONFIG_GLOBAL_STD