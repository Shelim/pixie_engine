#ifndef ENGINE_COMMON_UTILITY_DATA_MANIPULATOR_ACTUAL_ZIP_HPP
#define ENGINE_COMMON_UTILITY_DATA_MANIPULATOR_ACTUAL_ZIP_HPP
#pragma once

#include "global/core/data/provider_actual.hpp"
#include "utility/archive/zip.hpp"

namespace engine
{
	namespace data
	{
		class provider_actual_archive_t final : public provider_actual_t
		{

		public:

			bool is_read_only() final
			{
				return true;
			}

			provider_actual_archive_t(const virtual_path_t & virtual_path, std::filesystem::file_time_type last_update, std::shared_ptr<archive::zip_input_t> zip_input, const ustring_t & archive_path) : provider_actual_t(virtual_path, last_update), zip_input(zip_input), archive_path(archive_path)
			{

			}

		private:

			bool delete_target_local() final
			{
				return false;
			}

			std::unique_ptr<input_t> construct_input_local() final;
			std::unique_ptr<output_t> construct_output_local() final
			{
				return nullptr;
			}

			std::shared_ptr<archive::zip_input_t> zip_input;
			ustring_t archive_path;
		};
	}
}

#endif