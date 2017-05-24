#if 0

#include "common/modules.hpp"
#include "common/xml_pixie_archive.hpp"
#include "common/filesystem.hpp"
#include <fstream>

engine::module_manifest_t::module_manifest_t(const ustring_t & name) : manifest_base_t(name, get_path())
{
	try
	{
		std::ifstream ifs(to_string(get_physical_path()).to_utf8(), std::ios_base::in);
		XMLPixieInputArchive archive(ifs);
		archive(cereal::make_nvp("module", *this));
	}
	catch (cereal::Exception & ex)
	{
		emplace_problem(module_problem_t::module_manifest_corrupted, get_physical_path(), name, ustring_t::from_utf8(ex.what()));
	}
}

void engine::module_manifest_t::resave()
{
	std::ofstream ofs(to_string(get_physical_path()).to_utf8(), std::ios_base::out | std::ios_base::trunc);

	{
		XMLPixieOutputArchive archive(ofs);
		archive(cereal::make_nvp("module", *this));
	}
}

engine::sub_module_manifest_t::sub_module_manifest_t(const ustring_t & name) : manifest_base_t(name, get_path())
{
	try
	{
		std::ifstream ifs(to_string(get_physical_path()).to_utf8(), std::ios_base::in);
		XMLPixieInputArchive archive(ifs);
		archive(cereal::make_nvp("sub_module", *this));
	}
	catch (cereal::Exception & ex)
	{
		emplace_problem(module_problem_t::sub_module_manifest_corrupted, get_physical_path(), name, ustring_t::from_utf8(ex.what()));
	}
}

void engine::sub_module_manifest_t::resave()
{
	std::ofstream ofs(to_string(get_physical_path()).to_utf8(), std::ios_base::out | std::ios_base::trunc);
	{
		XMLPixieOutputArchive archive(ofs);
		archive(cereal::make_nvp("sub_module", *this));
	}
}

engine::manifest_database_t::manifest_database_t() : reload_requested(false)
{
	reload();
}

void engine::manifest_database_t::reload()
{
	std::lock_guard<std::recursive_mutex> guard(mutex_load);

	module_manifests.clear();
	sub_module_manifests.clear();
	problems.clear();

	ustring_collection_t output_modules;
	ustring_collection_t output_sub_modules;
	scan_for_available(_U("modules"), output_modules);
	scan_for_available(_U("sub_modules"), output_sub_modules);

	for (auto & item : output_modules)
	{
		std::unique_ptr<module_manifest_t> manifest = std::make_unique<module_manifest_t>(item);
		if (!manifest->is_valid())
			append_problems(manifest->get_problems());
		module_manifests[item] = std::move(manifest);
	}

	for (auto & item : output_sub_modules)
	{
		std::unique_ptr<sub_module_manifest_t> manifest = std::make_unique<sub_module_manifest_t>(item);
		if (!manifest->is_valid())
			append_problems(manifest->get_problems());
		sub_module_manifests[item] = std::move(manifest);
	}
	reload_requested = false;
}

void engine::manifest_database_t::check_for_reload()
{
	std::lock_guard<std::recursive_mutex> guard(mutex_load);

	ustring_collection_t output_modules;
	ustring_collection_t output_sub_modules;
	scan_for_available(_U("modules"), output_modules);
	scan_for_available(_U("sub_modules"), output_sub_modules);

	for (auto & mod : output_modules)
	{
		if (module_manifests.find(mod) == module_manifests.end())
		{
			reload_requested = true;
			return;
		}
	}
	for (auto & mod : output_sub_modules)
	{
		if (sub_module_manifests.find(mod) == sub_module_manifests.end())
		{
			reload_requested = true;
			return;
		}
	}
	for (auto & mod2 : module_manifests)
	{
		if (std::find(output_modules.begin(), output_modules.end(), mod2.first) == output_modules.end())
		{
			reload_requested = true;
			return;
		}
		if(mod2.second->is_updated())
		{
			reload_requested = true;
			return;
		}
	}
	for (auto & mod2 : sub_module_manifests)
	{
		if (std::find(output_sub_modules.begin(), output_sub_modules.end(), mod2.first) == output_sub_modules.end())
		{
			reload_requested = true;
			return;
		}
		if (mod2.second->is_updated())
		{
			reload_requested = true;
			return;
		}
	}
}

void engine::manifest_database_t::scan_for_available(const ustring_t & directory, ustring_collection_t & output)
{
	for (auto & iter : filesystem::directory_iterator(directory.get_cstring()))
	{
		if (filesystem::is_directory(iter.status()))
		{
			std::string filename = iter.path().filename().u8string();
			output.push_back(ustring_t::from_utf8(filename.c_str()));
		}
	}
}

