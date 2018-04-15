#ifndef ENGINE_COMMON_UTILITY_ARCHIVE_BASE_HPP
#define ENGINE_COMMON_UTILITY_ARCHIVE_BASE_HPP
#pragma once

#include "global/core/data/input.hpp"
#include "global/core/data/output.hpp"
#include "global/core/data/provider_actual.hpp"
#include "utility/pattern/flags.hpp"
#include "global/core/vfs/virtual_path.hpp"
#include "global/core/vfs/filesystem.hpp"
#include <vector>

namespace engine
{
	namespace data
	{
		namespace archive
		{

			class input_base_t
			{

			public:

				virtual ~input_base_t()
				{

				}

				std::filesystem::file_time_type get_time_last_mod() const
				{
					return time_last_mod;
				}

				const virtual_path_t & get_path() const
				{
					return input->virtual_path();
				}

				class file_t
				{

				public:

					file_t(const virtual_path_t & vpath, const ustring_t & archive_path, std::filesystem::file_time_type mod_time) : vpath(vpath), archive_path(archive_path), mod_time(mod_time)
					{

					}

					const virtual_path_t & get_path() const
					{
						return vpath;
					}

					const ustring_t & get_archive_path() const
					{
						return archive_path;
					}

					std::filesystem::file_time_type get_mod_time() const
					{
						return mod_time;
					}

				private:

					virtual_path_t vpath;
					ustring_t archive_path;
					std::filesystem::file_time_type mod_time;

				};

				typedef std::vector<file_t> files_t;

				const files_t & get_files()
				{
					return files;
				}

				virtual std::unique_ptr<data::input_t> get_file(const ustring_t & archive_path) = 0;
				std::unique_ptr<data::input_t> get_file(const file_t & file)
				{
					return std::move(get_file(file.get_archive_path()));
				}
			
			protected:

				input_base_t(std::unique_ptr<data::input_t> input, std::filesystem::file_time_type time_last_mod = std::filesystem::file_time_type::min()) : input(std::move(input)), time_last_mod(time_last_mod)
				{
					
				}

				input_base_t(data::provider_actual_t * provider) : input(std::move(provider->construct_input())), time_last_mod(provider->get_time_last_mod())
				{

				}

				data::input_t * get_input()
				{
					return input.get();
				}

				virtual_path_t construct_virtual_path(const std::filesystem::path & path_inside)
				{
					ustring_t str = get_path().get_path();
					str = str.substr(0, std::max(0, str.last_index_of("/"_u)));

					std::filesystem::path ret = str.get_cstring();

					ustring_t canonized = virtual_path_t::canonize_path(path_inside);
					ret /= canonized.get_cstring();
					
					virtual_path_t vpath = virtual_path_t::canonize_path(ret);
					vpath.set_type(get_path().get_type());

					return vpath;
				}

				void add_file(const virtual_path_t & vpath, const ustring_t & archive_path)
				{
					files.emplace_back(vpath, archive_path, time_last_mod);
				}

				void add_file(const virtual_path_t & vpath, const ustring_t & archive_path, std::filesystem::file_time_type mod_time)
				{
					files.emplace_back(vpath, archive_path, mod_time);
				}


			private:

				std::unique_ptr<data::input_t> input;
				std::filesystem::file_time_type time_last_mod;
				files_t files;
			};

			class output_base_t
			{

			public:

				virtual ~output_base_t()
				{

				}

				const virtual_path_t & get_path() const
				{
					return output->virtual_path();
				}

				virtual std::unique_ptr<data::output_t> save_file(const virtual_path_t & path) = 0;
			
			protected:

				output_base_t(std::unique_ptr<data::output_t> output) : output(std::move(output))
				{
					
				}

				output_base_t(data::provider_actual_t * provider) : output(std::move(provider->construct_output()))
				{

				}

				data::output_t * get_output()
				{
					return output.get();
				}

				std::filesystem::path construct_archive_path(const virtual_path_t & path)
				{
					
				}


			private:

				std::unique_ptr<data::output_t> output;
			};
		
		}

	}


}

#include "global/core/data/archive/zip.hpp"

#endif