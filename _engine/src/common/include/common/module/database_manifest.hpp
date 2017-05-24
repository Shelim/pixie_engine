#ifndef ENGINE_COMMON_MODULE_DATABASE_MANIFEST_HPP
#define ENGINE_COMMON_MODULE_DATABASE_MANIFEST_HPP
#pragma once

#include "common/module/manifest.hpp"
#include "common/data/database.hpp"
#include "common/module/info.hpp"

namespace engine
{

	namespace module
	{

		class database_manifest_t
		{

		public:
			
			typedef std::map<name_t, std::unique_ptr<module_manifest_t> > modules_manifests_t;
			typedef std::map<name_t, std::unique_ptr<submodule_manifest_t> > submodules_manifests_t;

			typedef std::map<name_t, module_information_t > modules_info_t;
			typedef std::map<name_t, submodule_information_t > submodules_info_t;

			database_manifest_t(std::shared_ptr<data::database_t> database_data) : database_data(database_data), updated(false)
			{
				init_update();
			}

			void init_update();

			const problems_t & get_problems_since_last_update() const
			{
				return problems_since_last_update;
			}

			module_manifest_t * create_module(const name_t & name)
			{
				auto & iter = modules_manifests.find(name);
				if (iter == modules_manifests.end())
				{
					modules_manifests[name] = std::make_unique<module_manifest_t>(name);
					modules_manifests[name]->resave();
				}

				return modules_manifests[name].get();
			}

			submodule_manifest_t * create_submodule(const name_t & name)
			{
				auto & iter = submodules_manifests.find(name);
				if (iter == submodules_manifests.end())
				{
					submodules_manifests[name] = std::make_unique<submodule_manifest_t>(name);
					submodules_manifests[name]->resave();
				}

				return submodules_manifests[name].get();
			}

			module_manifest_t * get_module_manifest(const name_t & name)
			{
				auto & iter = modules_manifests.find(name);
				if (iter == modules_manifests.end()) return nullptr;
				return iter->second.get();
			}

			submodule_manifest_t * get_submodule_manifest(const name_t & name)
			{
				auto & iter = submodules_manifests.find(name);
				if (iter == submodules_manifests.end()) return nullptr;
				return iter->second.get();
			}

			const modules_manifests_t & get_modules_manifests() const
			{
				return modules_manifests;
			}

			const submodules_manifests_t & get_submodules_manifests() const
			{
				return submodules_manifests;
			}

			module_information_t * get_module_info(const name_t & name)
			{
				auto & iter = modules_info.find(name);
				if (iter == modules_info.end()) return nullptr;
				return &iter->second;
			}

			submodule_information_t * get_submodule_info(const name_t & name)
			{
				auto & iter = submodules_info.find(name);
				if (iter == submodules_info.end()) return nullptr;
				return &iter->second;
			}

			void query_for_all_problems(problems_t * output)
			{
				for (auto & mod : modules_manifests)
				{
					output->merge_problems(mod.second->get()->get_problems());
				}
				for (auto & submod : submodules_manifests)
				{
					output->merge_problems(submod.second->get()->get_problems());
				}
				for (auto & mod : modules_info)
				{
					output->merge_problems(mod.second.get_problems());
				}
				for (auto & submod : submodules_info)
				{
					output->merge_problems(submod.second.get_problems());
				}
			}
		
			bool is_updated() const
			{
				return updated;
			}

		private:

			void reload_info();
			
			bool is_manifest(const virtual_path_t & path, name_t * name_output)
			{
				auto type = path.get_type();
				if (type != virtual_path_t::type_t::modules && type != virtual_path_t::type_t::submodules)
					return false;

				auto & p = path.get_path();

				int_fast32_t in = p.index_of(_U("/"));
				if (in == -1 || in == 0) return false;

				if (p.substr(in + 1) == item_manifest_base_t::manifest_filename())
				{
					if (name_output)
						*name_output = p.substr(0, in);
					return true;
				}

				return false;
			}

			std::shared_ptr<data::database_t> database_data;

			modules_manifests_t modules_manifests;
			submodules_manifests_t submodules_manifests;

			modules_info_t modules_info;
			submodules_info_t submodules_info;
			
			problems_t problems_since_last_update;
			bool updated;
		};

	}

}
#endif