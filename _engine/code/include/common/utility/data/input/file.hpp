#ifndef ENGINE_COMMON_UTILITY_DATA_INPUT_FILE_HPP
#define ENGINE_COMMON_UTILITY_DATA_INPUT_FILE_HPP
#pragma once

#include "utility/data/input.hpp"
#include "utility/vfs/filesystem.hpp"
#include "utility/vfs/virtual_path.hpp"
#include "platform/filesystem.hpp"

#include <fstream>
#include <system_error>

namespace engine
{
	namespace data
	{

		class input_file_t final : public input_t
		{

		public:

			input_file_t(const virtual_path_t & virtual_path, const std::filesystem::path & physical_path) : input_t(virtual_path), physical_path(physical_path)
			{
				file = platform::fopen(physical_path, platform::file_mode_t::read);
			}

			~input_file_t()
			{
				platform::fclose(file);
			}


			void jump_to_begin() final
			{
				platform::fseek(file, 0, platform::file_seek_origin_t::begin);
			}

			void jump_to_end() final
			{
				platform::fseek(file, 0, platform::file_seek_origin_t::end);
			}

			void skip(int32_t pos) final
			{
				platform::fseek(file, pos, platform::file_seek_origin_t::current);
			}
			void go_back(int32_t pos) final
			{
				platform::fseek(file, -pos, platform::file_seek_origin_t::current);
			}
			uint32_t read(uint8_t * buffer, uint32_t size) final
			{
				return platform::fread(buffer, size, file);
			}
			bool is_eof() final
			{
				return platform::is_eof(file);
			}

			uint32_t position() final
			{
				return platform::ftell(file);
			}

		private:

			platform::file_t file;
			std::filesystem::path physical_path;
		};
	}
}
#endif