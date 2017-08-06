#ifndef ENGINE_COMMON_DEFAULT_SCANNERS_HPP
#define ENGINE_COMMON_DEFAULT_SCANNERS_HPP
#pragma once

#include <SDL.h>
#include <cstdio>
#include <cstdlib>
#include "bootstrapper.hpp"
#include "component/logger.hpp"
#include "component/frame_notifier.hpp"
#include "utility/vfs/filesystem.hpp"
#include "utility/text/ustring.hpp"
#include "utility/vfs/virtual_path.hpp"
#include "utility/data/scanners.hpp"
#include "utility/data/scanner.hpp"
#include "utility/data/scanner/directory.hpp"
#include "provider/scanners.hpp"
#include <memory>

namespace engine
{
	SETTINGS_START(scanners_provider_t, windows_normal)
		SETTINGS_SET(engine::data::scanners_t::collection_t, scanners_collection, {
				std::make_unique<engine::data::scanner_directory_t>(engine::virtual_path_t(""_u, engine::virtual_path_t::type_t::common), )
						})
	SETTINGS_END()

	SETTINGS_START(scanners_provider_t, windows_portable)
	SETTINGS_END()
}

#endif