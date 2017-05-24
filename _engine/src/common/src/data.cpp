#include "common/data/database.hpp"
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