#ifndef ENGINE_COMMON_UTILITY_DATA_OUTPUT_VOID_HPP
#define ENGINE_COMMON_UTILITY_DATA_OUTPUT_VOID_HPP
#pragma once

#include "utility/data/output.hpp"
#include "utility/vfs/filesystem.hpp"

#include <fstream>
#include <system_error>

namespace engine
{
	namespace data
	{

		class output_void_t final : public output_t
		{

		public:

			output_void_t(const virtual_path_t & virtual_path) : output_t(virtual_path)
			{

			}

			~output_void_t()
			{

			}

			uint32_t write(const uint8_t * buffer, uint32_t size) final
			{
				return size;
			}

		private:

		};
	}
}
#endif