#ifndef ENGINE_COMMON_DATA_SCANNER_FILE_HPP
#define ENGINE_COMMON_DATA_SCANNER_FILE_HPP
#pragma once

#include "common/data/scanner.hpp"
#include "common/data/provider_actual_file.hpp"

namespace engine
{
	namespace data
	{
		class scanner_file_t final : public scanner_t
		{

		public:

			scanner_file_t(virtual_path_t virtual_path, const std::filesystem::path & physical_path, bool is_read_only) : virtual_path(virtual_path), physical_path(physical_path), is_read_only(is_read_only)
			{

			}

		private:

			void scan_local() final
			{
				std::error_code ec;
				if (std::filesystem::exists(physical_path, ec))
				{
					add_result(std::make_unique<provider_actual_file_t>(virtual_path, physical_path, is_read_only));
				}
			}

			virtual_path_t virtual_path;
			std::filesystem::path physical_path;
			bool is_read_only;
		};
	}

}

#endif