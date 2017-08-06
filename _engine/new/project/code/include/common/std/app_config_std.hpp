#ifndef APP_CONFIG_GLOBAL_STD
#define APP_CONFIG_GLOBAL_STD(...)
#endif

#ifndef APP_CONFIG_LOCAL_STD
#define APP_CONFIG_LOCAL_STD(...)
#endif

#ifndef APP_CONFIG_STD
#define APP_CONFIG_STD(...)
#endif

/*
APP_CONFIG_GLOBAL_STD(type, name, def_debug, def_release)
APP_CONFIG_LOCAL_STD(type, app, name, def_debug, def_release)
APP_CONFIG_STD(type, name, def_debug, def_release)
*/


#undef APP_CONFIG_STD
#undef APP_CONFIG_LOCAL_STD
#undef APP_CONFIG_GLOBAL_STD