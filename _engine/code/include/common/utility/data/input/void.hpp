#ifndef ENGINE_COMMON_UTILITY_DATA_INPUT_VOID_HPP
#define ENGINE_COMMON_UTILITY_DATA_INPUT_VOID_HPP
#pragma once

#include "utility/data/input.hpp"
#include "utility/vfs/filesystem.hpp"

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

			void skip(int32_t pos) final
			{
				
			}
			void go_back(int32_t pos) final
			{
				
			}
			uint32_t read(uint8_t * buffer, uint32_t size) final
			{
				return 0;
			}
			bool is_eof() final
			{
				return true;
			}

		private:

		};
	}

}
#endif