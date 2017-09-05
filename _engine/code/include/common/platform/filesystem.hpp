#ifndef ENGINE_COMMON_PLATFORM_FILESYSTEM_HPP
#define ENGINE_COMMON_PLATFORM_FILESYSTEM_HPP

#include "utility/vfs/filesystem.hpp"
#include "utility/text/ustring.hpp"

namespace engine
{

	namespace platform
	{

		void ensure_directory_exits(std::filesystem::path path);

		typedef void* file_t;

		enum class file_mode_t
		{
			read,
			write
		};

		file_t fopen(const std::filesystem::path & path, file_mode_t mode);
		void fclose(file_t file);
		std::size_t fread(uint8_t * buffer, std::size_t size, file_t file);
		std::size_t fwrite(const uint8_t * buffer, std::size_t size, file_t file);
		std::size_t ftell(file_t file);
		bool is_eof(file_t file);
		void fflush(file_t file);

		enum class file_seek_origin_t
		{
			begin,
			end,
			current
		};

		void fseek(file_t file, int32_t offset, file_seek_origin_t origin);
	}

}

#endif