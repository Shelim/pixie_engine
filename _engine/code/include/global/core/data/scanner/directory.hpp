#ifndef ENGINE_COMMON_UTILITY_DATA_SCANNER_DIRECTORY_HPP
#define ENGINE_COMMON_UTILITY_DATA_SCANNER_DIRECTORY_HPP
#pragma once

#include "global/core/data/scanner.hpp"
#include "global/core/data/provider_actual/file.hpp"
#include <algorithm>

namespace engine
{
	namespace data
	{
		class scanner_directory_t final : public scanner_t
		{

		public:

			scanner_directory_t(std::shared_ptr<filesystem_t> filesystem, virtual_path_t base_virtual_path, const virtual_path_t::path_t & base_physical_path, provider_actual_file_t::read_only_t read_only, provider_actual_file_t::use_safe_output_t use_safe_output = provider_actual_file_t::use_safe_output_t::yes) : filesystem(filesystem), base_virtual_path(base_virtual_path), base_physical_path(base_physical_path.get_cstring()), read_only(read_only), use_safe_output(use_safe_output)
			{

			}

		private:

			void scan_local(callback_t on_found) final
			{
				
			}

			std::shared_ptr<filesystem_t> filesystem;
			virtual_path_t base_virtual_path;
			std::filesystem::path base_physical_path;
			provider_actual_file_t::read_only_t read_only;
			provider_actual_file_t::use_safe_output_t use_safe_output;
		};
	}

}

#endif