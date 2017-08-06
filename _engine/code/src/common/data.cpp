#include "utility/data/input.hpp"
#include "utility/data/state.hpp"
#include "component/data_provider.hpp"
#include "utility/data/changes.hpp"

std::unique_ptr<engine::data::input_t> engine::data::input_t::spawn_partial(int32_t size)
{
	buffer_t buff;
	buff.resize(size);
	read(&buff[0], size);

	return std::move(std::make_unique<input_partial_t>(path, std::move(buff)));
}

void engine::data::scanners_t::scan(state_t * results)
{
	if (!results) return;

	results->clear();
	for (auto & scanner : *scanners)
	{
		scanner->scan();
		add_results(results, scanner.get());
	}
	results->calculate_directories();
}

void engine::data::scanners_t::add_results(state_t * results, scanner_t * scanner)
{
	for (auto & result : scanner->results)
	{
		results->add_provider_actual(std::move(result.second));
	}
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
		if (iter_prev.first.get_type() == virtual_path_t::type_t::log) continue;

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
		if (iter_cur.first.get_type() == virtual_path_t::type_t::log) continue;

		auto iter_prev = previous.providers.find(iter_cur.first);

		if (iter_prev == previous.providers.end())
		{
			ret.emplace_back(iter_cur.first, engine::data::change_t::added);

			mark_changed_directories(iter_cur.first, &ret, &changed_directories);
		}
	}

	return ret;
}


void engine::data_provider_real_t::refresh_virtual_path_type_changes()
{
	type_changes.clear();

	for (auto & change : changes)
	{
		if (change.get_type() == data::change_t::changed_directory)
			type_changes.insert(change.get_path().get_type());
	}
}

void engine::data_provider_real_t::rescan()
{
	std::lock_guard<std::recursive_mutex> guard(mutex_database_next);
	if (requested_rescan)
	{
		data::scanners_t scanner(std::move(scanners_provider->construct_scanners_collection()));
		scanner.scan(&next);
		requested_rescan = false;
	}
}
