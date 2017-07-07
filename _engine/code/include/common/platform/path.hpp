#ifndef ENGINE_COMMON_PLATFORM_PATH_HPP
#define ENGINE_COMMON_PLATFORM_PATH_HPP

#include "utility/vfs/filesystem.hpp"
#include "utility/text/ustring.hpp"

namespace engine
{

	namespace platform
	{

		ustring_t get_self_path();
		void ensure_directory_exits(std::filesystem::path path);

		enum class directory_special_t
		{
			local_app_data,
			roaming_app_data,
			saves
		};

		ustring_t resolve_directory(directory_special_t directory);
	}

}

#endif