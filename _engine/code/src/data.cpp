/*
#include "global/core/data/changes.hpp"
#include "global/core/data/collection_items.hpp"
#include "global/core/data/input.hpp"
#include "global/core/data/item_actual.hpp"
#include "global/core/data/item.hpp"
#include "global/core/data/output.hpp"
#include "global/core/data/provider_actual.hpp"
#include "global/core/data/provider.hpp"
#include "global/core/data/results.hpp"
#include "global/core/data/scanner.hpp"
#include "global/core/data/scanners.hpp"
#include "global/core/data/state.hpp"
*/

/*

#include "global/core/data/input.hpp"
#include "global/core/data/state.hpp"
#include "global/core/data/results.hpp"
#include "global/core/data/scanner/directory.hpp"
#include "global/core/data/provider_actual/archive.hpp"
#include "global/component/data_manipulator.hpp"
#include "global/component/data_manipulator/msg_scanner_found_item.hpp"
#include "global/core/data/changes.hpp"


std::unique_ptr<engine::data::input_t> engine::data::input_t::spawn_partial(int32_t size)
{
	buffer_t buff;
	buff.resize(size);
	read(&buff[0], size);

	return std::move(std::make_unique<input_partial_t>(path, std::move(buff)));
}

void engine::data::scanners_t::scan(results_t * results)
{
	if (!results) return;
	results->begin_scanning();
	for (auto & scanner : *scanners)
	{
		scanner->scan(results);
	}
	results->end_scanning();
}

void engine::data::state_t::mark_changed_directories(const virtual_path_t & path, changes_t * output, changed_directories_t * directories)
{
	virtual_path_t::path_t path_bare = path.get_path();

	int oldindex = 0;
	int newindex = 0;
	do
	{
		ustring_t dir = path_bare.substr(0, newindex);
		if (directories->find(dir) == directories->end())
		{
			virtual_path_t path_to_add(dir, path.get_type());
			output->emplace_back(path_to_add, engine::data::change_t::changed_directory);
			directories->insert(dir);
		}
		oldindex = newindex + 1;
	} while ((newindex = path_bare.index_of_utf8(u8"/", oldindex)) != -1);
}


engine::data::changes_t engine::data::state_t::calculate_changes(state_t & current, state_t & previous)
{
	engine::data::changes_t ret;
	changed_directories_t changed_directories;

	for (auto & iter_prev : previous.providers)
	{
		auto iter_cur = current.providers.find(iter_prev.first);

		bool deleted = (iter_cur == current.providers.end());
		bool updated = deleted;

		if (!deleted)
		{
			if (iter_cur->second->get_time_last_mod() != iter_prev.second->get_time_last_mod())
			{
				updated = true;
			}
		}

		if (updated)
		{
			if (deleted)
			{
				ret.emplace_back(iter_prev.first, engine::data::change_t::deleted);
			}
			else if (updated)
			{
				ret.emplace_back(iter_prev.first, engine::data::change_t::updated);
			}
			if (deleted || updated)
			{
				mark_changed_directories(iter_prev.first, &ret, &changed_directories);
			}
		}
	}

	for (auto & iter_cur : current.providers)
	{
		auto iter_prev = previous.providers.find(iter_cur.first);

		if (iter_prev == previous.providers.end())
		{
			ret.emplace_back(iter_cur.first, engine::data::change_t::added);

			mark_changed_directories(iter_cur.first, &ret, &changed_directories);
		}
	}

	return ret;
}


void engine::data_manipulator_real_actual_t::refresh_virtual_path_type_changes()
{
	type_changes.clear();

	for (auto & change : changes)
	{
		if (change.get_type() == data::change_t::changed_directory)
			type_changes.insert(change.get_path().get_type());
	}
}

void engine::data_manipulator_real_actual_t::rescan()
{
	std::lock_guard<std::recursive_mutex> guard(mutex_database_next);
	if (requested_rescan)
	{
		data::scanners_t scanner(std::move(scanners_provider->construct_scanners_collection()));
		data::results_t results(&next, messenger);
		scanner.scan(&results);
		requested_rescan = false;
	}
}

void engine::data::scanner_directory_t::scan_local(results_t  * results) 
{
	std::size_t len = base_physical_path.native().size();
	if (base_physical_path.native()[len - 1] != '/' && base_physical_path.native()[len - 1] != '\\')
		len++;

	std::error_code ec;

	for (auto & file : filesystem->iterate_files_in_subdirectories(base_physical_path))
	{
		virtual_path_t virtual_path;
		virtual_path.set_type(base_virtual_path.get_type());

		virtual_path_t::path_t path = base_virtual_path.get_path();
		if (!path.is_empty() && path.last_ascii() != '/')
			path.append_utf8(u8"/");

		std::string path_regular = file.u8string().substr(len);
		std::replace(path_regular.begin(), path_regular.end(), '\\', '/');
		path.append_utf8(path_regular.c_str());

		virtual_path.set_path(path);

		results->add_result(std::make_unique<provider_actual_file_t>(filesystem, virtual_path, file, read_only, use_safe_output));
	}
}



bool engine::data::provider_t::add_provider_actual(std::unique_ptr<provider_actual_t> provider)
{
	if (!provider) return false;

	if (providers.empty())
		virtual_path = provider->get_virtual_path();

	if (virtual_path == provider->get_virtual_path())
	{
		providers.insert(std::move(provider));
		return true;
	}
	return false;
}

std::unique_ptr<engine::data::input_t> engine::data::provider_actual_archive_t::construct_input_local()
{
	return std::make_unique<engine::data::input_buffer_t> (get_virtual_path(), zip_input->unpack_file(archive_path));
}

*/