engine::module_information_t engine::module_resolver_t::query_module_information(const ustring_t & module)
{
	std::set<ustring_t> modules_checked;
	std::queue<module_to_check_t> modules_to_check;

	module_information_t ret;

	modules_to_check.emplace(module, true);

	while (!modules_to_check.empty())
	{
		auto mod = modules_to_check.front();
		modules_to_check.pop();

		if (modules_checked.find(mod.name) != modules_checked.end())
			continue;

		modules_checked.insert(mod.name);

		auto manifest = database.get_module(mod.name);
		if (!manifest)
		{
			ret.problems.emplace_back(module_problem_t::module_does_not_exists, manifest_base_t::create_path(mod.name, module_manifest_t::get_path()), mod.name, _U("Manifest file was not found"), mod.callstack);
		}
		else if (!manifest->is_valid())
		{
			if (mod.name == module)
			{
				auto & problems = manifest->get_problems();
				ret.problems.insert(ret.problems.end(), problems.begin(), problems.end());
			}
		}
		else
		{
			ret.modules_stack.push_back(mod.callstack);

			auto & sublist = manifest->get_sub_modules().submodules;
			if (manifest->get_type() == module_manifest_t::allow_any_listed_submodule)
			{
				for (auto & iter : sublist)
				{
					if (mod.allow_sub_modules_from_this_module)
						ret.submodules_allowed[iter] = query_submodule_information(iter);
				}
				for (auto & iter : *database.get_sub_modules())
				{
					if (std::find(sublist.begin(), sublist.end(), iter.first) == sublist.end())
						ret.submodules_forbidden_source[iter.first].push_back(mod.callstack);
				}
			}
			else
			{
				for (auto & iter : *database.get_sub_modules())
				{
					if (mod.allow_sub_modules_from_this_module)
					{
						if (std::find(sublist.begin(), sublist.end(), iter.first) == sublist.end())
							ret.submodules_allowed[iter.first] = query_submodule_information(iter.first);
					}
				}
				for (auto & iter : sublist)
				{
					ret.submodules_forbidden_source[iter].push_back(mod.callstack);
				}
			}

			for (auto & iter : manifest->get_includes().includes)
			{
				modules_to_check.emplace(iter, mod.allow_sub_modules_from_this_module && manifest->is_flag(module_manifest_t::allow_from_included_modules), mod.callstack);
			}
		}
	}

	return ret;
}

