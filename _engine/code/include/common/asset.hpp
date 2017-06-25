#if 0
#ifndef ENGINE_COMMON_ASSET_HPP
#define ENGINE_COMMON_ASSET_HPP
#pragma once

#include "common/platform.hpp"
#include "common/logger.hpp"
#include "common/config.hpp"
#include "common/filesystem.hpp"
#include "common/modules.hpp"
#include "common/utility/text/ustring.hpp"
#include <fstream>
#include <map>
#include <set>
#include <bitset>
#include <pugixml.hpp>
#include <memory>
#include <cereal/archives/xml.hpp>
#include <cereal/access.hpp>

namespace engine
{
	class asset_manager_t
	{

	};

	class asset_loader_t
	{

	public:

		virtual void skip(int32_t pos) = 0;
		virtual void go_back(int32_t pos) = 0;
		virtual uint32_t read(uint8_t * buffer, uint32_t size) = 0;
		virtual bool is_eof() = 0;

		virtual ~asset_loader_t() { }

		ustring_t read_all();

		typedef std::vector<uint8_t> buffer_t;

		buffer_t read_buffer();

		engine::asset_manager_t * get_asset_manager() const
		{
			return asset_manager;
		}

		asset_loader_t(asset_loader_t const&) = delete;
		asset_loader_t(asset_loader_t &&) = delete;
		asset_loader_t& operator=(asset_loader_t &&) = delete;
		asset_loader_t& operator=(asset_loader_t const&) = delete;

	protected:

		asset_loader_t(engine::asset_manager_t * asset_manager) : asset_manager(asset_manager)
		{

		}

	private:

		engine::asset_manager_t * asset_manager;

	};

	class asset_streambuf_t : public std::streambuf
	{

	public:

		asset_streambuf_t(asset_loader_t * loader) : loader(loader)
		{

		}

		int_type underflow() final;
		int_type uflow() final;

		int_type pbackfail(int_type ch) final;

		std::streamsize xsgetn(char* s, std::streamsize n) final;

	private:

		asset_loader_t * loader;

	};

	class asset_manager_t;

	class asset_description_t
	{

	public:

		virtual std::unique_ptr<asset_loader_t> construct_loader(engine::asset_manager_t * asset_manager) = 0;
		virtual ~asset_description_t() { }
		virtual ustring_t to_string() = 0;
		filesystem::file_time_type get_time() const
		{
			return time;
		}

		asset_description_t(asset_description_t const&) = delete;
		asset_description_t(asset_description_t &&) = delete;
		asset_description_t& operator=(asset_description_t &&) = delete;
		asset_description_t& operator=(asset_description_t const&) = delete;

		enum class flag_t
		{
			none,
			deleted,

			count
		};

		bool is_flag(flag_t flag)
		{
			return flags.test(static_cast<std::size_t>(flag));
		}

		const ustring_t & get_tag() const
		{
			return tag;
		}

		const ustring_t & get_lang() const
		{
			return lang;
		}

		const virtual_path_t & get_virtual_path() const
		{
			return virtual_path;
		}

	protected:

		asset_description_t(filesystem::file_time_type time) : time(time) { }

		void parse_filename(const filesystem::path & path);

		void set_flag(flag_t flag, bool value)
		{
			flags.set(static_cast<std::size_t>(flag), value);
		}

		void set_tag(const ustring_t & value)
		{
			tag = value;
		}

		void set_lang(const ustring_t & value)
		{
			lang = lang;
		}

		void set_virtual_path(const virtual_path_t & value)
		{
			virtual_path = canonize_virtual(value);
		}

	private:

		filesystem::file_time_type time;
		virtual_path_t virtual_path;
		std::bitset < static_cast<std::size_t>(flag_t::count)> flags;
		ustring_t tag;
		ustring_t lang;
	}; 
	
	class asset_problem_t
	{

	public:

		enum type_t : uint8_t
		{
			none,
#define GAME_ASSET_PROBLEM_STD(x) x,
#include "common/std/problems_std.hpp"
			count
		};

		ustring_t get_type_key() const
		{
			switch (type)
			{
#define GAME_ASSET_PROBLEM_STD(x) case x: return _U(#x);
#include "common/std/problems_std.hpp"
			}
			return _U("unknown");
		}

		const ustring_t & get_scanner1() const
		{
			return scanner1;
		}

