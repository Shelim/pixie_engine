#if 0

#include "common/asset/path.hpp"
#include "common/asset/item.hpp"
#include "common/asset/item_holder.hpp"
#include "common/asset/collection.hpp"

void engine::asset::item_collection_holder_t::resolve_collection(std::shared_ptr<database_providers_t> database)
{
	/*
	if (!collection && database && !is_deatached())
		collection = database->load_collection(path);
	*/
}

void engine::asset::item_holder_t::resolve_item(std::shared_ptr<database_providers_t> database)
{
	/*
	if (!is_deatached() && database)
		item = database->load_item(path);
	*/
}

engine::asset::path_t::path_t(const virtual_path_t & path_real) : path_real(path_real), owner_type(type_t::unknown)
{
	switch (path_real.get_type())
	{
	case virtual_path_t::type_t::common: owner_type = type_t::common; break;
	case virtual_path_t::type_t::modules: owner_type = type_t::modules; break;
	case virtual_path_t::type_t::submodules: owner_type = type_t::submodules; break;
	}

	std::string src = path_real.get_path().to_lower().to_utf8();

	if (owner_type != type_t::common)
	{
		std::size_t first_slash = src.find('/');
		if (first_slash != std::string::npos)
		{
			std::string tmp = src.substr(0, first_slash);
			owner = ustring_t::from_utf8(tmp.c_str());
			src = src.substr(first_slash + 1);
		}
		else
		{
			owner = ustring_t::from_utf8(src.c_str());
			return;
		}
	}

	if (src.find('!') != std::string::npos)
	{
		set_flag(flag_t::deleted, true);
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

	path_canonized = clean_path(src, (owner_type == type_t::common));

	int index = path_canonized.last_index_of_utf8(u8"/");
	name = path_canonized.substr(index + 1);
}

engine::asset::path_t::canonized_t engine::asset::path_t::clean_path(std::string src, bool is_common)
{
	if (!is_common)
	{
		std::size_t first_slash = src.find('/');
		if (first_slash != std::string::npos)
			src = src.substr(first_slash + 1);
		else
			return engine::virtual_path_t::path_t();
	}

	src.erase(std::remove(src.begin(), src.end(), '!'), src.end());

	std::size_t iter = 0;

	std::string ret;

	while (iter < src.size())
	{
		std::size_t lang_pos = src.find('#', iter);
		std::size_t tag_pos = src.find('^', iter);
		std::size_t end_pos = find_next_separator(src, iter + 1);

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

#if 0

#include "common/asset.hpp"
#include "common/xml_pixie_archive.hpp"
#include "common/modules.hpp"

engine::ustring_t engine::asset_loader_t::read_all()
{
	const std::size_t PACKET_SIZE = 4096;
	std::string ret;

	while (!is_eof())
	{
		uint8_t packet[PACKET_SIZE + 1];

		uint32_t size = read(packet, PACKET_SIZE);
		packet[size] = 0;
		ret.append(reinterpret_cast<char*>(packet));
	}

	return ustring_t::from_utf8(ret.c_str());
}

engine::asset_loader_t::buffer_t engine::asset_loader_t::read_buffer()
{
	const std::size_t PACKET_SIZE = 4096;
	std::vector<uint8_t> ret;

	while (!is_eof())
	{
		uint8_t packet[PACKET_SIZE + 1];

		uint32_t size = read(packet, PACKET_SIZE);
		packet[size] = 0;
		ret.insert(ret.end(), packet, packet + size);
	}

	return ret;
}

void engine::asset_description_t::parse_filename(const filesystem::path & path)
{
	filesystem::path vpath;

	engine::virtual_path_t can_path = canonize_ipath(path);
	filesystem::path input_path = can_path.get_cstring();

	for (auto & item : input_path)
	{
		ustring_t str = ustring_t::from_utf8(item.u8string().c_str());
		if (str.index_of('!') != -1) set_flag(flag_deleted, true);

		int_fast32_t in;
		while ((in = str.index_of('!')) != -1)
		{
			ustring_t prior = str.substr(0, in);
			ustring_t post = str.substr(in + 1);
			str = prior; str.append(post);
		}

		int_fast32_t lang_pos = str.index_of('#') + 1;
		int_fast32_t tag_pos = str.index_of('^') + 1;
		int_fast32_t end_pos = str.index_of('.') + 1; if (end_pos == 0) end_pos = str.len();

		if (lang_pos == 0 && tag_pos == 0)
		{
			vpath /= str.get_cstring();
		}
		else if (lang_pos == 0)
		{
			tag = str.substr(tag_pos, end_pos - tag_pos - 1);

			ustring_t p = str.substr(0, tag_pos - 1);
			p.append(str.substr(end_pos - 1));
			vpath /= p.get_cstring();
		}
		else if (tag_pos == 0)
		{
			lang = str.substr(lang_pos, end_pos - lang_pos - 1);

			ustring_t p = str.substr(0, lang_pos - 1);
			p.append(str.substr(end_pos - 1));
			vpath /= p.get_cstring();
		}
		else
		{
			if (lang_pos < tag_pos)
			{
				tag = str.substr(tag_pos, end_pos - tag_pos - 1);
				lang = str.substr(lang_pos, tag_pos - lang_pos - 1);
			}
			else
			{
				tag = str.substr(tag_pos, lang_pos - tag_pos - 1);
				lang = str.substr(lang_pos, end_pos - lang_pos - 1);
			}

			ustring_t p = str.substr(0, std::min(tag_pos, lang_pos) - 1);
			p.append(str.substr(end_pos - 1));
			vpath /= p.get_cstring();
		}
	}

	virtual_path = canonize_path(vpath);
}

engine::asset_problem_t engine::asset_description_collection_t::get_merge_problem(const asset_description_collection_t & other_col)
{
	for (auto & iter : other_col.asset_descriptions_all)
	{
		auto iter2_1 = asset_descriptions.find(iter->get_tag());
		if (iter2_1 != asset_descriptions.end())
		{
			auto iter2_2 = iter2_1->second.find(iter->get_lang());
			if (iter2_2 != iter2_1->second.end())
			{
				if (!other_col.is_deleted() && !is_deleted())
					return asset_problem_t(asset_problem_t::double_definitions, iter->get_virtual_path(), iter->to_string(), iter2_2->second.begin()->second->to_string());
				else if (other_col.is_deleted() != is_deleted())
					return asset_problem_t(asset_problem_t::defined_and_deleted, iter->get_virtual_path(), iter->to_string(), iter2_2->second.begin()->second->to_string());
			}
		}
	}
	return asset_problem_t();
}

std::shared_ptr<engine::asset_description_t> engine::asset_description_collection_t::get_desc_no_fallback(const ustring_t & tag, const ustring_t & lang)
{
	if (is_flag(flags, asset_description_t::flag_deleted)) return nullptr;

	auto tag_value = asset_descriptions.find(tag);
	if (tag_value == asset_descriptions.end())
	{
		return nullptr;
	}

	auto lang_value = tag_value->second.find(lang);
	if (lang_value == tag_value->second.end())
	{
		return nullptr;
	}

	std::shared_ptr<asset_description_t> ret;

	for (auto & iter : lang_value->second)
	{
		if (is_flag(iter.first, asset_description_t::flag_deleted))
			return nullptr;
		else
			ret = iter.second;
	}
	return ret;
}

std::shared_ptr<engine::asset_description_t> engine::asset_description_collection_t::get_desc(const ustring_t & tag, const ustring_t & lang)
{
	if (is_flag(flags, asset_description_t::flag_deleted)) return nullptr;

	auto tag_value = asset_descriptions.find(tag);
	if (tag_value == asset_descriptions.end())
	{
		if (tag.is_empty()) return nullptr;
		return get_desc(_U(""), lang);
	}

	auto lang_value = tag_value->second.find(lang);
	if (lang_value == tag_value->second.end())
	{
		if (lang.is_empty()) return nullptr;
		return get_desc(tag, _U(""));
	}

	std::shared_ptr<asset_description_t> ret;

	for (auto & iter : lang_value->second)
	{
		if (is_flag(iter.first, asset_description_t::flag_deleted))
		{
			if (tag.is_empty() && lang.is_empty()) return nullptr;
			else if (!tag.is_empty() && !lang.is_empty()) return get_desc(tag, _U(""));
			else return get_desc(_U(""), _U(""));
		}
		else
		{
			ret = iter.second;
		}
	}

	return ret;
}

std::streambuf::int_type engine::asset_streambuf_t::underflow()
{

	uint8_t c;
	loader->read(&c, 1);

	if (loader->is_eof())
	{
		loader->go_back(1);
		return traits_type::eof();
	}

	loader->go_back(1);

	return traits_type::to_int_type(c);
}

std::streambuf::int_type engine::asset_streambuf_t::uflow()
{
	uint8_t c;
	loader->read(&c, 1);

	if (loader->is_eof())
		return traits_type::eof();

	return traits_type::to_int_type(c);
}

std::streambuf::int_type engine::asset_streambuf_t::pbackfail(int_type ch)
{
	uint8_t c;
	loader->go_back(1);
	loader->read(&c, 1);
	loader->go_back(1);

	return traits_type::to_int_type(c);
}

std::streamsize engine::asset_streambuf_t::xsgetn(char* s, std::streamsize n)
{
	return loader->read(reinterpret_cast<uint8_t*>(s), n);
}

std::vector<engine::virtual_path_t> engine::asset_database_base_t::iterate_directories(const virtual_path_t & path)
{
	std::lock_guard<std::recursive_mutex> guard(mutex_files);
	auto iter = directories.find(canonize_ipath(path));
	if (iter != directories.end())
		return iter->second;
	return std::vector<virtual_path_t>();
}

std::vector<engine::virtual_path_t> engine::asset_database_base_t::iterate_files(const virtual_path_t & path, const ustring_t & ext)
{
	std::lock_guard<std::recursive_mutex> guard(mutex_files);
	std::vector<virtual_path_t> ret;
	ustring_t ext_lower = ext.to_lower();
	auto iter = files.find(canonize_ipath(path));
	if (iter != files.end())
	{
		for (auto iter2 = iter->second.begin(); iter2 != iter->second.end(); iter2++)
		{
			int index_of = iter2->last_index_of('.');

			if (index_of == -1)
			{
				if(ext_lower.is_empty())
					ret.push_back(*iter2);
			}
			else if (iter2->substr(index_of) == ext_lower)
				ret.push_back(*iter2);
		}
	}
	return ret;
}

template<class T> void engine::asset_collection_t<T>::reload(engine::asset_manager_t * asset_manager, std::shared_ptr<logger_t> logger)
{
	std::vector<filesystem::path> filenames;

	/*
	if (extension.empty())
		filenames = asset_manager_t->iterate_directories(get_virtual_path());
	else
		filenames = asset_manager_t->iterate_files(get_virtual_path(), extension);
		*/
	for (auto item = items.begin(); item != items.end();)
	{
		filesystem::path path = std::static_pointer_cast<asset_t>(*item)->get_virtual_path();
		if (path.has_filename()) path = path.filename();

		if (std::find(filenames.begin(), filenames.end(), path) == filenames.end())
		{
			items_named.erase((*item)->get_name().to_lower());
			remove(*item);
			item = items.erase(item);
		}
		else
			item++;
	}

	for (auto filename = filenames.begin(); filename != filenames.end(); filename++)
	{
		bool found = false;
		for (auto item = items.begin(); item != items.end(); item++)
		{
			filesystem::path path = std::static_pointer_cast<asset_t>(*item)->get_virtual_path();

			if (path.has_filename() && path.filename() == *filename)
			{
				found = true;
				break;
			}
		}
		if (!found)
		{
			filesystem::path full_filename = get_virtual_path();
			full_filename /= *filename;

			std::shared_ptr<T> asset = nullptr; // asset_manager_t->load<T>(full_filename);
			items_named[asset->get_name().to_lower()] = asset;
			items.push_back(asset);
			add(asset);
		}
	}
}

void engine::asset_scanner_t::end_scan()
{
	virtual_path_t path;
	bool is_deleted = false;

	for (auto & iter : assets)
	{
		virtual_path_t sec = iter.first;
		if (iter.second.is_deleted() && !is_deleted)
		{
			path = iter.first;
			is_deleted = true;
		}
		else if (is_deleted && sec.substr(0, path.len()) == path && sec.len() > path.len() && (sec[path.len()] == '/'))
		{
			iter.second.delete_all();
		}
		else
		{
			is_deleted = false;
		}
	}
}

void engine::asset_scanner_files_t::scan()
{
	begin_scan();
	std::size_t len = get_path().u8string().length();

	for (auto & iter : filesystem::recursive_directory_iterator(get_path()))
	{
		filesystem::path path_relative = iter.path().u8string().substr(len + 1);

		if(filesystem::is_directory(iter.status()))
		{
			add(std::make_shared<asset_description_directory_t>(iter.path(), path_relative));
		}
		else if (filesystem::is_regular_file(iter.status()))
		{
			if (iter.path().filename() == "_manifest.info")
				continue;
			add(std::make_shared<asset_description_file_t>(iter.path(), path_relative));
		}
	}

	end_scan();
}

void engine::asset_scanner_t::overwrite(desc_collection_t & desc_collection1, const desc_collection_t & desc_collection2)
{
	for (auto & iter : desc_collection2)
	{
		desc_collection1[iter.first].add(iter.second);
	}
}

engine::asset_problems_t engine::asset_scanner_t::merge(desc_collection_t & desc_collection1, const desc_collection_t & desc_collection2)
{
	asset_problems_t ret;
	for (auto & iter : desc_collection2)
	{
		asset_problem_t problem = desc_collection1[iter.first].get_merge_problem(iter.second);

		if (problem.get_type() != asset_problem_t::none)
			ret.push_back(problem);

		desc_collection1[iter.first].add(iter.second);
	}
	return ret;
}

void engine::asset_database_base_t::add_desc_collection(const virtual_path_t & dir, std::unique_ptr<asset_description_collection_t> desc)
{
	assets_desc[dir] = std::move(desc);

	if(dir.index_of('.') != -1)
		add_file(dir);
	else
		add_directory(dir);
}

void engine::asset_database_base_t::add_file(const virtual_path_t & dir)
{
	int index = dir.last_index_of('/');

	virtual_path_t file_dir = _U("");
	virtual_path_t file_name = dir;

	if (index > 0)
	{
		file_dir = dir.substr(0, index - 1);
		file_name = dir.substr(index + 1);
	}

	files[file_dir].push_back(file_name);

	add_directory(file_dir);
}

void engine::asset_database_base_t::add_directory(const virtual_path_t & dir)
{
	int index = dir.last_index_of('/');

	virtual_path_t path = _U("");
	virtual_path_t name = dir;

	if (index > 0)
	{
		path = dir.substr(0, index - 1);
		name = dir.substr(index + 1);
	}

	directories[path].push_back(name);

	if(!path.is_empty())
		add_directory(path);
}

void engine::asset_database_base_t::updated_directory(const ustring_t & dir, engine::asset_changes_t & changes, std::set<engine::ustring_t> & dir_already)
{
	ustring_collection_t directories;
	int oldindex = 0;
	int newindex = 0;
	while ((newindex = dir.index_of_utf8(u8"/", oldindex)) != -1)
	{
		directories.push_back(dir.substr(0, newindex));
		oldindex = newindex + 1;
	}

	for (auto & d : directories)
	{
		if (dir_already.find(d) == dir_already.end())
		{
			changes.emplace_back(d, asset_change_t::type_t::changed_directory);
			dir_already.insert(d);
		}
	}
}

engine::asset_changes_t engine::asset_database_base_t::get_changes_since(asset_database_base_t * old)
{
	engine::asset_changes_t ret;
	std::lock_guard<std::recursive_mutex> guard2(mutex_desc);
	std::set<engine::ustring_t> dir_already;

	for (auto & iter : old->assets_desc)
	{
		if (iter.second->is_deleted()) continue;

		auto iter_new = assets_desc.find(iter.first);

		bool deleted = (iter_new == assets_desc.end());
		bool updated = deleted;

		if (!deleted)
		{
			for (auto & iter2 : iter.second->get_all())
			{
				auto other = iter_new->second->get_desc_no_fallback(iter2->get_tag(), iter2->get_lang());
				if (!other || other->get_time() != iter2->get_time())
					updated = true;
			}
			for (auto & iter2 : iter_new->second->get_all())
			{
				auto other = iter.second->get_desc_no_fallback(iter2->get_tag(), iter2->get_lang());
				if (!other)
					updated = true;
			}
		}

		if (updated)
		{
			if (deleted)
			{
				ret.emplace_back(iter.first, asset_change_t::type_t::deleted);
			}
			else if (updated)
			{
				ret.emplace_back(iter.first, asset_change_t::type_t::updated);
			}
			if (deleted || updated)
			{
				updated_directory(iter.first, ret, dir_already);
			}
		}
	}

	for (auto & iter : assets_desc)
	{
		if (iter.second->is_deleted()) continue;

		auto iter_new = old->assets_desc.find(iter.first);

		if (iter_new == old->assets_desc.end() || iter_new->second->is_deleted())
		{
			ret.emplace_back(iter.first, asset_change_t::type_t::added);

			updated_directory(iter.first, ret, dir_already);
		}
	}

	return ret;
}

void engine::asset_database_game_t::rescan_local(asset_scanner_t::desc_collection_t & output)
{
	scan_path_overwriting(_U("engine/game"), output);
	scan_path_overwriting(_U("common"), output);

	module_resolver->check_for_reload();
	if(!has_updated_since() &&
		(module_resolver->is_reload_requested()))
	{
		set_updated();
	}
	module_resolver->reload();

	auto state = module_resolver->get_state(config);

	for (auto iter = state.get_modules().rbegin(); iter != state.get_modules().rend(); iter++)
	{
		ustring_t path_to_module = _U("modules/");
		path_to_module.append(*iter);
		scan_path_overwriting(path_to_module, output);
	}

	asset_scanner_t::desc_collection_t sub_modules;

	for (auto & iter : state.get_sub_modules())
	{
		ustring_t path_to_sub_module = _U("sub_modules/");
		path_to_sub_module.append(iter);
		scan_path_merging(path_to_sub_module, sub_modules);
	}

	asset_scanner_t::overwrite(output, sub_modules);

	problems_state.clear();
	problems_state.insert(problems_state.end(), state.get_problems().begin(), state.get_problems().end());
}

#endif
#endif