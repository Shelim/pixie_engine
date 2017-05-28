#include "common/data/database.hpp"
#include "common/data/database_items.hpp"
#include "common/data/item.hpp"
#include "common/platform.hpp"
#include "common/data/input.hpp"
#include "common/data/item_operation.hpp"

engine::data::item_base_t::~item_base_t()
{
	database_items->get_logger()->p_msg(_U("Unloaded '#1#' as no longer needed"), get_path());
}

void engine::data::item_operation_t::execute_steps(step_t::caller_t caller)
{
	while (execute_step(caller))
	{
		// deliberately do nothing!
	}
	if (is_completed())
	{
		std::lock_guard<std::recursive_mutex> guard(mutex_completed);
		if (!is_flag(flag_t::has_completed_executed))
		{
			set_flag(flag_t::has_completed_executed, true);
			if (result == result_t::error)
				target->destroy();
			else if (result == result_t::success && get_type() != type_t::free)
				target->undestroy();
		}
	}
}

void engine::data::item_operation_t::init()
{
	if (target->set_operation_pending(this))
	{
		if (logger)
		{
			if(get_type() == type_t::load)
				task = logger->p_task_start(_U("Reloading '#1#'"), target->get_path());
			else if (get_type() == type_t::save)
				task = logger->p_task_start(_U("Resaving '#1#'"), target->get_path());
			else if (get_type() == type_t::free)
				task = logger->p_task_start(_U("Freeing '#1#'"), target->get_path());
		}

		add_step(0, step_t::caller_t::sync);
	}
	else
		result = result_t::already_started;
}

engine::data::item_operation_t::~item_operation_t()
{
	if (logger)
	{
		if (result == result_t::success)
			logger->p_task_done(task);
		else if(result != result_t::already_started)
			logger->p_task_failed(task);
	}

	target->set_operation_pending(this, true);
}

bool engine::data::item_operation_t::execute_step(step_t::caller_t caller)
{
	if (current_step >= steps.size()) return false;

	if (steps[current_step].get_caller() != caller) return false;

	if (!target->execute_operation_pending(steps[current_step]))
	{
		result = result_t::error;
		current_step = steps.size();
		return false;
	}
	++current_step;

	return true;
}

bool engine::data::item_generic_t::set_operation_pending(item_operation_t * operation, bool clear)
{
	std::lock_guard<std::mutex> guard(mutex_operation_pending);
	if (operation_pending)
	{
		if (clear && operation_pending == operation)
		{
			operation_pending = nullptr;
			return true;
		}
		return false;
	}

	if (operation->get_type() == engine::data::item_operation_t::type_t::save)
		clear_requested_save();
	else if (operation->get_type() == engine::data::item_operation_t::type_t::load)
		clear_requested_load();

	operation_pending = operation;
	return true;
}

std::unique_ptr<engine::data::input_t> engine::data::input_t::spawn_partial(int32_t size)
{
	buffer_t buff;
	buff.resize(size);
	read(&buff[0], size);

	return std::move(std::make_unique<input_partial_t>(path, std::move(buff)));
}

void engine::data::item_content_base_t::set_dirty()
{
	if(owner && owner->auto_resave())
		owner->request_save();
}

bool engine::data::item_generic_t::execute_operation_pending(item_operation_t::step_t step)
{
	std::lock_guard<std::mutex> guard(mutex_operation_pending);
	if (operation_pending)
	{
		return content->execute_operation(step, operation_pending);
	}
	return true;
}

void engine::data::database_t::refresh_virtual_path_type_changes()
{
	type_changes.clear();

	for (auto & change : changes)
	{
		if (change.get_type() == database_change_t::changed_directory)
			type_changes.insert(change.get_path().get_type());
	}
}

void engine::data::database_t::rescan()
{
	std::lock_guard<std::recursive_mutex> guard(mutex_database_next);
	if (requested_rescan)
	{
		scanners_t scanner(std::move(platform_get_scanners_collection()));
		scanner.scan(&next); 
		requested_rescan = false;
	}
}

