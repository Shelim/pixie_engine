#if 0

#include "common/module/problem.hpp"
#include "common/module/manifest.hpp"
#include "common/module/database.hpp"
#include "common/module/info.hpp"
#include <queue>

void engine::module::item_manifest_base_t::reload_async(data::input_t * input)
{
	if (!input)
	{
		problems.add_problem(problem_t(problem_t::type_t::target_does_not_exists, name));
		return;
	}

	engine::data::input_streambuf_t buff(input);

	std::istream is(&buff);
	
	try
	{
		XMLPixieInputArchive archive(is);
		if (virtual_path.get_type() == virtual_path_t::type_t::modules)
		{
			item_module_manifest_t * mod = dynamic_cast<item_module_manifest_t*>(this);
			if (mod)
				archive(cereal::make_nvp("module", *mod));
		}
		else if (virtual_path.get_type() == virtual_path_t::type_t::submodules)
		{
			item_submodule_manifest_t * mod = dynamic_cast<item_submodule_manifest_t*>(this);
			if (mod)
				archive(cereal::make_nvp("submodule", *mod));
		}
	}
	catch (cereal::Exception & ex)
	{
		problems.add_problem(problem_t(problem_t::type_t::target_manifest_corrupted, name, callstack_t(), ustring_t::from_utf8(ex.what())));
		return;
	}
}

void engine::module::item_manifest_base_t::resave_async(data::output_t * output)
{
	engine::data::output_streambuf_t streambuf(output);

	std::ostream os(&streambuf);
	{
		XMLPixieOutputArchive archive(os);
		if (virtual_path.get_type() == virtual_path_t::type_t::modules)
		{
			item_module_manifest_t * mod = dynamic_cast<item_module_manifest_t*>(this);
			if(mod)
				archive(cereal::make_nvp("module", *mod));
		}
		else if (virtual_path.get_type() == virtual_path_t::type_t::submodules)
		{
			item_submodule_manifest_t * mod = dynamic_cast<item_submodule_manifest_t*>(this);
			if (mod)
				archive(cereal::make_nvp("submodule", *mod));
		}
	}
}

void engine::module::database_manifest_t::init_update()
{
	updated = false;
	problems_since_last_update.clear();
	if (database_data->is_type_change(virtual_path_t::type_t::modules))
	{
		for (auto & mod : modules_manifests)
		{
			if (mod.second->get_meta()->is_flag(data::base_item_t::flag_t::just_updated))
			{
				updated = true;
				problems_since_last_update.merge_problems(mod.second->get()->get_problems());
			}
		}
	}
	if (database_data->is_type_change(virtual_path_t::type_t::submodules))
	{
		for (auto & submod : submodules_manifests)
		{
			if (submod.second->get_meta()->is_flag(data::base_item_t::flag_t::just_updated))
			{
				updated = true;
				problems_since_last_update.merge_problems(submod.second->get()->get_problems());
			}
		}
	}

	if (updated)
	{
		reload_info();

		for (auto & mod : modules_info)
		{
			problems_since_last_update.merge_problems(mod.second.get_problems());
		}
		for (auto & submod : submodules_info)
		{
			problems_since_last_update.merge_problems(submod.second.get_problems());
		}
	}
}

void engine::module::database_manifest_t::reload_info()
{
	modules_info.clear();
	submodules_info.clear();

	for (auto & mod : modules_manifests)
	{
		if(!mod.second->get()->is_valid())
			modules_info.emplace(mod.first, module_information_t(mod.first, this));
	}
	for (auto & submod : submodules_manifests)
	{
		if (!submod.second->get()->is_valid())
			submodules_info.emplace(submod.first, submodule_information_t(submod.first, this));
	}
}

engine::module::module_information_t::module_information_t(const name_t & name, database_manifest_t * database_manifest)
{
	items_t modules_checked;
	std::queue<module_to_check_t> modules_to_check;

	modules_to_check.emplace(name_full_t(name, name_full_t::type_t::module), true);

	while (!modules_to_check.empty())
	{
		auto mod = modules_to_check.front();
		modules_to_check.pop();

		if (modules_checked.find(mod.get_name().get_name()) != modules_checked.end())
			continue;

		modules_checked.insert(mod.get_name().get_name());

		auto manifest = database_manifest->get_module_manifest(mod.get_name().get_name());
		if (!manifest || manifest->is_destroyed())
			problems.add_problem(problem_t(problem_t::type_t::target_does_not_exists, mod.get_name(), mod.get_callstack()));
		else if (!manifest->is_valid())
		{
			if (mod.get_name().get_name() == name)
				problems.merge_problems(manifest->get_problems());
		}
		else
		{
			for (auto & iter : mod.get_callstack().get_items())
				modules_stack.add(iter);

			auto & sublist = manifest->get_submodules().get_submodules();
			if (manifest->get_submodules().get_type() == module_manifest_t::submodules_t::allow_any_listed_submodule)
			{
				for (auto & iter : sublist)
				{
					if (mod.is_allowed_sub_modules_from_this_module())
						submodules_allowed.insert(iter);
				}
				for (auto & iter : database_manifest->get_submodules_manifests())
				{
					if (std::find(sublist.begin(), sublist.end(), iter.first) == sublist.end())
					{
						for (auto & iter : mod.get_callstack().get_items())
							submodules_forbidden_source[iter.get_name()].add(iter);
					}
				}
			}
			else
			{
				for (auto & iter : database_manifest->get_submodules_manifests())
				{
					if (mod.is_allowed_sub_modules_from_this_module())
					{
						if (std::find(sublist.begin(), sublist.end(), iter.first) == sublist.end())
							submodules_allowed.insert(iter.first);
					}
				}
				for (auto & iter : sublist)
				{
					for (auto & iter : mod.get_callstack().get_items())
						submodules_forbidden_source[iter.get_name()].add(iter);
				}
			}

			for (auto & iter : manifest->get_includes().get_includes())
			{
				modules_to_check.emplace(module_to_check_t(name_full_t(iter, name_full_t::type_t::module), mod.is_allowed_sub_modules_from_this_module() && manifest->get_submodules().is_flag(module_manifest_t::submodules_t::flag_t::allow_from_included_modules), mod.get_callstack()));
			}
		}
	}
}

