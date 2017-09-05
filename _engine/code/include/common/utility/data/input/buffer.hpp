#ifndef ENGINE_COMMON_UTILITY_DATA_INPUT_BUFFER_HPP
#define ENGINE_COMMON_UTILITY_DATA_INPUT_BUFFER_HPP
#pragma once

#include "utility/data/input.hpp"
#include "utility/vfs/filesystem.hpp"
#include "utility/text/ustring.hpp"

#include <fstream>
#include <system_error>
#include <algorithm>

namespace engine
{
	namespace data
	{

		class input_buffer_t final : public input_t
		{

		public:

			input_buffer_t(const virtual_path_t & virtual_path, const buffer_t & data) : input_t(virtual_path), data(data), pos(0)
			{
			}

			~input_buffer_t()
			{

			}

			void jump_to_begin() final
			{
				this->pos = 0;
			}

			void jump_to_end() final
			{
				this->pos = static_cast<int32_t>(data.size()) - 1;
			}

			void skip(int32_t pos) final
			{
				this->pos = std::max(0, std::min(static_cast<int32_t>(data.size()) - 1, this->pos + pos));
			}
			void go_back(int32_t pos) final
			{
				this->pos = std::max(0, std::min(static_cast<int32_t>(data.size()) - 1, this->pos - pos));
			}
			uint32_t read(uint8_t * buffer, uint32_t size) final
			{
				uint32_t len = std::min(data.size(), this->pos + size) - this->pos;
				memcpy(buffer, &data[this->pos], len);
				this->pos += len;
				return len;
			}
			bool is_eof() final
			{
				return pos >= data.size();
			}

			uint32_t position() final
			{
				return pos;
			}

		private:

			int32_t pos;

			buffer_t data;
		};
	}

}
#endif