void engine::data::database_state_t::mark_changed_directories(const virtual_path_t & path, database_changes_t * output, changed_directories_t * directories)
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
			output->emplace_back(path_to_add, engine::data::database_change_t::changed_directory);
			directories->insert(dir);
		}
		oldindex = newindex + 1;
	} while ((newindex = path_bare.index_of_utf8(u8"/", oldindex)) != -1);
}

engine::data::database_changes_t engine::data::database_state_t::calculate_changes(database_state_t & current, database_state_t & previous)
{
	engine::data::database_changes_t ret;
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
				ret.emplace_back(iter_prev.first, engine::data::database_change_t::deleted);
			}
			else if (updated)
			{
				ret.emplace_back(iter_prev.first, engine::data::database_change_t::updated);
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
			ret.emplace_back(iter_cur.first, engine::data::database_change_t::added);

			mark_changed_directories(iter_cur.first, &ret, &changed_directories);
		}
	}

	return ret;
}

bool engine::data::item_content_base_t::is_destroyed()
{
	return !owner || owner->is_destroyed();
}

void engine::data::item_generic_t::destroy_self(item_content_base_t * content)
{
	this->content_destroyed = content;
	get_database_items()->perform_destroy(get_path());
}

void engine::data::database_items_t::save(std::shared_ptr<item_generic_t> item)
{
	{
		std::lock_guard<std::recursive_mutex> guard(operations_mutex);
		item->request_save();
		create_operation(item, database->get_output(item->get_path()), !item->do_not_log_operations());
	}
	execute_operations(item_operation_t::step_t::caller_t::sync);
}

void engine::data::database_items_t::reload(std::shared_ptr<item_generic_t> item)
{
	{
		std::lock_guard<std::recursive_mutex> guard(operations_mutex);
		item->request_load();
		create_operation(item, database->get_input(item->get_path()), !item->do_not_log_operations());
	}
	execute_operations(item_operation_t::step_t::caller_t::sync);
}

void engine::data::database_items_t::perform_destroy(const virtual_path_t & path)
{
	std::lock_guard<std::recursive_mutex> guard(mutex_items);
	auto iter = items.find(path);

	std::shared_ptr<item_generic_t > item;

	if (iter != items.end() && (item = std::static_pointer_cast<item_generic_t >(iter->second.lock())))
	{
		create_operation(item, item_operation_t::free_t(), !item->do_not_log_operations());
		execute_operations(item_operation_t::step_t::caller_t::sync);
	}
}

template<class T> std::shared_ptr<engine::data::item_t<T> > engine::data::database_items_t::load_item_detached(std::unique_ptr<input_t> input)
{
	std::shared_ptr<item_t<T> > ret;
	ret = item_t<T>::create_item(path);

	{
		std::lock_guard<std::recursive_mutex> guard(operations_mutex);
		item->request_load();
		create_operation(item, std::move(input), !item->do_not_log_operations());
	}
	execute_operations(item_operation_t::step_t::caller_t::sync);
}

void engine::data::database_items_t::update_async()
{
	execute_operations(item_operation_t::step_t::caller_t::async);

	{
		std::lock_guard<std::recursive_mutex> guard(operations_mutex);
		for (auto & it : items)
		{
			auto item = it.second.lock();
			if (item && !item->is_operation_pending())
			{
				if (item->is_requested_load())
					create_operation(item, database->get_input(item->get_path()), !item->do_not_log_operations());
				else if (item->is_requested_save())
					create_operation(item, database->get_output(item->get_path()), !item->do_not_log_operations());
			}
		}

		clear_completed_operations();
	}
}


void engine::data::database_items_t::init_update()
{
	auto & changes = database->get_changes();

	for (auto & change : changes)
	{
		auto iter = items.find(change.get_path());
		std::shared_ptr<item_generic_t> item;

		if (iter != items.end() && (item = std::static_pointer_cast<item_generic_t>(iter->second.lock())))
		{
			if (!item->is_deatached() && !item->do_not_auto_reload())
			{
				auto type = change.get_type();

				if (type == database_change_t::added || type == database_change_t::updated)
				{
					reload(item);
				}
				else if (type == database_change_t::deleted)
				{
					item->destroy();
				}
			}
		}
	}

	execute_operations(item_operation_t::step_t::caller_t::sync);
	execute_operations(item_operation_t::step_t::caller_t::sync_force_main_thread);
}

