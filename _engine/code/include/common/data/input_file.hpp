#ifndef ENGINE_COMMON_DATA_INPUT_FILE_HPP
#define ENGINE_COMMON_DATA_INPUT_FILE_HPP
#pragma once

#include "common/data/input.hpp"
#include "common/filesystem.hpp"

#include <fstream>
#include <system_error>

namespace engine
{
	namespace data
	{

		class input_file_t final : public input_t
		{

		public:

			input_file_t(const virtual_path_t & virtual_path, const std::filesystem::path & physical_path) : input_t(virtual_path), stream(physical_path, std::ios_base::in | std::ios_base::binary), physical_path(physical_path)
			{

			}

			~input_file_t()
			{

			}

			void skip(int32_t pos) final
			{
				stream.seekg(pos, std::ios_base::cur);
			}
			void go_back(int32_t pos) final
			{
				stream.seekg(-pos, std::ios_base::cur);
			}
			uint32_t read(uint8_t * buffer, uint32_t size) final
			{
				return stream.read(reinterpret_cast<char *>(buffer), size).gcount();
			}
			bool is_eof() final
			{
				return stream.eof();
			}

		private:

			std::ifstream stream;
			std::filesystem::path physical_path;
		};
	}
}
#endif