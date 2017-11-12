#ifndef ENGINE_COMMON_UTILITY_DATA_OUTPUT_FILE_SAFE_HPP
#define ENGINE_COMMON_UTILITY_DATA_OUTPUT_FILE_SAFE_HPP
#pragma once

#include "core/data/output.hpp"
#include "core/vfs/filesystem.hpp"
#include "core/vfs/virtual_path.hpp"
#include "component/filesystem.hpp"

#include <fstream>
#include <system_error>

namespace engine
{
	namespace data
	{

		class output_file_safe_t final : public output_t
		{

		public:

			output_file_safe_t(const virtual_path_t & virtual_path, std::shared_ptr<filesystem_t> filesystem, std::filesystem::path path) : output_t(virtual_path), filesystem(filesystem), target_file(path)
			{
				tmp_file = filesystem->construct_temp_file();
				output = filesystem->output(tmp_file);
			}

			~output_file_safe_t()
			{
				output->flush();
				output = nullptr; // Force close output
				filesystem->move_file(tmp_file, target_file);
			}

			uint32_t write(const uint8_t * buffer, uint32_t size) final
			{
				return output->write(buffer, size);
			}
			
			void force_flush() final
			{
				output->flush();
				filesystem->copy_file(tmp_file, target_file);
			}

		private:

			std::shared_ptr<filesystem_t> filesystem;
			std::filesystem::path tmp_file;
			std::filesystem::path target_file;
			std::shared_ptr<filesystem_t::file_output_t> output;
		};
	}
}
#endif