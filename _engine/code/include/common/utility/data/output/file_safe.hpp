#ifndef ENGINE_COMMON_UTILITY_DATA_OUTPUT_FILE_SAFE_HPP
#define ENGINE_COMMON_UTILITY_DATA_OUTPUT_FILE_SAFE_HPP
#pragma once

#include "utility/data/output.hpp"
#include "utility/vfs/filesystem.hpp"
#include "utility/vfs/virtual_path.hpp"

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

				stream.open(physical_path_tmp, std::ios_base::out | std::ios_base::binary);
			}

			~output_file_safe_t()
			{
				stream.close();

				std::error_code ec;
				std::filesystem::copy_file(physical_path_tmp, physical_path, std::filesystem::copy_options::overwrite_existing, ec);
				std::filesystem::remove(physical_path_tmp, ec);
			}

			uint32_t write(const uint8_t * buffer, uint32_t size) final
			{
				std::ofstream::pos_type pos1 = stream.tellp();
				if (pos1 < 0) return 0;

				stream.write(reinterpret_cast<const char *>(buffer), size);
					
				std::ofstream::pos_type pos2 = stream.tellp();
				if (pos1 < 0) return 0;

				return pos2 - pos1;
			}
			
			void force_flush() final
			{
				stream.flush();

				std::error_code ec;
				std::filesystem::copy_file(physical_path_tmp, physical_path, std::filesystem::copy_options::overwrite_existing, ec);
			}

		private:

			std::ofstream stream;

			std::filesystem::path physical_path_tmp;
			std::filesystem::path physical_path;
		};
	}
}
#endif