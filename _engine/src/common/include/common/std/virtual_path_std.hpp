#ifndef GAME_VIRTUAL_PATH_STD
#define GAME_VIRTUAL_PATH_STD(...)
#endif

#ifndef GAME_ASSET_PATH_STD
#define GAME_ASSET_PATH_STD(...)
#endif

#ifndef GAME_CONFIG_VIRTUAL_PATH_STD
#define GAME_CONFIG_VIRTUAL_PATH_STD(...)
#endif

#ifndef GAME_LOGGER_VIRTUAL_PATH_STD
#define GAME_LOGGER_VIRTUAL_PATH_STD(...)
#endif

#ifndef GAME_KEYBINDING_VIRTUAL_PATH_STD
#define GAME_KEYBINDING_VIRTUAL_PATH_STD(...)
#endif

GAME_VIRTUAL_PATH_STD(config, local_app_data, "")
GAME_VIRTUAL_PATH_STD(log, local_app_data, "")
GAME_VIRTUAL_PATH_STD(keybinding, roaming_app_data, "")
GAME_VIRTUAL_PATH_STD(common, local_app_data, "common")
GAME_VIRTUAL_PATH_STD(crash_dumps, local_app_data, "crash_dumps")
GAME_VIRTUAL_PATH_STD(editor, local_app_data, "editor")
GAME_VIRTUAL_PATH_STD(launcher, local_app_data, "launcher")
GAME_VIRTUAL_PATH_STD(modules, local_app_data, "modules")
GAME_VIRTUAL_PATH_STD(saves, saves, "")
GAME_VIRTUAL_PATH_STD(submodules, local_app_data, "submodules")

GAME_ASSET_PATH_STD(common)
GAME_ASSET_PATH_STD(modules)
GAME_ASSET_PATH_STD(submodules)

GAME_CONFIG_VIRTUAL_PATH_STD("config.xml")
GAME_LOGGER_VIRTUAL_PATH_STD("log_" XSTR(PIXIE_OUTPUT_UNIX_NAME) ".xml")
GAME_KEYBINDING_VIRTUAL_PATH_STD("keybinding.xml")

#undef GAME_KEYBINDING_VIRTUAL_PATH_STD
#undef GAME_CONFIG_VIRTUAL_PATH_STD
#undef GAME_LOGGER_VIRTUAL_PATH_STD
#undef GAME_ASSET_PATH_STD
#undef GAME_VIRTUAL_PATH_STD