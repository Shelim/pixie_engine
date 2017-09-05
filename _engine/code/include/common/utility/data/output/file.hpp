#ifndef ENGINE_COMMON_UTILITY_DATA_OUTPUT_FILE_HPP
#define ENGINE_COMMON_UTILITY_DATA_OUTPUT_FILE_HPP
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

				file = platform::fopen(physical_path, platform::file_mode_t::write);
			}

			~output_file_t()
			{
				platform::fclose(file);
			}

			uint32_t write(const uint8_t * buffer, uint32_t size) final
			{
				return platform::fwrite(buffer, size, file);
			}
			
			void force_flush() final
			{
				platform::fflush(file);
			}

		private:

			platform::file_t file;

			std::filesystem::path physical_path;
		};
	}
}
#endif