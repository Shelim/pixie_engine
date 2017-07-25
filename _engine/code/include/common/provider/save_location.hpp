#ifndef ENGINE_PROVIDER_SAVE_LOCATION_HPP
#define ENGINE_PROVIDER_SAVE_LOCATION_HPP
#pragma once

#include "utility/pattern/class_settings.hpp"
#include "utility/vfs/filesystem.hpp"
#include "utility/text/ustring.hpp"
#include "utility/vfs/virtual_path.hpp"

namespace engine
{

	struct save_location_resolver_t
	{

	};

	SETTINGS_TABLE_START(save_location_resolver_t)
#define ENGINE_VIRTUAL_PATH_STD(name) SETTINGS_TABLE_ENTRY(ustring_t, save_path_for_##name)
#include "std/virtual_path_std.hpp"
	SETTINGS_TABLE_END()


	class save_location_provider_t
	{

	public:

		save_location_provider_t(std::unique_ptr<settings_t<save_location_resolver_t>> path_resolver) : path_resolver(std::move(path_resolver))
		{

		}

		ustring_t get_save_path(const virtual_path_t & path, bool ensure_it_exists = true);

	private:

		std::unique_ptr<settings_t<save_location_resolver_t>> path_resolver;
	};

}

#endif