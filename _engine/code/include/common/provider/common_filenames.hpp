#ifndef ENGINE_PROVIDER_COMMON_FILENAMES_HPP
#define ENGINE_PROVIDER_COMMON_FILENAMES_HPP
#pragma once

#include "utility/pattern/class_settings.hpp"
#include "utility/vfs/filesystem.hpp"
#include "utility/pattern/compilation.hpp"

namespace engine
{
	struct common_filenames_t
	{

	};

	SETTINGS_TABLE_START(common_filenames_t)
		SETTINGS_TABLE_ENTRY(std::filesystem::path, logger, "log_" XSTR(PIXIE_OUTPUT_UNIX_NAME) ".log")
	SETTINGS_TABLE_END()

	class common_filenames_provider_t
	{

	public:

		common_filenames_provider_t(PTR_TO_SETTINGS_FOR(common_filenames_t) common_filenames) : common_filenames(std::move(common_filenames))
		{

		}

		auto get()
		{
			return common_filenames->get();
		}

	private:

		PTR_TO_SETTINGS_FOR(common_filenames_t) common_filenames;
	};
}

#endif