#ifndef ENGINE_VIRTUAL_PATH_STD
#define ENGINE_VIRTUAL_PATH_STD(...)
#endif

#ifndef ENGINE_ASSET_PATH_STD
#define ENGINE_ASSET_PATH_STD(...)
#endif

#ifndef ENGINE_PATH_RESOLVE_STD
#define ENGINE_PATH_RESOLVE_STD(...)
#endif

ENGINE_VIRTUAL_PATH_STD(config)
ENGINE_VIRTUAL_PATH_STD(log)
ENGINE_VIRTUAL_PATH_STD(keybinding)
ENGINE_VIRTUAL_PATH_STD(common)
ENGINE_VIRTUAL_PATH_STD(crash_dumps)
ENGINE_VIRTUAL_PATH_STD(editor)
ENGINE_VIRTUAL_PATH_STD(launcher)
ENGINE_VIRTUAL_PATH_STD(modules)
ENGINE_VIRTUAL_PATH_STD(saves)
ENGINE_VIRTUAL_PATH_STD(submodules)

ENGINE_ASSET_PATH_STD(common)
ENGINE_ASSET_PATH_STD(modules)
ENGINE_ASSET_PATH_STD(submodules)

ENGINE_PATH_RESOLVE_STD("*local_app_data*"_u, platform::resolve_directory(platform::directory_special_t::local_app_data))
ENGINE_PATH_RESOLVE_STD("*roaming_app_data*"_u, platform::resolve_directory(platform::directory_special_t::roaming_app_data))
ENGINE_PATH_RESOLVE_STD("*saves*"_u, platform::resolve_directory(platform::directory_special_t::saves))
ENGINE_PATH_RESOLVE_STD("*exe_path*"_u, platform::get_self_path())

#undef ENGINE_PATH_RESOLVE_STD
#undef ENGINE_ASSET_PATH_STD
#undef ENGINE_VIRTUAL_PATH_STD