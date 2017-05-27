#include "common/data/database.hpp"
#include "common/data/database_items.hpp"
#include "common/data/item.hpp"
#include "common/platform.hpp"

void engine::data::item_content_base_t::set_dirty()
{
	owner->set_dirty();
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

void engine::data::item_content_base_t::save_item(item_t & item)
{

}

void engine::data::item_content_base_t::load_item(item_t & item)
{
	if (!is_reloading()) return;


}

engine::data::item_content_base_t::result_t engine::data::item_t::reload_sync(std::unique_ptr<input_t> input, database_items_t * database_items, bool force)
{
	auto database = database_items->get_database().get();

	item_content_base_t::result_t ret = item_content_base_t::result_t::success;

	if ((ret = content->reload_init(std::move(input), database, force)) != item_content_base_t::result_t::success)
		return ret;
	if ((ret = content->reload_async(database_items)) != item_content_base_t::result_t::success)
		return ret;
	if ((ret = content->reload_end(database)) != item_content_base_t::result_t::success)
		return ret;

	clear_placeholder_flag();

	return item_content_base_t::result_t::success;
}

engine::data::item_content_base_t::result_t engine::data::item_t::reload_async_init(std::unique_ptr<input_t> input, database_items_t * database_items, bool force)
{
	auto database = database_items->get_database().get();
	auto policy = content->get_reload_policy();

	if (policy == item_content_base_t::policy_io_t::implicit_async_copy || policy == item_content_base_t::policy_io_t::explicit_async_copy)
		content_load = std::move(create_content_func(get_path(), this));

	return get_content_load()->reload_init(std::move(input), database, force);
}

engine::data::item_content_base_t::result_t engine::data::item_t::reload_async(database_items_t * database_items)
{
	return get_content_load()->reload_async(database_items);
}

engine::data::item_content_base_t::result_t engine::data::item_t::reload_async_end(database_items_t * database_items)
{
	auto database = database_items->get_database().get();
	auto policy = content->get_reload_policy();

	item_content_base_t::result_t ret = item_content_base_t::result_t::success;

	if ((ret = get_content_load()->reload_end(database)) != item_content_base_t::result_t::success)
		return ret;

	if (policy == item_content_base_t::policy_io_t::implicit_async_copy || policy == item_content_base_t::policy_io_t::explicit_async_copy)
		std::swap(content, content_load);

	clear_placeholder_flag();

	return item_content_base_t::result_t::success;
}

void engine::data::database_items_t::reload(std::shared_ptr<item_t> item, mode_reload_t mode)
{
	auto policy = item->get_base()->get_reload_policy();
	std::thread::id calling_thread_id = std::this_thread::get_id();
	item_content_base_t::result_t result;

	if (policy == item_content_base_t::policy_io_t::forbidden)
	{
		return;
	}
	else if (mode == mode_reload_t::deffered)
	{
		items_deffered.push(item);
	}
	else if (mode == mode_reload_t::force_sync || mode == mode_reload_t::force_sync_now || policy == item_content_base_t::policy_io_t::implicit_sync || policy == item_content_base_t::policy_io_t::explicit_sync)
	{
		if (mode == mode_reload_t::force_sync_now || calling_thread_id == main_thread_id)
		{
			result = item->reload_sync(database->get_input(item->get_path()), this, (mode == mode_reload_t::force_sync_now));
			if (result == item_content_base_t::result_t::already_started)
			{
				items_reload_next.push(item);
			}
		}
		else
		{
			items_to_reload.enqueue_to_sync(item);
		}
	}
	else
	{
		if (calling_thread_id != main_thread_id)
		{
			items_to_reload.enqueue_to_init(item);
		}
		else
		{
			result = item->reload_async_init(database->get_input(item->get_path()), this);
			if (result == item_content_base_t::result_t::already_started)
				items_reload_next.push(item);
			else if(result == item_content_base_t::result_t::success)
				items_to_reload.enqueue_to_async(item);
		}
	}
}

void engine::data::database_items_t::init_update()
{
	item_content_base_t::result_t result;

	if (!items_deffered.is_empty()) // one per frame!!!
	{
		auto item = items_deffered.pop();
		reload(item);
	}

	auto & changes = database->get_changes();

	for (auto & change : changes)
	{
		auto iter = items.find(change.get_path());
		std::shared_ptr<item_t> item;

		if (iter != items.end() && (item = std::static_pointer_cast<item_t>(iter->second.lock())))
		{
			auto policy = item->content->get_reload_policy();
			if (policy == item_content_base_t::policy_io_t::implicit_sync ||
				policy == item_content_base_t::policy_io_t::implicit_async ||
				policy == item_content_base_t::policy_io_t::implicit_async_copy)
			{
				auto type = change.get_type();

				if (type == database_change_t::added  || type == database_change_t::updated)
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

	while (!items_reload_next.is_empty())
	{
		auto & item = items_reload_next.pop();
		if (!item->is_reloading())
		{
			reload(item);
		}
		else
		{
			items_reload_next.push(item);
			break;
		}
	}

	for (;;)
	{
		std::shared_ptr<item_t> next_item = items_to_reload.get_item_to_sync();

		if (next_item)
		{
			result = next_item->reload_sync(database->get_input(next_item->get_path()), this);
			if(result == item_content_base_t::result_t::already_started)
				items_reload_next.push(next_item);
		}
		else
			break;
	}

	for (;;)
	{
		std::shared_ptr<item_t> next_item = items_to_reload.get_item_to_end();

		if (next_item)
			next_item->reload_async_end(this);
		else
			break;
	}

	for (;;)
	{
		std::shared_ptr<item_t> next_item = items_to_reload.get_item_to_init();

		if (next_item)
		{
			result = next_item->reload_async_init(database->get_input(next_item->get_path()), this);
			if (result == item_content_base_t::result_t::already_started)
			{
				items_reload_next.push(next_item);
			}
			else if(result == item_content_base_t::result_t::success)
				items_to_reload.enqueue_to_async(next_item);
		}
		else
			break;
	}
}

bool engine::data::item_t::is_reloading()
{
	if (content->is_io_pending()) return true;

	auto policy = content->get_reload_policy();
	if (policy == item_content_base_t::policy_io_t::implicit_async_copy || policy == item_content_base_t::policy_io_t::explicit_async_copy)
	{
		if (content_load)
			return content_load->is_io_pending();
	}
	return false;
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