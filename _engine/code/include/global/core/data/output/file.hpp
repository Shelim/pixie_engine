#ifndef ENGINE_COMMON_UTILITY_DATA_OUTPUT_FILE_HPP
#define ENGINE_COMMON_UTILITY_DATA_OUTPUT_FILE_HPP
#pragma once

#include "global/core/data/output.hpp"
#include "global/core/vfs/filesystem.hpp"
#include "global/core/vfs/virtual_path.hpp"
#include "global/component/filesystem.hpp"

#include <fstream>
#include <system_error>

namespace engine
{
	namespace data
	{

		class output_file_t final : public output_t
		{

		public:

			output_file_t(const virtual_path_t & virtual_path, std::shared_ptr<filesystem_t> filesystem, std::filesystem::path path) : output_t(virtual_path), output(filesystem->output(path))
			{

			}

			~output_file_t()
			{

			}

			uint32_t write(const uint8_t * buffer, uint32_t size) final
			{
				return output->write(buffer, size);
			}
			
			void force_flush() final
			{
				output->flush();
			}

		private:

			std::shared_ptr<filesystem_t::file_output_t> output;
		};
	}
}
#endif