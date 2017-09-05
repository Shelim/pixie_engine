#ifndef ENGINE_COMMON_UTILITY_DATA_INPUT_USTRING_HPP
#define ENGINE_COMMON_UTILITY_DATA_INPUT_USTRING_HPP
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

		class input_ustring_t final : public input_t
		{

		public:

			input_ustring_t(const virtual_path_t & virtual_path, const ustring_t & data) : input_t(virtual_path), data(data), pos(0)
			{
			}

			~input_ustring_t()
			{

			}


			void jump_to_begin() final
			{
				this->pos = 0;
			}

			void jump_to_end() final
			{
				this->pos = data.len() - 1;
			}

			void skip(int32_t pos) final
			{
				this->pos = std::max(0, std::min(static_cast<int32_t>(data.len()) - 1, this->pos + pos));
			}
			void go_back(int32_t pos) final
			{
				this->pos = std::max(0, std::min(static_cast<int32_t>(data.len()) - 1, this->pos - pos));
			}
			uint32_t read(uint8_t * buffer, uint32_t size) final
			{
				uint32_t len = std::min(data.len(), this->pos + size) - this->pos;
				memcpy(buffer, data.get_cstring() + this->pos, len);
				pos += len;
				return len;
			}
			bool is_eof() final
			{
				return pos >= data.len();
			}

			uint32_t position() final
			{
				return pos;
			}

		private:

			int32_t pos;

			ustring_t data;
		};
	}

}
#endif