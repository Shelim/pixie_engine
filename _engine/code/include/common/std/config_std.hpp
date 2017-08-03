#ifndef ENGINE_CONFIG_GLOBAL_STD
#define ENGINE_CONFIG_GLOBAL_STD(...)
#endif

#ifndef ENGINE_CONFIG_LOCAL_STD
#define ENGINE_CONFIG_LOCAL_STD(...)
#endif

#ifndef ENGINE_CONFIG_STD
#define ENGINE_CONFIG_STD(...)
#endif

ENGINE_CONFIG_STD(bool, has_terminal)

#define APP_CONFIG_GLOBAL_STD(type, name, def_debug, def_release) ENGINE_CONFIG_GLOBAL_STD(type, name)
#define APP_CONFIG_LOCAL_STD(type, app, name, def_debug, def_release) ENGINE_CONFIG_LOCAL_STD(type, app, name)
#define APP_CONFIG_STD(type, name, def_debug, def_release) ENGINE_CONFIG_STD(type, name)
#include "std/app_config_std.hpp"

#undef ENGINE_CONFIG_STD
#undef ENGINE_CONFIG_LOCAL_STD
#undef ENGINE_CONFIG_GLOBAL_STD