engine::module::submodule_information_t::submodule_information_t(const name_t & name, database_manifest_t * database_manifest)
{
	items_t submodules_checked;
	std::queue<sub_module_to_check_t> submodules_to_check;

	std::map<name_t, std::vector<sub_module_to_check_t> > required_mod_src;
	std::map<name_t, std::vector<sub_module_to_check_t> > forbidden_mod_src;
	std::map<name_t, std::vector<sub_module_to_check_t> > required_submod_src;
	std::map<name_t, std::vector<sub_module_to_check_t> > forbidden_submod_src;
	
	submodules_to_check.emplace(name_full_t(name, name_full_t::type_t::submodule));
	required_submod_src[name].push_back(submodules_to_check.front());

	while (!submodules_to_check.empty())
	{
		auto mod = submodules_to_check.front();
		submodules_to_check.pop();

		if (submodules_checked.find(mod.get_name().get_name()) != submodules_checked.end())
			continue;

		submodules_checked.insert(mod.get_name().get_name());

		auto manifest = database_manifest->get_submodule_manifest(mod.get_name().get_name());
		if (!manifest || manifest->is_destroyed())
			problems.add_problem(problem_t(problem_t::type_t::target_does_not_exists, mod.get_name(), mod.get_callstack()));
		else if (!manifest->is_valid())
			problems.merge_problems(manifest->get_problems());
		else
		{
			submodules_required.insert(mod.get_name().get_name());
			submodules_stack[mod.get_name().get_name()] = mod.get_callstack();
			for (auto & iter : manifest->get_required().get_modules().get_modules())
			{
				modules_required.insert(iter);
				required_mod_src[iter].push_back(mod);
			}
			for (auto & iter : manifest->get_required().get_submodules().get_submodules())
			{
				submodules_to_check.emplace(sub_module_to_check_t(name_full_t(iter, name_full_t::type_t::submodule), mod.get_callstack()));
				required_submod_src[iter].push_back(mod);
			}
			if (manifest->get_forbidden().is_flag(submodule_manifest_t::forbidden_t::flag_t::forbid_any_module_but_required))
			{
				auto & all_modules = database_manifest->get_modules_manifests();
				auto & required_mods = manifest->get_required().get_modules().get_modules();
				for (auto & iter : all_modules)
				{
					if (std::find(required_mods.begin(), required_mods.end(), iter.first) == required_mods.end())
					{
						modules_forbidden.insert(iter.first);
						forbidden_mod_src[iter.first].push_back(mod);
					}
				}
			}
			else
			{
				for (auto & iter : manifest->get_forbidden().get_modules().get_modules())
				{
					modules_forbidden.insert(iter);
					forbidden_mod_src[iter].push_back(mod);
				}
			}

			if (manifest->get_forbidden().is_flag(submodule_manifest_t::forbidden_t::flag_t::forbid_any_sub_module_but_required))
			{
				auto & all_modules = database_manifest->get_submodules_manifests();
				auto & required_mods = manifest->get_required().get_submodules().get_submodules();
				for (auto & iter : all_modules)
				{
					if (iter.first != name && std::find(required_mods.begin(), required_mods.end(), iter.first) == required_mods.end())
					{
						submodules_forbidden.insert(iter.first);
						forbidden_submod_src[iter.first].push_back(mod);
					}
				}
			}
			else
			{
				for (auto & iter : manifest->get_forbidden().get_submodules().get_submodules())
				{
					submodules_forbidden.insert(iter);
					forbidden_submod_src[iter].push_back(mod);
				}
			}
		}
	}

	for (auto & iter : modules_required)
	{
		if (modules_forbidden.find(iter) != modules_forbidden.end())
		{
			for (auto & err : required_mod_src[iter])
				problems.add_problem(problem_t(problem_t::type_t::target_required_here, name_full_t(iter, name_full_t::type_t::module), err.get_callstack()));
			for (auto & err : forbidden_mod_src[iter])
				problems.add_problem(problem_t(problem_t::type_t::target_forbidden_here, name_full_t(iter, name_full_t::type_t::module), err.get_callstack()));
		}
	}

	for (auto & iter : submodules_required)
	{
		if (submodules_forbidden.find(iter) != submodules_forbidden.end())
		{
			for (auto & err : required_submod_src[iter])
				problems.add_problem(problem_t(problem_t::type_t::target_required_here, name_full_t(iter, name_full_t::type_t::submodule), err.get_callstack()));
			for (auto & err : forbidden_submod_src[iter])
				problems.add_problem(problem_t(problem_t::type_t::target_forbidden_here, name_full_t(iter, name_full_t::type_t::submodule), err.get_callstack()));
		}
	}
}

#endif