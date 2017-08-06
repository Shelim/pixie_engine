#ifndef ENGINE_COMMON_UTILITY_DATA_PROVIDER_ACTUAL_FILE_HPP
#define ENGINE_COMMON_UTILITY_DATA_PROVIDER_ACTUAL_FILE_HPP
#pragma once

#include "utility/data/provider.hpp"
#include "utility/data/input/file.hpp"
#include "utility/data/output/file.hpp"
#include "utility/data/output/file_safe.hpp"

namespace engine
{
	namespace data
	{
		class provider_actual_file_t final : public provider_actual_t
		{

		public:

			bool is_read_only() final
			{
				return read_only;
			}

			provider_actual_file_t(const virtual_path_t & virtual_path, const std::filesystem::path & physical_path, bool read_only = true) : provider_actual_t(virtual_path, std::filesystem::last_write_time(physical_path)), physical_path(physical_path), read_only(read_only)
			{

			}

		private:

			bool delete_target_local() final
			{
				std::error_code ec;
				return remove(physical_path, ec);
			}

			std::unique_ptr<input_t> construct_input_local() final
			{
				return std::make_unique<input_file_t>(get_virtual_path(), physical_path);
			}
			std::unique_ptr<output_t> construct_output_local() final
			{
				if (read_only) return nullptr;
				return std::make_unique<output_file_safe_t>(get_virtual_path(), physical_path);
			}

			std::filesystem::path physical_path;
			bool read_only;
		};
	}
}

#endif