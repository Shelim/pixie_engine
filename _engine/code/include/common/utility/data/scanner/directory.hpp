#ifndef ENGINE_COMMON_UTILITY_DATA_SCANNER_DIRECTORY_HPP
#define ENGINE_COMMON_UTILITY_DATA_SCANNER_DIRECTORY_HPP
#pragma once

#include "utility/data/scanner.hpp"
#include "utility/data/provider_actual/file.hpp"
#include <algorithm>

namespace engine
{
	namespace data
	{
		class scanner_directory_t final : public scanner_t
		{

		public:

			scanner_directory_t(virtual_path_t base_virtual_path, const virtual_path_t::path_t & base_physical_path, provider_actual_file_t::read_only_t read_only) : base_virtual_path(base_virtual_path), base_physical_path(base_physical_path.get_cstring()), read_only(read_only)
			{

			}

		private:

			void scan_local(results_t  * results) final;

			virtual_path_t base_virtual_path;
			std::filesystem::path base_physical_path;
			provider_actual_file_t::read_only_t read_only;
		};
	}

}

#endif