		const ustring_t & get_scanner2() const
		{
			return scanner2;
		}

		const virtual_path_t & get_path() const
		{
			return path;
		}
		
		type_t get_type() const
		{
			return type;
		}

		asset_problem_t() : type(none)
		{

		}

		asset_problem_t(type_t type, const virtual_path_t path, const ustring_t & scanner1, const ustring_t & scanner2 = _U("")) : scanner1(scanner1), scanner2(scanner2), path(path), type(type)
		{

		}

	private:

		ustring_t scanner1;
		ustring_t scanner2;

		virtual_path_t path;

		type_t type;

	};

	class asset_description_collection_t
	{

	public:

		asset_description_collection_t()
		{
			time = filesystem::file_time_type::min();
		}
		asset_description_collection_t(asset_description_collection_t const&) = default;
		asset_description_collection_t(asset_description_collection_t &&) = delete;
		asset_description_collection_t& operator=(asset_description_collection_t &&) = delete;
		asset_description_collection_t& operator=(asset_description_collection_t const&) = delete;

		void add(std::shared_ptr<asset_description_t> asset_description)
		{
			auto & item = asset_descriptions[asset_description->get_tag()][asset_description->get_lang()][asset_description->get_flags()];
			if(!item)
				asset_descriptions_all.push_back(asset_description);
			else
			{
				auto & desc_item_all = std::find(asset_descriptions_all.begin(), asset_descriptions_all.end(), item);
				asset_descriptions_all.erase(desc_item_all);
				asset_descriptions_all.push_back(asset_description);
			}

			item = asset_description;

			time = std::max(time, asset_description->get_time());

			if (asset_description->is_flag(asset_description_t::flag_t::deleted))
				delete_all();
		}

		void add(const asset_description_collection_t & other_col)
		{
			for (auto & iter : other_col.asset_descriptions_all)
			{
				add(iter);
			}
		}

		asset_problem_t get_merge_problem(const asset_description_collection_t & other_col);

		std::shared_ptr<asset_description_t> get_desc(const ustring_t & tag = _U(""), const ustring_t & lang = _U(""));
		std::shared_ptr<asset_description_t> get_desc_no_fallback(const ustring_t & tag = _U(""), const ustring_t & lang = _U(""));
		bool has_base_desc() const
		{
			auto iter = asset_descriptions.find(_U(""));
			if (iter != asset_descriptions.end())
			{
				auto iter2 = iter->second.find(_U(""));
				if (iter2 != iter->second.end())
				{
					return true;
				}
			}
			return false;
		}

		typedef std::vector< std::shared_ptr<asset_description_t> > desc_all_t;

		const desc_all_t & get_all() const
		{
			return asset_descriptions_all;
		}

		void delete_all()
		{
			set_flag(asset_description_t::flag_t::deleted, true);
		}

		bool is_deleted() const
		{
			return is_flag(asset_description_t::flag_t::deleted);
		}

		filesystem::file_time_type get_time()
		{
			return time;
		}

	private:

		typedef std::map <asset_description_t::flag_t, std::shared_ptr<asset_description_t> >  flag_map_t;
		typedef std::map <ustring_t, flag_map_t> lang_map_t;
		typedef std::map <ustring_t, lang_map_t> tag_map_t;

		desc_all_t asset_descriptions_all;

		void set_flag(asset_description_t::flag_t flag, bool value)
		{
			if (value)
				flags = static_cast<asset_description_t::flag_t>(flags | flag);
			else
				flags = static_cast<asset_description_t::flag_t>(flags & ~flag);
		}

		tag_map_t asset_descriptions;
		asset_description_t::flag_t flags;
		filesystem::file_time_type time;

	};

	class asset_loader_file_t : public asset_loader_t
	{

	public:

		asset_loader_file_t(engine::asset_manager_t * asset_manager, const filesystem::path & physical_path) : asset_loader_t(asset_manager), stream(to_string(physical_path).to_utf8(), std::ios_base::in | std::ios_base::binary)
		{
			
		}

		~asset_loader_file_t()
		{

		}

		void skip(int32_t pos) final
		{
			stream.seekg(pos, std::ios_base::cur);
		}
		void go_back(int32_t pos) final
		{
			stream.seekg(-pos, std::ios_base::cur);
		}
		uint32_t read(uint8_t * buffer, uint32_t size) final
		{
			return stream.read(reinterpret_cast<char *>(buffer), size).gcount();
		}
		bool is_eof() final
		{
			return stream.eof();
		}

