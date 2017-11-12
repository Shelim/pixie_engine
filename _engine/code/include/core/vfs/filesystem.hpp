#ifndef ENGINE_COMMON_UTILITY_VFS_FILESYSTEM_HPP
#define ENGINE_COMMON_UTILITY_VFS_FILESYSTEM_HPP
#pragma once

#include <filesystem>

#ifdef _MSC_VER
#if _MSC_VER >= 1900
namespace std
{
	namespace filesystem = std::experimental::filesystem;
}
#endif
#endif

namespace engine
{
	typedef	std::vector<std::filesystem::path> paths_t;
}


#endif