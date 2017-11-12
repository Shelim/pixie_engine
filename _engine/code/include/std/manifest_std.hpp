#ifndef ENGINE_MANIFEST_TYPE_STD
#define ENGINE_MANIFEST_TYPE_STD(...)
#endif

#ifndef ENGINE_MANIFEST_START_STD
#define ENGINE_MANIFEST_START_STD(...)
#endif

#ifndef ENGINE_MANIFEST_STD
#define ENGINE_MANIFEST_STD(...)
#endif

#ifndef ENGINE_MANIFEST_END_STD
#define ENGINE_MANIFEST_END_STD(...)
#endif

ENGINE_MANIFEST_TYPE_STD(common)
#define ENGINE_OS_STD(platform) ENGINE_MANIFEST_TYPE_STD(platform)
#include "std/os_std.hpp"

ENGINE_MANIFEST_START_STD(common)
ENGINE_MANIFEST_STD(common, full_name)
ENGINE_MANIFEST_END_STD()

ENGINE_MANIFEST_START_STD(windows)
ENGINE_MANIFEST_STD(windows, installer_filename)
ENGINE_MANIFEST_STD(windows, portable_filename)
ENGINE_MANIFEST_STD(windows, executable_filename)
ENGINE_MANIFEST_STD(windows, resources_company)
ENGINE_MANIFEST_STD(windows, resources_internal)
ENGINE_MANIFEST_STD(windows, resources_copyrights)
ENGINE_MANIFEST_STD(windows, resources_product)
ENGINE_MANIFEST_STD(windows, install_dir)
ENGINE_MANIFEST_STD(windows, install_shortcut)
ENGINE_MANIFEST_STD(windows, install_registry)
ENGINE_MANIFEST_STD(windows, game_app_dir)
ENGINE_MANIFEST_STD(windows, storage_registry)
ENGINE_MANIFEST_END_STD()

#undef ENGINE_MANIFEST_END_STD
#undef ENGINE_MANIFEST_STD
#undef ENGINE_MANIFEST_START_STD
#undef ENGINE_MANIFEST_TYPE_STD