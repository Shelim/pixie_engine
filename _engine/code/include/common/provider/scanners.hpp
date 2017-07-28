#ifndef ENGINE_PROVIDER_SCANNERS_HPP
#define ENGINE_PROVIDER_SCANNERS_HPP
#pragma once

#include "utility/pattern/class_settings.hpp"
#include "utility/vfs/filesystem.hpp"
#include "utility/text/ustring.hpp"
#include "utility/vfs/virtual_path.hpp"
#include "utility/data/scanners.hpp"

namespace engine
{

	struct scanners_provider_settings_t
	{

	};


	SETTINGS_TABLE_START(scanners_provider_settings_t)
#define ENGINE_VIRTUAL_PATH_STD(name) SETTINGS_TABLE_ENTRY(ustring_collection_t, path_readonly_for_##name)
#include "std/virtual_path_std.hpp"
#define ENGINE_VIRTUAL_PATH_STD(name) SETTINGS_TABLE_ENTRY(ustring_collection_t, path_fullaccess_for_##name)
#include "std/virtual_path_std.hpp"
	SETTINGS_TABLE_END()


	class scanners_provider_t
	{

	public:

		scanners_provider_t(std::unique_ptr<settings_t<scanners_provider_settings_t>> scanners) : scanners(std::move(scanners))
		{

		}

		std::unique_ptr<engine::data::scanners_t::collection_t > construct_scanners_collection();

	private:

		std::unique_ptr<settings_t<scanners_provider_settings_t>> scanners;
	};


}

#endif