	private:

		std::ifstream stream;

	};

	class asset_loader_directory_t : public asset_loader_t
	{


	public:

		asset_loader_directory_t(engine::asset_manager_t * asset_manager) : asset_loader_t(asset_manager)
		{

		}
		void skip(int32_t pos) final
		{

		}
		void go_back(int32_t pos) final
		{

		}
		uint32_t read(uint8_t * buffer, uint32_t size) final
		{
			return 0;
		}
		bool is_eof() final
		{
			return true;
		}
	};

	class asset_description_file_t : public asset_description_t
	{

	public:

		asset_description_file_t(const filesystem::path & physical_path, const filesystem::path & relative_path) : physical_path(physical_path), asset_description_t(filesystem::last_write_time(physical_path)) { parse_filename(relative_path); }
		std::unique_ptr<asset_loader_t> construct_loader(engine::asset_manager_t * asset_manager) final
		{
			return std::make_unique<asset_loader_file_t>(asset_manager, physical_path);
		}
		ustring_t to_string() final
		{
			return ustring_t::from_utf8(physical_path.u8string().c_str());
		}

	private:

		filesystem::path physical_path;
	};

	class asset_description_directory_t : public asset_description_t
	{

	public:

		asset_description_directory_t(const filesystem::path & physical_path, const filesystem::path & relative_path) : physical_path(physical_path), asset_description_t(filesystem::file_time_type::min()) { parse_filename(relative_path); }
		std::unique_ptr<asset_loader_t> construct_loader(engine::asset_manager_t * asset_manager) final
		{
			return std::make_unique<asset_loader_directory_t>(asset_manager);
		}
		ustring_t to_string() final
		{
			return ustring_t::from_utf8(physical_path.u8string().c_str());
		}

	private:

		filesystem::path physical_path;
	};

	class asset_t
	{

	public:

		virtual ~asset_t() {}

		const filesystem::path & get_virtual_path()
		{
			return virtual_path;
		}

		const ustring_t & get_name()
		{
			return name;
		}

		virtual bool is_deferrable()
		{
			return true;
		}

		asset_t(asset_t && other) = delete;
		asset_t& operator=(asset_t const&) = delete;
		asset_t& operator=(asset_t &&) = delete;

		friend class assets;

		bool is_destroyed() const
		{
			return destroyed;
		}

	protected:

		asset_t(const filesystem::path & virtual_path) : virtual_path(virtual_path), destroyed(false)
		{
			name = ustring_t::from_utf8(virtual_path.filename().u8string().c_str());
		}

		void destroy()
		{
			destroy_local();
			destroyed = true;
		}

	private:

		filesystem::path virtual_path;
		ustring_t name;

		virtual void reload_local(std::unique_ptr<asset_loader_t> loader, std::shared_ptr<logger_t> logger) = 0;
		virtual void destroy_local() { }

		bool destroyed;

		template<class T> friend class asset_deatachable_t;

		asset_t(asset_t const& other) : virtual_path(other.virtual_path), name(other.name), destroyed(other.destroyed)
		{

		}

		void reload(std::unique_ptr<asset_loader_t> loader, std::shared_ptr<logger_t> logger)
		{
			reload_local(std::move(loader), logger);
			destroyed = false;
		}

	};

	template<class T> class asset_deatachable_t : public asset_t, public std::enable_shared_from_this<asset_deatachable_t<T> >
	{
	public:

		bool is_deatached() const
		{
			return deatached;
		}

		std::shared_ptr<T> deatach()
		{
			if (is_deatached()) return std::static_pointer_cast<T>(this->shared_from_this());

			std::shared_ptr<T> ret = std::shared_ptr<T>(new T(*static_cast<T*>(this)));
			ret->deatach_local();
			return ret;
		}

		virtual ~asset_deatachable_t() {}

	protected:

		asset_deatachable_t(const ustring_t & virtual_path) : asset_t(virtual_path), deatached(false) { }

		asset_deatachable_t(asset_deatachable_t const& as) : asset_t(as), deatached(true)
		{

		}

	private:

		bool deatached;
		virtual void deatach_local() { }
	};

	class asset_collection_base_t
	{

	public:

		virtual ~asset_collection_base_t() {}

