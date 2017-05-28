#ifndef ENGINE_COMMON_DATA_PROVIDER_ACTUAL_FILE_HPP
#define ENGINE_COMMON_DATA_PROVIDER_ACTUAL_FILE_HPP
#pragma once

#include "common/data/provider.hpp"
#include "common/data/input_file.hpp"
#include "common/data/output_file.hpp"
#include "common/data/output_file_safe.hpp"

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
				return std::move(platform_construct_output_local());
			}
			std::unique_ptr<output_t> platform_construct_output_local();

			std::filesystem::path physical_path;
			bool read_only;
		};
	}
}

#endif