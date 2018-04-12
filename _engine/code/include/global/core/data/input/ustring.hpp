#ifndef ENGINE_COMMON_UTILITY_DATA_INPUT_USTRING_HPP
#define ENGINE_COMMON_UTILITY_DATA_INPUT_USTRING_HPP
#pragma once

#include "global/core/data/input.hpp"
#include "global/core/vfs/filesystem.hpp"
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

			void skip(int64_t pos) final
			{
				this->pos = std::max(0ll, std::min(static_cast<int64_t>(data.len()) - 1, static_cast<int64_t>(this->pos) + pos));
			}
			void go_back(int64_t pos) final
			{
				this->pos = std::max(0ll, std::min(static_cast<int64_t>(data.len()) - 1, static_cast<int64_t>(this->pos) - pos));
			}
			uint64_t read(uint8_t * buffer, uint64_t size) final
			{
				uint64_t len = std::min(data.len(), static_cast<int64_t>(this->pos) + size) - this->pos;
				memcpy(buffer, data.get_cstring() + this->pos, len);
				pos += len;
				return len;
			}
			bool is_eof() final
			{
				return pos >= data.len();
			}

			uint64_t position() final
			{
				return pos;
			}

		private:

			int64_t pos;

			ustring_t data;
		};
	}

}
#endif