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

#include "std/app_config_std.hpp"

#undef GAME_CONFIG_STD
#undef GAME_CONFIG_LOCAL_STD
#undef GAME_CONFIG_GLOBAL_STD