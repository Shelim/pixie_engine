#ifndef GAME_ENVIRONMENT_INFO_STD
#define GAME_ENVIRONMENT_INFO_STD(...)
#endif

GAME_ENVIRONMENT_INFO_STD(game_name, "Game Name"_u)
GAME_ENVIRONMENT_INFO_STD(app_type, "App Type"_u)
GAME_ENVIRONMENT_INFO_STD(game_version, "Game version"_u)
GAME_ENVIRONMENT_INFO_STD(launched_time, "Launched at"_u)
GAME_ENVIRONMENT_INFO_STD(os_name, "OS"_u)
GAME_ENVIRONMENT_INFO_STD(os_ver, "OS version"_u)
GAME_ENVIRONMENT_INFO_STD(cpu_cores, "CPU cores"_u)
GAME_ENVIRONMENT_INFO_STD(cpu_cache, "CPU L1 cache size"_u)
GAME_ENVIRONMENT_INFO_STD(cpu_features, "CPU features"_u)
GAME_ENVIRONMENT_INFO_STD(sdl_version_compiled, "SDL version (compiled)"_u)
GAME_ENVIRONMENT_INFO_STD(sdl_version_linked, "SDL version (dll)"_u)
GAME_ENVIRONMENT_INFO_STD(sdl_revision_compiled, "SDL revision (compiled)"_u)
GAME_ENVIRONMENT_INFO_STD(sdl_revision_linked, "SDL revision (dll)"_u)


#undef GAME_ENVIRONMENT_INFO_STD