		asset_collection_base_t& operator=(asset_collection_base_t const&) = delete;

		const filesystem::path & get_virtual_path() const
		{
			return virtual_path;
		}

		friend class assets;

	protected:

		asset_collection_base_t(const filesystem::path & virtual_path) : virtual_path(virtual_path) { }

	private:

		template<class T> friend class asset_collection_t;

		virtual void reload(engine::asset_manager_t * assets, std::shared_ptr<logger_t> logger) = 0;

		filesystem::path virtual_path;

		asset_collection_base_t(asset_collection_base_t const& other) : virtual_path(other.virtual_path)
		{

		}

		virtual void after_reload(engine::asset_manager_t * assets)
		{

		}
	};

	template<class T> class asset_collection_t : public asset_collection_base_t, public std::enable_shared_from_this<asset_collection_t<T> >
	{

	public:

		static_assert(std::is_base_of<asset_t, T>::value, "Assets in collection must be of asset_t class");

		virtual ~asset_collection_t() {}

		asset_collection_t& operator=(asset_collection_t const&) = delete;

		friend class assets;

		typedef std::vector<std::shared_ptr<T> > items_t;

		const items_t & get_items() const
		{
			return items;
		}

		std::shared_ptr<T> get(const ustring_t & name, bool any_if_missing = false) const
		{
			auto & iter = items_named.find(name.to_lower());
			if (iter == items_named.end())
			{
				if (any_if_missing) return get_any();
				return nullptr;
			}

			return iter->second;

		}

		std::shared_ptr<T> get_any() const
		{
			if (items.empty()) return nullptr;
			return *(items.begin());
		}

	protected:

		asset_collection_t(const filesystem::path & virtual_path, const filesystem::path & extension = "") : asset_collection_base_t(virtual_path), extension(extension)
		{

		}

	private:

		items_t items;
		typedef std::map<ustring_t, std::shared_ptr<T> > items_named_t;
		items_named_t items_named;
		filesystem::path extension;

		template<class V> friend class asset_collection_deatachable_t;

		asset_collection_t(asset_collection_t const& other) : asset_collection_base_t(other), items(other.items), extension(other.extension)
		{

		}

		void reload(engine::asset_manager_t * asset_manager, std::shared_ptr<logger_t> logger) final;

		virtual void remove(std::shared_ptr<T> asset)
		{

		}

		virtual void add(std::shared_ptr<T> asset)
		{

		}

	};

	template<class T> class asset_collection_deatachable_t : public asset_collection_t<T>
	{
	public:

		bool is_deatached() const
		{
			return deatached;
		}

		template<class U> std::shared_ptr<U> deatach()
		{
			if (is_deatached()) return std::static_pointer_cast<U>(this->shared_from_this());

			std::shared_ptr<U> ret = std::shared_ptr<U>(new U(*static_cast<U*>(this)));
			for (std::size_t i = 0; i < ret->items.size(); i++)
			{
				ret->items_named[ret->items[i]->get_name()] = ret->items[i] = ret->items[i]->deatach();
			}
			ret->deatach_local();
			return ret;
		}

		virtual ~asset_collection_deatachable_t() {}

	protected:

		asset_collection_deatachable_t(const ustring_t & virtual_path, ustring_t extension = _U("")) : asset_collection_t<T>(virtual_path, extension), deatached(false) { }

		asset_collection_deatachable_t(asset_collection_deatachable_t const& as) : asset_collection_t<T>(as), deatached(true)
		{

		}

	private:

		bool deatached;
		virtual void deatach_local() { }
	};

	class asset_string_t : public asset_t
	{

	public:

		asset_string_t(const filesystem::path & virtual_path) :
			asset_t(virtual_path)
		{

		}

		~asset_string_t()
		{

		}

		const ustring_t & get()
		{
			return str;
		}

	private:

		void reload_local(std::unique_ptr<asset_loader_t> loader, std::shared_ptr<logger_t> logger) final
		{
			str = loader->read_all();
		}

		void destroy_local() final
		{
			str = _U("?!?");
		}

		ustring_t str;
	};

	typedef std::vector<asset_problem_t> asset_problems_t;

	class asset_scanner_t
	{

	public:

		asset_scanner_t(const filesystem::path & path) : path(path)
		{

		}

		virtual ~asset_scanner_t()
		{

		}

