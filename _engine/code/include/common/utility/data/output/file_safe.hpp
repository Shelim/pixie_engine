#ifndef ENGINE_COMMON_UTILITY_DATA_OUTPUT_FILE_SAFE_HPP
#define ENGINE_COMMON_UTILITY_DATA_OUTPUT_FILE_SAFE_HPP
#pragma once

#include "utility/data/output.hpp"
#include "utility/vfs/filesystem.hpp"
#include "utility/vfs/virtual_path.hpp"
#include "platform/filesystem.hpp"

#include <fstream>
#include <system_error>

namespace engine
{
	namespace data
	{

		class output_file_safe_t final : public output_t
		{

		public:

			output_file_safe_t(const virtual_path_t & virtual_path, const std::filesystem::path & physical_path) : output_t(virtual_path), physical_path(physical_path)
			{
				std::error_code ec;

				std::filesystem::path dir = physical_path;
				dir.remove_filename();
				if(!is_directory(dir, ec))
					create_directories(dir, ec);

				physical_path_tmp = std::filesystem::temp_directory_path();
				physical_path_tmp /= XSTR(PIXIE_APP_UNIX_NAME);

				if (!is_directory(physical_path_tmp, ec))
					create_directories(physical_path_tmp, ec);

				physical_path_tmp /= physical_path.filename();
				physical_path_tmp.replace_extension(physical_path_tmp.extension().string() + ".tmp");

				file = platform::fopen(physical_path_tmp, platform::file_mode_t::write);
			}

			~output_file_safe_t()
			{
				platform::fclose(file);

				std::error_code ec;
				std::filesystem::copy_file(physical_path_tmp, physical_path, std::filesystem::copy_options::overwrite_existing, ec);
				std::filesystem::remove(physical_path_tmp, ec);
			}

			uint32_t write(const uint8_t * buffer, uint32_t size) final
			{
				return platform::fwrite(buffer, size, file);
			}
			
			void force_flush() final
			{
				platform::fflush(file);

				std::error_code ec;
				std::filesystem::copy_file(physical_path_tmp, physical_path, std::filesystem::copy_options::overwrite_existing, ec);
			}

		private:

			platform::file_t file;

			std::filesystem::path physical_path_tmp;
			std::filesystem::path physical_path;
		};
	}
}
#endif