engine::submodule_information_t engine::module_resolver_t::query_submodule_information(const ustring_t & submodule)
{
	std::set<ustring_t> submodules_checked;
	std::queue<sub_module_to_check_t> submodules_to_check;

	std::map<ustring_t, std::vector<sub_module_to_check_t> > required_mod_src;
	std::map<ustring_t, std::vector<sub_module_to_check_t> > forbidden_mod_src;
	std::map<ustring_t, std::vector<sub_module_to_check_t> > required_submod_src;
	std::map<ustring_t, std::vector<sub_module_to_check_t> > forbidden_submod_src;

	submodule_information_t ret;

	submodules_to_check.emplace(submodule);
	required_submod_src[submodule].push_back(submodules_to_check.front());

	while (!submodules_to_check.empty())
	{
		auto mod = submodules_to_check.front();
		submodules_to_check.pop();

		if (submodules_checked.find(mod.name) != submodules_checked.end())
			continue;

		submodules_checked.insert(mod.name);

		auto manifest = database.get_sub_module(mod.name);
		if (!manifest)
			ret.problems.emplace_back(module_problem_t::sub_module_does_not_exists, manifest_base_t::create_path(mod.name, module_manifest_t::get_path()), mod.name, _U("Manifest file was not found"), mod.callstack);
		else if (!manifest->is_valid())
		{
			auto & problems = manifest->get_problems();
			ret.problems.insert(ret.problems.end(), problems.begin(), problems.end());
		}
		else
		{
			ret.submodules_required.insert(mod.name);
			ret.submodules_stack[mod.name] = mod.callstack;
			for (auto & iter : manifest->get_required().modules.modules)
			{
				ret.modules_required.insert(iter);
				required_mod_src[iter].push_back(mod);
			}
			for (auto & iter : manifest->get_required().sub_modules.sub_modules)
			{
				submodules_to_check.emplace(iter, mod.callstack);
				required_submod_src[iter].push_back(mod);
			}
			if (manifest->get_forbidden().is_flag(sub_module_manifest_t::forbid_any_module_but_required))
			{
				auto all_modules = database.get_modules();
				auto & required_mods = manifest->get_required().modules.modules;
				for (auto & iter : *all_modules)
				{
					if (std::find(required_mods.begin(), required_mods.end(), iter.first) == required_mods.end())
					{
						ret.modules_forbidden.insert(iter.first);
						forbidden_mod_src[iter.first].push_back(mod);
					}
				}
			}
			else
			{
				for (auto & iter : manifest->get_forbidden().modules.modules)
				{
					ret.modules_forbidden.insert(iter);
					forbidden_mod_src[iter].push_back(mod);
				}
			}

			if (manifest->get_forbidden().is_flag(sub_module_manifest_t::forbid_any_sub_module_but_required))
			{
				auto all_modules = database.get_sub_modules();
				auto & required_mods = manifest->get_required().sub_modules.sub_modules;
				for (auto & iter : *all_modules)
				{
					if (iter.first != submodule && std::find(required_mods.begin(), required_mods.end(), iter.first) == required_mods.end())
					{
						ret.submodules_forbidden.insert(iter.first);
						forbidden_submod_src[iter.first].push_back(mod);
					}
				}
			}
			else
			{
				for (auto & iter : manifest->get_forbidden().sub_modules.sub_modules)
				{
					ret.submodules_forbidden.insert(iter);
					forbidden_submod_src[iter].push_back(mod);
				}
			}
		}
	}

	for (auto & iter : ret.modules_required)
	{
		if (ret.modules_forbidden.find(iter) != ret.modules_forbidden.end())
		{
			for (auto & err : required_mod_src[iter])
				ret.problems.emplace_back(module_problem_t::module_required_here, manifest_base_t::create_path(iter, module_manifest_t::get_path()), iter, _U("Submodules requirements not satisfable"), err.callstack);
			for (auto & err : forbidden_mod_src[iter])
				ret.problems.emplace_back(module_problem_t::module_forbidden_here, manifest_base_t::create_path(iter, module_manifest_t::get_path()), iter, _U("Submodules requirements not satisfable"), err.callstack);
		}
	}

	for (auto & iter : ret.submodules_required)
	{
		if (ret.submodules_forbidden.find(iter) != ret.submodules_forbidden.end())
		{
			for (auto & err : required_submod_src[iter])
				ret.problems.emplace_back(module_problem_t::sub_module_required_here, manifest_base_t::create_path(err.name, sub_module_manifest_t::get_path()), iter, _U("Submodules requirements not satisfable"), err.callstack);
			for (auto & err : forbidden_submod_src[iter])
				ret.problems.emplace_back(module_problem_t::sub_module_forbidden_here, manifest_base_t::create_path(err.name, sub_module_manifest_t::get_path()), iter, _U("Submodules requirements not satisfable"), err.callstack);
		}
	}

	return ret;
}

void engine::module_resolver_t::reload()
{
	database.reload();
	modules_information.clear();
	sub_modules_information.clear();
	problems.clear();

	for (auto & iter : *database.get_modules())
	{
		auto item = modules_information[iter.first] = query_module_information(iter.first);
		append_problems(item.get_problems());
	}
	for (auto & iter : *database.get_sub_modules())
	{
		auto item = sub_modules_information[iter.first] = query_submodule_information(iter.first);
		append_problems(item.get_problems());
	}
}

engine::module_state_t::module_state_t(std::shared_ptr<module_resolver_t> resolver, const ustring_t & module, const ustring_collection_t & sub_modules) : resolver(resolver)
{
	const auto & mod_info = resolver->modules_information.find(module);
	module_callstack_t callstack;
	callstack.push_back(module);

	if (mod_info == resolver->modules_information.end())
	{
		problems.emplace_back(module_problem_t::module_requested_does_not_exists, module_manifest_t::get_path(), module, _U("Module does not exists"), callstack);
		return;
	}
	problems.insert(problems.end(), mod_info->second.get_problems().begin(), mod_info->second.get_problems().end());
	if (mod_info->second.get_problems().empty())
	{
		for (auto & iter : mod_info->second.get_modules_stack())
		{
			modules.push_back(iter.back());
		}
	}

	for (auto & submod : sub_modules)
	{
		enable_internal(submod);
	}
}