		typedef std::map<virtual_path_t, asset_description_collection_t> desc_collection_t;

		static void overwrite(desc_collection_t & desc_collection1, const desc_collection_t & desc_collection2);
		static asset_problems_t merge(desc_collection_t & desc_collection1, const desc_collection_t & desc_collection2);

		static void overwrite(desc_collection_t & desc_collection1, asset_scanner_t * scanner)
		{
			overwrite(desc_collection1, scanner->get_all());
		}
		static asset_problems_t merge(desc_collection_t & desc_collection1, asset_scanner_t * scanner)
		{
			return merge(desc_collection1, scanner->get_all());
		}

		const desc_collection_t & get_all() const
		{
			return assets;
		}

		const filesystem::path & get_path() const
		{
			return path;
		}

	protected:

		void begin_scan()
		{
			assets.clear();
		}

		void add(std::shared_ptr<asset_description_t> asset_description)
		{
			assets[asset_description->get_virtual_path()].add(asset_description);
		}

		void end_scan();

	private:

		filesystem::path path;
		desc_collection_t assets;
	};

	class asset_scanner_files_t : public asset_scanner_t
	{

	public:

		asset_scanner_files_t(const filesystem::path & path) : asset_scanner_t(path)
		{
			scan();
		}

	private:

		void scan();

	};


	class asset_change_t
	{

	public:

		enum type_t : uint8_t
		{
			added,
			updated,
			deleted,
			changed_directory
		};

		const virtual_path_t & get_path() const
		{
			return path;
		}

		type_t get_type() const
		{
			return type;
		}

		asset_change_t(const virtual_path_t & path, type_t type) : type(type), path(path)
		{

		}

	private:

		virtual_path_t path;
		type_t type;

	};

	typedef std::vector<asset_change_t> asset_changes_t;

	class asset_database_base_t
	{

	public:

		virtual ~asset_database_base_t()
		{

		}

		void rescan()
		{
			std::lock_guard<std::recursive_mutex> guard1(mutex_desc);
			std::lock_guard<std::recursive_mutex> guard2(mutex_files);
			asset_scanner_t::desc_collection_t output;
			problems.clear();

			rescan_local(output);

			for (auto & iter : output)
			{
				if (!iter.second.has_base_desc())
				{
					this->problems.emplace_back(asset_problem_t::no_base_item, iter.first, _U(""));
				}
			}

			assets_desc.clear();
			directories.clear();
			files.clear();
			for (auto & iter : output)
			{
				if (iter.second.has_base_desc())
					add_desc_collection(iter.first, std::make_unique<asset_description_collection_t>(iter.second));
			}
		}

		std::shared_ptr<asset_description_t> get_desc(const virtual_path_t & path, const ustring_t & tag = _U(""), const ustring_t & lang = _U(""))
		{
			std::lock_guard<std::recursive_mutex> guard2(mutex_desc);

			auto iter = assets_desc.find(path);
			if (iter == assets_desc.end()) return nullptr;

			return iter->second->get_desc(tag, lang);
		}

		filesystem::file_time_type get_time(const virtual_path_t & path)
		{
			std::lock_guard<std::recursive_mutex> guard2(mutex_desc);

			auto iter = assets_desc.find(path);
			if (iter == assets_desc.end()) return filesystem::file_time_type::min();

			return iter->second->get_time();
		}

		asset_changes_t get_changes_since(asset_database_base_t * old);
		bool has_updated_since()
		{
			return has_updated;
		}

		void clear_updated_since()
		{
			has_updated = false;
		}

		const asset_problems_t & get_problems()
		{
			return problems;
		}

		std::vector<virtual_path_t> iterate_directories(const virtual_path_t & path);
		std::vector<virtual_path_t> iterate_files(const virtual_path_t & path, const ustring_t & ext);
		
		std::shared_ptr<engine::platform_t> get_platform()
		{
			return platform;
		}

		typedef std::vector<std::unique_ptr<asset_scanner_t> > asset_scanners_t;

	protected:

		asset_database_base_t(std::shared_ptr<engine::platform_t> platform) : platform(platform), has_updated(false)
		{

		}

		void set_updated()
		{
			has_updated = true;
		}

		void raport_problems(asset_problems_t problems)
		{
			this->problems.insert(this->problems.end(), problems.begin(), problems.end());
		}

