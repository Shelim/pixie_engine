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

			scanner_directory_t(std::shared_ptr<filesystem_t> filesystem, const virtual_path_t & base_virtual_path, const std::filesystem::path & base_physical_path, provider_actual_file_t::read_only_t read_only, provider_actual_file_t::use_safe_output_t use_safe_output = provider_actual_file_t::use_safe_output_t::yes) : scanner_t(kind_t::directory), filesystem(filesystem), base_virtual_path(base_virtual_path), base_physical_path(base_physical_path), read_only(read_only), use_safe_output(use_safe_output)
			{

			}

		private:

			friend bool operator==(const scanner_directory_t & scanner1, const scanner_directory_t & scanner2);

			void scan_local(callback_t on_found) final;

			std::shared_ptr<filesystem_t> filesystem;
			virtual_path_t base_virtual_path;
			std::filesystem::path base_physical_path;
			provider_actual_file_t::read_only_t read_only;
			provider_actual_file_t::use_safe_output_t use_safe_output;
		};

		inline bool operator==(const scanner_directory_t & scanner1, const scanner_directory_t & scanner2)
		{
			return scanner1.base_virtual_path == scanner2.base_virtual_path &&
					scanner1.base_physical_path == scanner2.base_physical_path &&
					scanner1.read_only == scanner2.read_only &&
					scanner1.use_safe_output == scanner2.use_safe_output;
		}

	}

}

#endif