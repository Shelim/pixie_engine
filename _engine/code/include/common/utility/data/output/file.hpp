#ifndef ENGINE_COMMON_UTILITY_DATA_OUTPUT_FILE_HPP
#define ENGINE_COMMON_UTILITY_DATA_OUTPUT_FILE_HPP
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

		class output_file_t final : public output_t
		{

		public:

			output_file_t(const virtual_path_t & virtual_path, const std::filesystem::path & physical_path) : output_t(virtual_path), physical_path(physical_path)
			{
				std::error_code ec;

				std::filesystem::path dir = physical_path;
				dir.remove_filename();
				if(!is_directory(dir, ec))
					create_directories(dir, ec);

				stream.open(physical_path, std::ios_base::out | std::ios_base::binary);
			}

			~output_file_t()
			{
				stream.close();
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
			}

		private:

			std::ofstream stream;

			std::filesystem::path physical_path;
		};
	}
}
#endif