		asset_scanners_t get_scanners(const ustring_t & path)
		{
			asset_scanners_t ret;
			
			filesystem::path path_final = platform->get_exe_path();
			path_final /= path.get_cstring();

			for (auto& iter : filesystem::directory_iterator(path_final))
			{
				if (iter.status().type() == filesystem::file_type::regular)
				{
					std::string ext = iter.path().extension().u8string();
					ustring_t ext_lower = ustring_t::to_lower_utf8(ext.c_str());

					if (ext_lower == _U(".pixie_pack"))
					{
						// ToDo: Add asset_scanners for pack!
					}
				}
			}

			ret.push_back(std::make_unique<asset_scanner_files_t>(path_final));
			return ret;
		}
		
		void scan_path_overwriting(const ustring_t & path, asset_scanner_t::desc_collection_t & output)
		{
			auto scanners = get_scanners(path);

			for (auto & iter : scanners)
			{
				asset_scanner_t::overwrite(output, iter.get());
			}
		}

		void scan_path_merging(const ustring_t & path, asset_scanner_t::desc_collection_t & output)
		{
			auto scanners = get_scanners(path);

			for (auto & iter : scanners)
			{
				raport_problems(asset_scanner_t::merge(output, iter.get()));
			}
		}

		void scan_merge(asset_scanner_t::desc_collection_t & output, asset_scanner_t::desc_collection_t & to_merge)
		{
			raport_problems(asset_scanner_t::merge(output, to_merge));
		}

		std::recursive_mutex mutex_desc;
		std::recursive_mutex mutex_files;

	private:

		asset_problems_t problems;

		virtual void rescan_local(asset_scanner_t::desc_collection_t & output) = 0;

		void add_desc_collection(const virtual_path_t & dir, std::unique_ptr<asset_description_collection_t> desc);
		void add_file(const virtual_path_t & dir);
		void add_directory(const virtual_path_t & dir);

		std::shared_ptr<engine::platform_t> platform;
		
		void updated_directory(const ustring_t & dir, engine::asset_changes_t & changes, std::set<engine::ustring_t> & dir_already);

		typedef std::map<virtual_path_t, std::unique_ptr<asset_description_collection_t>> assets_desc_t;
		typedef std::map<virtual_path_t, std::vector<virtual_path_t> > directories_t;
		typedef std::map<virtual_path_t, std::vector<virtual_path_t> > files_t;

		assets_desc_t assets_desc;
		directories_t directories;
		files_t files;

		bool has_updated;

	};

	class asset_database_simple_t : public asset_database_base_t
	{

	public:

		asset_database_simple_t(std::shared_ptr<engine::platform_t> platform) : asset_database_base_t(platform)
		{

		}

		void set_path(const ustring_t & value)
		{
			std::lock_guard<std::recursive_mutex> guard1(mutex_desc);
			path = value;
		}

		const ustring_t & get_path() const
		{
			return path;
		}

	private:

		void rescan_local(asset_scanner_t::desc_collection_t & output) final
		{
			scan_path_overwriting(path, output);
		}

		ustring_t path;

	};

	class asset_database_launcher_t : public asset_database_base_t
	{

	public:

		asset_database_launcher_t(std::shared_ptr<engine::platform_t> platform) : asset_database_base_t(platform)
		{

		}

	private:

		void rescan_local(asset_scanner_t::desc_collection_t & output) final
		{
			scan_path_overwriting(_U("common"), output);
			scan_path_overwriting(_U("launcher"), output);
		}

	};

	class asset_database_editor_t : public asset_database_base_t
	{

	public:

		asset_database_editor_t(std::shared_ptr<engine::platform_t> platform) : asset_database_base_t(platform)
		{

		}

	private:

		void rescan_local(asset_scanner_t::desc_collection_t & output) final
		{
			scan_path_overwriting(_U("common"), output);
			scan_path_overwriting(_U("editor"), output);
		}

	};

	class module_resolver_t;

	class asset_database_game_t : public asset_database_base_t
	{

	public:

		asset_database_game_t(std::shared_ptr<engine::platform_t> platform, std::shared_ptr<engine::config_t> config, std::shared_ptr<engine::module_resolver_t> module_resolver) : asset_database_base_t(platform), config(config), module_resolver(module_resolver)
		{

		}

		std::shared_ptr<engine::config_t> get_config()
		{
			return config;
		}