engine::module_state_t::enable_result_t engine::module_state_t::enable_test(const ustring_t & submodule)
{
	enable_result_t ret;
	if (!is_valid_module()) return ret;
	auto mod_info = resolver->modules_information[get_module()];

	const auto & sub_mod_info = resolver->sub_modules_information.find(submodule);
	if (sub_mod_info == resolver->sub_modules_information.end())
	{
		ret.problems.emplace_back(module_problem_t::sub_module_requested_does_not_exists, manifest_base_t::create_path(mod_info.modules_stack.begin()->back(), module_manifest_t::get_path()), submodule, _U("Submodule does not exists or has invalid manifest"));
		return ret;
	}

	for (auto & submod : sub_mod_info->second.submodules_required)
	{
		bool invalid = false;
		const auto & sub_mod_info = mod_info.submodules_allowed.find(submodule);

		if (sub_mod_info == mod_info.submodules_allowed.end())
		{
			for (auto & forbidden_source : mod_info.submodules_forbidden_source[submodule])
			{
				ret.problems.emplace_back(module_problem_t::forbidden_by_module, manifest_base_t::create_path(mod_info.modules_stack.begin()->back(), module_manifest_t::get_path()), submod, _U("Submodule forbidden by module"), forbidden_source);
			}
		}
		else
		{
			ret.problems.insert(ret.problems.end(), sub_mod_info->second.get_problems().begin(), sub_mod_info->second.get_problems().end());
			invalid = invalid || sub_mod_info->second.get_problems().size();

			for (auto & iter : sub_mod_info->second.modules_required)
			{
				bool found = false;
				for (auto & stack : mod_info.modules_stack)
				{
					if (!stack.empty() && stack.back() == iter)
					{
						found = true;
					}
				}
				if (!found)
				{
					ret.problems.emplace_back(module_problem_t::requires_module_not_enabled, manifest_base_t::create_path(mod_info.modules_stack.begin()->back(), module_manifest_t::get_path()), iter, _U("Required module not found"), sub_mod_info->second.submodules_stack[submod]);
					invalid = true;
				}
			}
			for (auto & iter : sub_mod_info->second.modules_forbidden)
			{
				for (auto & stack : mod_info.modules_stack)
				{
					if (!stack.empty() && stack.back() == iter)
					{
						ret.problems.emplace_back(module_problem_t::forbids_module_enabled, manifest_base_t::create_path(iter, module_manifest_t::get_path()), iter, _U("Forbidden module is on"), sub_mod_info->second.submodules_stack[submod]);
						invalid = true;
					}
				}
			}
			for (auto & iter : sub_mod_info->second.submodules_forbidden)
			{
				if (std::find(sub_mod_info->second.submodules_required.begin(), sub_mod_info->second.submodules_required.end(), iter) != sub_mod_info->second.submodules_required.end())
					ret.problems.emplace_back(module_problem_t::submodules_forbids_here, manifest_base_t::create_path(iter, module_manifest_t::get_path()), iter, _U("Forbidden sub module is on"), sub_mod_info->second.submodules_stack[submod]);
			}

			if (!invalid)
				ret.other_sub_modules_to_enable.insert(submod);
		}
	}
	return ret;
}

void engine::module_state_t::enable_internal(const ustring_t & submodule)
{
	enable_result_t result = enable(submodule);

	for (auto & problem : result.problems)
	{
		problems.push_back(problem);
	}
}

engine::module_state_t::enable_result_t engine::module_state_t::enable(const ustring_t & submodule)
{
	enable_result_t ret = enable_test(submodule);

	for (auto & iter : ret.other_sub_modules_to_enable)
	{
		auto & enabled = std::find(sub_modules.begin(), sub_modules.end(), iter);
		if (enabled == sub_modules.end())
			sub_modules.push_back(iter);
	}

	return ret;
}

engine::module_state_t::disable_result_t engine::module_state_t::disable_test(const ustring_t & sub_mod_name)
{
	disable_result_t ret;

	if (!is_valid_module()) return ret;

	auto mod_info = resolver->modules_information[get_module()];

	ret.other_sub_modules_to_disable.insert(sub_mod_name);

	for (const auto & iter : mod_info.submodules_allowed)
	{
		if (iter.second.submodules_required.find(sub_mod_name) != iter.second.submodules_required.end())
		{
			for (auto & iter2 : iter.second.submodules_required)
			{
				auto & enabled = std::find(sub_modules.begin(), sub_modules.end(), iter2);
				ret.other_sub_modules_to_disable.insert(*enabled);
			}
		}
	}
	return ret;
}

engine::module_state_t::disable_result_t engine::module_state_t::disable(const ustring_t & sub_mod_name)
{
	disable_result_t ret = disable_test(sub_mod_name);

	for (auto & iter : ret.other_sub_modules_to_disable)
	{
		auto & disabled = std::find(sub_modules.begin(), sub_modules.end(), iter);
		sub_modules.erase(disabled);
	}

	return ret;
}

#endif