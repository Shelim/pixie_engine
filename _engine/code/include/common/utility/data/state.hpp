#ifndef ENGINE_COMMON_UTILITY_DATA_STATE_HPP
#define ENGINE_COMMON_UTILITY_DATA_STATE_HPP
#pragma once

#include "utility/vfs/virtual_path.hpp"
#include "utility/data/provider.hpp"
#include "utility/data/changes.hpp"
#include <map>
#include <vector>
#include <string>
#include <unordered_set>

namespace engine
{
	class logger_t;

	namespace data
	{
		class state_t final
		{

		public:

			typedef std::map<virtual_path_t, std::unique_ptr<provider_t> > providers_t;

			typedef std::unordered_set<virtual_path_t::path_t> items_t;

			typedef std::map<virtual_path_t, items_t > directories_t;
			typedef std::map<virtual_path_t, items_t > files_t;
			typedef std::map<virtual_path_t::type_t, items_t > all_t;

			provider_t * get_provider(const virtual_path_t & filename)
			{
				auto iter = providers.find(filename);
				if (iter == providers.end())
					return nullptr;
				return iter->second.get();
			}

			bool is_read_only(const virtual_path_t & filename)
			{
				auto iter = providers.find(filename);
				if (iter == providers.end())
					return true;
				return iter->second->is_read_only();
			}

			const items_t & get_files(const virtual_path_t & path)
			{
				auto iter = files.find(path);
				if (iter == files.end())
					return empty;
				return iter->second;
			}

			const items_t & get_all(const virtual_path_t::type_t & type)
			{
				auto iter = all.find(type);
				if (iter == all.end())
					return empty;
				return iter->second;
			}

			const items_t & get_directories(const virtual_path_t & path)
			{
				auto iter = directories.find(path);
				if (iter == directories.end())
					return empty;
				return iter->second;
			}
			
			void clear()
			{
				providers.clear();
				directories.clear();
				files.clear();
			}

			void add_provider_actual(std::unique_ptr<provider_actual_t> provider)
			{
				virtual_path_t path = provider->get_virtual_path();
				auto iter = providers.find(path);
				if (iter == providers.end())
				{
					providers[path] = std::make_unique<provider_t>();
				}

				providers[path]->add_provider_actual(std::move(provider));
			}

			void calculate_directories()
			{
				for (auto & iter : providers)
				{
					add_file(iter.first.get_path_lower(), iter.first.get_type());
				}
			}

			static changes_t calculate_changes(state_t & current, state_t & previous);

		private:

			typedef std::set<ustring_t> changed_directories_t;

			static void mark_changed_directories(const virtual_path_t & path, changes_t * output, changed_directories_t * directories);

			void add_file(virtual_path_t::path_t path, virtual_path_t::type_t type)
			{
				int_fast32_t index = path.last_index_of('/');

				virtual_path_t dir;
				dir.set_type(type);
				virtual_path_t::path_t filename = path;

				if (index > 0)
				{
					dir.set_path(path.substr(0, index));
					filename = path.substr(index + 1);
				}

				files[dir].insert(filename);
				all[type].insert(path);

				add_directory(dir.get_path(), type);
			}

			void add_directory(virtual_path_t::path_t path, virtual_path_t::type_t type)
			{
				int_fast32_t index = path.last_index_of('/');

				virtual_path_t dir;
				dir.set_type(type);
				virtual_path_t::path_t dirname = path;

				if (index > 0)
				{
					dir.set_path(path.substr(0, index));
					dirname = path.substr(index + 1);
				}

				directories[dir].insert(dirname);

				if (!dir.get_path().is_empty())
					add_directory(dir.get_path(), type);
			}

			providers_t providers;
			all_t all;
			directories_t directories;
			files_t files;
			items_t empty;

		};
	}

}

#endif