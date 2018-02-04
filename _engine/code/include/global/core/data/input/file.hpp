#ifndef ENGINE_COMMON_UTILITY_DATA_INPUT_FILE_HPP
#define ENGINE_COMMON_UTILITY_DATA_INPUT_FILE_HPP
#pragma once

#include "global/core/data/input.hpp"
#include "global/core/vfs/filesystem.hpp"
#include "global/core/vfs/virtual_path.hpp"
#include "global/component/filesystem.hpp"

#include <cstdio>
#include <system_error>

namespace engine
{
	namespace data
	{

		class input_file_t final : public input_t
		{

		public:

			input_file_t(const virtual_path_t & virtual_path, std::shared_ptr<filesystem_t> filesystem, std::filesystem::path path) : input_t(virtual_path), input(filesystem->input(path))
			{

			}

			~input_file_t()
			{

			}


			void jump_to_begin() final
			{
				input->seek(0, engine::filesystem_t::file_seek_origin_t::begin);
			}

			void jump_to_end() final
			{
				input->seek(0, engine::filesystem_t::file_seek_origin_t::end);
			}

			void skip(int32_t pos) final
			{
				input->seek(pos, engine::filesystem_t::file_seek_origin_t::current);
			}
			void go_back(int32_t pos) final
			{
				input->seek(-pos, engine::filesystem_t::file_seek_origin_t::current);
			}
			uint32_t read(uint8_t * buffer, uint32_t size) final
			{
				return input->read(buffer, size);
			}
			bool is_eof() final
			{
				return input->is_eof();
			}

			uint32_t position() final
			{
				return input->tell();
			}

		private:

			std::shared_ptr<filesystem_t::file_input_t> input;
		};
	}
}
#endif