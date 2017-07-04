#ifndef ENGINE_VIRTUAL_PATH_STD
#define ENGINE_VIRTUAL_PATH_STD(...)
#endif

#ifndef ENGINE_ASSET_PATH_STD
#define ENGINE_ASSET_PATH_STD(...)
#endif

#ifndef ENGINE_CONFIG_VIRTUAL_PATH_STD
#define ENGINE_CONFIG_VIRTUAL_PATH_STD(...)
#endif

#ifndef ENGINE_LOGGER_VIRTUAL_PATH_STD
#define ENGINE_LOGGER_VIRTUAL_PATH_STD(...)
#endif

#ifndef ENGINE_KEYBINDING_VIRTUAL_PATH_STD
#define ENGINE_KEYBINDING_VIRTUAL_PATH_STD(...)
#endif

ENGINE_VIRTUAL_PATH_STD(config, local_app_data, "")
ENGINE_VIRTUAL_PATH_STD(log, local_app_data, "")
ENGINE_VIRTUAL_PATH_STD(keybinding, roaming_app_data, "")
ENGINE_VIRTUAL_PATH_STD(common, local_app_data, "common")
ENGINE_VIRTUAL_PATH_STD(crash_dumps, local_app_data, "crash_dumps")
ENGINE_VIRTUAL_PATH_STD(editor, local_app_data, "editor")
ENGINE_VIRTUAL_PATH_STD(launcher, local_app_data, "launcher")
ENGINE_VIRTUAL_PATH_STD(modules, local_app_data, "modules")
ENGINE_VIRTUAL_PATH_STD(saves, saves, "")
ENGINE_VIRTUAL_PATH_STD(submodules, local_app_data, "submodules")

ENGINE_ASSET_PATH_STD(common)
ENGINE_ASSET_PATH_STD(modules)
ENGINE_ASSET_PATH_STD(submodules)

ENGINE_CONFIG_VIRTUAL_PATH_STD("config.xml")
ENGINE_LOGGER_VIRTUAL_PATH_STD("log_" XSTR(PIXIE_OUTPUT_UNIX_NAME) ".log")
ENGINE_KEYBINDING_VIRTUAL_PATH_STD("keybinding.xml")

#undef ENGINE_KEYBINDING_VIRTUAL_PATH_STD
#undef ENGINE_CONFIG_VIRTUAL_PATH_STD
#undef ENGINE_LOGGER_VIRTUAL_PATH_STD
#undef ENGINE_ASSET_PATH_STD
#undef ENGINE_VIRTUAL_PATH_STD