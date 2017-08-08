#ifndef ENGINE_PROVIDER_COMMON_FILENAMES_HPP
#define ENGINE_PROVIDER_COMMON_FILENAMES_HPP
#pragma once

#include "utility/pattern/class_settings.hpp"
#include "utility/vfs/filesystem.hpp"
#include "utility/platform/compilation.hpp"

namespace engine
{
	struct common_filenames_t
	{

	};

	SETTINGS_TABLE_START(common_filenames_t)
		SETTINGS_TABLE_ENTRY(std::filesystem::path, logger)
	SETTINGS_TABLE_END()

	class common_filenames_provider_t
	{

	public:

		common_filenames_provider_t(std::unique_ptr<settings_t<common_filenames_t>> common_filenames) : common_filenames(std::move(common_filenames))
		{

		}

		auto get()
		{
			return common_filenames->get();
		}

	private:

		std::unique_ptr<settings_t<common_filenames_t>> common_filenames;
	};
}

#endif