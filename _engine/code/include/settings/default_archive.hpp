#ifndef ENGINE_COMMON_DEFAULT_ARCHIVE_HPP
#define ENGINE_COMMON_DEFAULT_ARCHIVE_HPP
#pragma once

#include "bootstrapper.hpp"
#include "global/component/data_archives.hpp"

namespace engine
{

	SETTINGS_START(data_archives_t, normal)

		SETTINGS_SET(ustring_collection_t, allowed_extensions_for_zip, ustring_collection_t({ ".zip"_u }))

	SETTINGS_END()

}

#endif