/*

#include "common/data/provider_asset.hpp"

engine::data::provider_asset_t::info_t::info_t(const virtual_path_t::path_t & path) : flags(flag_none)
{
	std::string src = path.to_utf8();

	std::size_t first_slash = src.find('/');
	if (first_slash != std::string::npos)
	{
		std::string tmp = src.substr(0, first_slash);
		owner_name = ustring_t::from_utf8(tmp.c_str());
		src = src.substr(first_slash + 1);
	}
	else
	{
		owner_name = path;
		return;
	}

	if (src.find('!') != std::string::npos)
	{
		set_flag(flag_deleted, true);
	}
	
	src.erase(std::remove(src.begin(), src.end(), '!'), src.end());

	std::size_t lang_pos = src.rfind('#');
	std::size_t tag_pos = src.rfind('^');

	if (lang_pos != std::string::npos && tag_pos != std::string::npos)
	{
		std::size_t end_pos_lang = find_next_separator(src, lang_pos);
		std::size_t end_pos_tag = find_next_separator(src, tag_pos);

		std::size_t end_pos_min = std::min(end_pos_lang, end_pos_tag);
		std::size_t end_pos_max = std::max(end_pos_lang, end_pos_tag);

		if (lang_pos < tag_pos)
		{
			{
				std::size_t end_pos = std::min(tag_pos, end_pos_min);
				std::string type = src.substr(lang_pos + 1, end_pos - lang_pos - 1);
				lang = ustring_t::from_utf8(type.c_str());
			}
			{
				std::size_t end_pos = end_pos_tag;
				std::string type = src.substr(tag_pos + 1, end_pos - tag_pos - 1);
				tag = ustring_t::from_utf8(type.c_str());
			}
		}
		else 
		{
			{
				std::size_t end_pos = std::min(lang_pos, end_pos_min);
				std::string type = src.substr(tag_pos + 1, end_pos - tag_pos - 1);
				tag = ustring_t::from_utf8(type.c_str());
			}
			{
				std::size_t end_pos = end_pos_lang;
				std::string type = src.substr(lang_pos + 1, end_pos - lang_pos - 1);
				lang = ustring_t::from_utf8(type.c_str());
			}
		}
	}
	else if (lang_pos != std::string::npos)
	{
		std::size_t end_pos = find_next_separator(src, lang_pos);
		std::string type = src.substr(lang_pos + 1, end_pos - lang_pos - 1);
		lang = ustring_t::from_utf8(type.c_str());
	}
	else if (tag_pos != std::string::npos)
	{
		std::size_t end_pos = find_next_separator(src, tag_pos);
		std::string type = src.substr(tag_pos + 1, end_pos - tag_pos - 1);
		tag = ustring_t::from_utf8(type.c_str());
	}
}

engine::virtual_path_t::path_t engine::data::provider_asset_t::info_t::clean_path(const virtual_path_t::path_t & path)
{
	std::string src = path.to_utf8();

	std::size_t first_slash = src.find('/');
	if (first_slash != std::string::npos)
		src = src.substr(first_slash + 1);
	else
		return engine::virtual_path_t::path_t();

	src.erase(std::remove(src.begin(), src.end(), '!'), src.end());

	std::size_t iter = 0;
	
	std::string ret;

	while (iter < src.size())
	{
		std::size_t lang_pos = src.find('#', iter);
		std::size_t tag_pos = src.find('^', iter);
		std::size_t end_pos = src.find('/', iter + 1); if (end_pos == std::string::npos) { end_pos = src.find('.', iter + 1); } if (end_pos == std::string::npos) { end_pos = src.size(); }

		if (lang_pos == std::string::npos && tag_pos == std::string::npos)
		{
			ret += src.substr(iter);
			iter = src.size();
		}
		else if (lang_pos != std::string::npos)
		{
			ret += src.substr(iter, lang_pos - iter);
			iter = end_pos;
		}
		else if (tag_pos != std::string::npos)
		{
			ret += src.substr(iter, tag_pos - iter);
			iter = end_pos;
		}
		else
		{
			ret += src.substr(iter, std::min(lang_pos, tag_pos) - iter);
			iter = end_pos;
		}
	}

	return ustring_t::from_utf8(ret.c_str());
}

*/