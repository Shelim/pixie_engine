#ifndef ENGINE_COMMON_UTILITY_DATA_INPUT_VOID_HPP
#define ENGINE_COMMON_UTILITY_DATA_INPUT_VOID_HPP
#pragma once

#include "global/core/data/input.hpp"
#include "global/core/vfs/filesystem.hpp"

#include <fstream>
#include <system_error>

namespace engine
{
	namespace data
	{
		class input_void_t final : public input_t
		{

		public:

			input_void_t(const virtual_path_t & virtual_path) : input_t(virtual_path)
			{

			}

			~input_void_t()
			{

			}

			void jump_to_begin() final
			{

			}

			void jump_to_end() final
			{

			}

			void skip(int64_t pos) final
			{
				
			}
			void go_back(int64_t pos) final
			{
				
			}
			uint64_t read(uint8_t * buffer, uint64_t size) final
			{
				return 0;
			}
			bool is_eof() final
			{
				return true;
			}

			uint64_t position() final
			{
				return 0;
			}

		private:

		};
	}

}
#endif