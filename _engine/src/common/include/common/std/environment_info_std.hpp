#ifndef GAME_ENVIRONMENT_INFO_STD
#define GAME_ENVIRONMENT_INFO_STD(...)
#endif

GAME_ENVIRONMENT_INFO_STD(game_name, _U("Game Name"))
GAME_ENVIRONMENT_INFO_STD(app_type, _U("App Type"))
GAME_ENVIRONMENT_INFO_STD(game_version, _U("Game version"))
GAME_ENVIRONMENT_INFO_STD(launched_time, _U("Launched at"))
GAME_ENVIRONMENT_INFO_STD(os_name, _U("OS"))
GAME_ENVIRONMENT_INFO_STD(os_ver, _U("OS version"))
GAME_ENVIRONMENT_INFO_STD(cpu_cores, _U("CPU cores"))
GAME_ENVIRONMENT_INFO_STD(cpu_cache, _U("CPU L1 cache size"))
GAME_ENVIRONMENT_INFO_STD(cpu_features, _U("CPU features"))
GAME_ENVIRONMENT_INFO_STD(sdl_version_compiled, _U("SDL version (compiled)"))
GAME_ENVIRONMENT_INFO_STD(sdl_version_linked, _U("SDL version (dll)"))
GAME_ENVIRONMENT_INFO_STD(sdl_revision_compiled, _U("SDL revision (compiled)"))
GAME_ENVIRONMENT_INFO_STD(sdl_revision_linked, _U("SDL revision (dll)"))


#undef GAME_ENVIRONMENT_INFO_STD