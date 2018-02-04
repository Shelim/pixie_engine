#ifndef ENGINE_COMMON_UTILITY_ARCHIVE_BASE_HPP
#define ENGINE_COMMON_UTILITY_ARCHIVE_BASE_HPP
#pragma once

#include "global/core/vfs/virtual_path.hpp"
#include "utility/pattern/flags.hpp"
#include "global/core/data/provider_actual.hpp"
#include "global/core/data/input.hpp"

namespace engine
{
	namespace data
	{
		class results_t;
	}
	namespace archive
	{

		class input_base_t
		{

		public:

			virtual ~input_base_t()
			{

			}

			const virtual_path_t & get_path() const
			{
				return path;
			}

			std::filesystem::file_time_type get_time_last_mod() const
			{
				return time_last_mod;
			}

			typedef std::vector<std::unique_ptr<data::provider_actual_t> > providers_t;

			virtual void construct_all_providers(providers_t * providers) = 0;

			void iterate_files_to_scanning_results(data::results_t * results);

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

			virtual data::input_t::buffer_t unpack_file(const ustring_t & archive_path) = 0;
			data::input_t::buffer_t unpack_file(const file_t & file)
			{
				return unpack_file(file.get_archive_path());
			}
		
		protected:

			input_base_t(std::unique_ptr<data::input_t> input, virtual_path_t path = virtual_path_t(), std::filesystem::file_time_type time_last_mod = std::filesystem::file_time_type::min()) : input(std::move(input)), path(path), time_last_mod(time_last_mod)
			{
				
			}

			input_base_t(data::provider_actual_t * provider) : input(std::move(provider->construct_input())), path(provider->get_virtual_path()), time_last_mod(provider->get_time_last_mod())
			{

			}

			data::input_t * get_input()
			{
				return input.get();
			}

			virtual_path_t construct_virtual_path(const std::filesystem::path & path_inside)
			{
				ustring_t str = path.get_path();
				str = str.substr(0, std::max(0, str.last_index_of("/"_u)));
				std::filesystem::path ret = str.get_cstring();
				ustring_t canonized = virtual_path_t::canonize_path(path_inside);
				ret /= canonized.get_cstring();
				virtual_path_t vpath = virtual_path_t::canonize_path(ret);
				vpath.set_type(path.get_type());
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
			virtual_path_t path;
			std::filesystem::file_time_type time_last_mod;
			files_t files;
		};
	}


}

#endif