		const module_problems_t & get_problems_state()
		{
			return problems_state;
		}

	private:

		void rescan_local(asset_scanner_t::desc_collection_t & output) final;
		
		std::shared_ptr<engine::config_t> config;
		std::shared_ptr<engine::module_resolver_t> module_resolver;

		module_problems_t problems_state;

	};

	class asset_monitor_base_t
	{

	public:

		virtual ~asset_monitor_base_t()
		{
			change_running(false);
		}

		void change_running(bool run = true)
		{
			if (run)
			{
				if (!is_scanning)
				{
					end_scanning = false;
					is_scanning = true;
					check_filesystem_thread = std::thread([this] { check_filesystem(); });
				}
			}
			else
			{
				if (is_scanning)
				{
					end_scanning = true;
					check_filesystem_thread.join();
					is_scanning = false;
				}
			}
		}

		void init_update()
		{
			std::lock_guard<std::recursive_mutex> guard(mutex);

			has_config_changed = false;

			if (config->was_updated())
			{
				has_config_changed = true;
				rescan();
			}

			init_update_local();
		}
		virtual void rescan() = 0;

		const asset_changes_t & get_changes()
		{
			return changes;
		}

		bool is_changed()
		{
			std::lock_guard<std::recursive_mutex> guard(mutex);
			return !changes.empty() || has_config_changed || is_changed_local();
		}

	protected:
		
		asset_monitor_base_t(std::shared_ptr<engine::logger_t> logger, std::shared_ptr<engine::config_t> config) : logger(logger), config(config), is_scanning(false), end_scanning(false)
		{

		}

		std::recursive_mutex mutex;

		asset_changes_t changes;

		bool is_currently_scanning()
		{
			return is_scanning;
		}

		std::shared_ptr<engine::logger_t> get_logger()
		{
			return logger;
		}

		std::shared_ptr<engine::config_t> get_config()
		{
			return config;
		}

	private:

		virtual bool is_changed_local() = 0;

		virtual void init_update_local() = 0;

		void check_filesystem()
		{
			logger->p_msg(_U("Will monitor filesystem for asset changes"));
			for (;;)
			{
				if (end_scanning) break;
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
				rescan();
			}
			logger->p_msg(_U("Finished monitoring filesystem for asset changes"));
		}

		std::thread check_filesystem_thread;
		bool is_scanning;
		bool end_scanning;
		bool has_config_changed;

		std::shared_ptr<engine::logger_t> logger;
		std::shared_ptr<engine::config_t> config;

	};

	template<class T> class asset_monitor_t : public asset_monitor_base_t
	{

		static_assert(std::is_base_of<asset_database_base_t, T>::value, "Monitor can only monit asset_database!");

	public:

		asset_monitor_t(std::shared_ptr<engine::logger_t> logger, std::shared_ptr<engine::config_t> config, std::unique_ptr<T> current, std::unique_ptr<T> to_scan) : asset_monitor_base_t(logger, config), is_updated(false), has_database_updated(false)
		{
			database_current = std::move(current);
			database_to_scan = std::move(to_scan);
			database_current->rescan();
			asset_monitor_t<T>::rescan();
			init_update_local();
		}
		

		T * get_database_current()
		{
			std::lock_guard<std::recursive_mutex> guard(mutex);
			return database_current.get();
		}

		bool is_changed_local() final
		{
			return has_database_updated;
		}

		void rescan() final
		{
			std::lock_guard<std::recursive_mutex> guard(mutex);
			database_to_scan->rescan();
			is_updated = true;
		}

	private:

		void init_update_local() final
		{
			has_database_updated = false;
			if (is_updated)
			{
				std::lock_guard<std::recursive_mutex> guard(mutex);
				changes = database_to_scan->get_changes_since(database_current.get());

				std::swap(database_to_scan, database_current);

				has_database_updated = database_current->has_updated_since();
				database_current->clear_updated_since();

				is_updated = false;
			}
			else if (!changes.empty())
				changes.clear();
		}

		std::unique_ptr<T> database_current;
		std::unique_ptr<T> database_to_scan;

		bool is_updated;
		bool has_database_updated;
	};

	class asset_cache_t
	{

	public:

	protected:

		template<class T> void register_asset_type()
		{

		}

	private:

		template<class T> void add_to_cache(std::shared_ptr<T> asset)
		{

		}

	};
}

#endif
#endif