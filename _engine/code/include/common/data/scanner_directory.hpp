#ifndef ENGINE_COMMON_DATA_SCANNER_DIRECTORY_HPP
#define ENGINE_COMMON_DATA_SCANNER_DIRECTORY_HPP
#pragma once

#include "common/data/scanner.hpp"
#include "common/data/provider_actual_file.hpp"
#include <algorithm>

namespace engine
{
	namespace data
	{
		class scanner_directory_t final : public scanner_t
		{

		public:

			scanner_directory_t(virtual_path_t base_virtual_path, const std::filesystem::path & base_physical_path, bool is_read_only) : base_virtual_path(base_virtual_path), base_physical_path(base_physical_path), is_read_only(is_read_only)
			{

			}

		private:

			void scan_local() final
			{
				std::size_t len = base_physical_path.native().size();
				if (base_physical_path.native()[len - 1] != '/' && base_physical_path.native()[len - 1] != '\\')
					len++;

				std::error_code ec;

				for (auto & file : std::filesystem::recursive_directory_iterator(base_physical_path))
				{
					if (is_regular_file(file, ec))
					{
						virtual_path_t virtual_path;
						virtual_path.set_type(base_virtual_path.get_type());

						virtual_path_t::path_t path = base_virtual_path.get_path();
						if(!path.is_empty() && path[path.len()-1] != '/')
							path.append_utf8(u8"/");

						std::string path_regular = file.path().u8string().substr(len);
						std::replace(path_regular.begin(), path_regular.end(), '\\', '/');
						path.append_utf8(path_regular.c_str());

						virtual_path.set_path(path);

						add_result(std::make_unique<provider_actual_file_t>(virtual_path, file.path(), is_read_only));
					}
				}				
			}

			virtual_path_t base_virtual_path;
			std::filesystem::path base_physical_path;
			bool is_read_only;
		};
	}

}

#endif