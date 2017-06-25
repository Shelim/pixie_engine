#if 0
#ifndef ENGINE_COMMON_MODULES_HPP
#define ENGINE_COMMON_MODULES_HPP
#pragma once

#include <cereal/archives/xml.hpp>
#include <cereal/access.hpp>
#include <queue>
#include <set>
#include "common/utility/text/ustring.hpp"
#include "common/config.hpp"
#include "common/filesystem.hpp"

namespace engine
{
	typedef ustring_collection_t module_callstack_t;

	class module_problem_t
	{

	public:

		enum type_t : uint8_t
		{
			none,
#define GAME_MODULE_PROBLEM_STD(x) x,
#include "common/std/problems_std.hpp"
			count
		};

		ustring_t get_type_key() const
		{
			switch (type)
			{
#define GAME_MODULE_PROBLEM_STD(x) case x: return _U(#x);
#include "common/std/problems_std.hpp"
			}
			return _U("unknown");
		}

		const ustring_t & get_target_name() const
		{
			return target_name;
		}

		const module_callstack_t & get_mod_callstack() const
		{
			return mod_callstack;
		}

		ustring_t get_mod_callstack_ustring() const
		{
			ustring_t ret;
			for (auto & iter : mod_callstack)
			{
				if (!ret.is_empty()) ret.append(_U(" -> "));
				ret.append(iter);
			}
			return ret;
		}

		const virtual_path_t & get_target_path() const
		{
			return target_path;
		}

		const ustring_t & get_debug_txt() const
		{
			return debug_txt;
		}

		type_t get_type() const
		{
			return type;
		}

		module_problem_t(type_t type, const virtual_path_t target_path, const ustring_t & target_name, const ustring_t & debug_txt = _U("(None)"), const module_callstack_t & mod_callstack = module_callstack_t()) : target_path(target_path), target_name(target_name), mod_callstack(mod_callstack), debug_txt(debug_txt), type(type)
		{

		}

	private:

		virtual_path_t target_path;
		ustring_t target_name;
		module_callstack_t mod_callstack;
		ustring_t debug_txt;

		type_t type;

	};

	typedef std::vector<module_problem_t> module_problems_t;

	class manifest_provider_base_t
	{

	public:

		virtual ustring_t get_data() = 0;
		virtual filesystem::file_time_type get_time_last_mod() = 0;

	protected:

		virtual ~manifest_provider_base_t()
		{

		}

	private:

	};
	
	class manifest_provider_standard_t
	{

	};

	class manifest_base_t
	{

	public:

		virtual ~manifest_base_t()
		{

		}

		filesystem::file_time_type get_time_last_mod() const
		{
			return time_last_mod;
		}

		const ustring_t & get_name() const
		{
			return name;
		}

		const ustring_t & get_physical_path() const
		{
			return physical_path;
		}

		const module_problems_t & get_problems() const
		{
			return problems;
		}

		bool is_valid()
		{
			return problems.empty();
		}

		bool is_updated()
		{
			std::error_code ec;
			return time_last_mod != filesystem::last_write_time(physical_path.get_cstring(), ec);
		}

		static ustring_t create_path(const ustring_t & name, const ustring_t & path)
		{
			ustring_t	physical_path = path;
			physical_path.append(_U("/"));
			physical_path.append(name);
			physical_path.append(_U("/_manifest.info"));

			return physical_path;
		}

	protected:

		template<class... args> void emplace_problem(args... a)
		{
			problems.emplace_back(std::forward<args>(a)...);
		}

		manifest_base_t(const ustring_t & name, const ustring_t & path) : name(name)
		{
			physical_path = create_path(name, path);

			time_last_mod = get_time_last_mod();
		}

	private:
		
		filesystem::file_time_type get_time_last_mod()
		{
			std::error_code ec;
			return filesystem::last_write_time(physical_path.get_cstring(), ec);
		}

		filesystem::file_time_type time_last_mod;
		ustring_t physical_path;
		ustring_t name;

		module_problems_t problems;
	};

	class module_manifest_t : public manifest_base_t
	{

	public:

		module_manifest_t(const ustring_t & name);
		module_manifest_t(module_manifest_t &) = delete;
		module_manifest_t & operator= (module_manifest_t&) = delete;
		module_manifest_t(module_manifest_t &&) = default;
		module_manifest_t & operator= (module_manifest_t&&) = default;

		enum type_t : uint8_t
		{
			allow_any_listed_submodule,
			allow_any_not_listed_submodule
		};

		enum flag_t : uint8_t
		{
			none = 0,
			allow_from_included_modules = 1 << 0
		};

		const type_t get_type() const
		{
			return sub_modules.type;
		}

		void set_type(type_t value)
		{
			sub_modules.type = value;
		}

		const flag_t get_flag() const
		{
			return sub_modules.flags;
		}

		const bool is_flag(flag_t flag) const
		{
			return sub_modules.flags & flag;
		}

		void set_flag(flag_t flag, bool value)
		{
			if (value)
				sub_modules.flags = static_cast<flag_t>(sub_modules.flags | flag);
			else
				sub_modules.flags = static_cast<flag_t>(sub_modules.flags & ~flag);
		}
		
		struct submodules_t
		{
			ustring_collection_t submodules;
			type_t type;
			flag_t flags;
		};
		
		struct includes_t
		{
			ustring_collection_t includes;
		};

		const includes_t & get_includes() const
		{
			return includes;
		}

		ustring_collection_t * get_includes_for_edit()
		{
			return &includes.includes;
		}

		const submodules_t & get_sub_modules() const
		{
			return sub_modules;
		}

		ustring_collection_t * get_sub_modules_for_edit()
		{
			return &sub_modules.submodules;
		}

		void resave();

		static ustring_t get_path()
		{
			return _U("modules");
		}

	private:

		friend class cereal::access;

		includes_t includes;
		submodules_t sub_modules;

		template<class archive_t> void serialize(archive_t & ar)
		{
			ar(cereal::make_nvp("includes", includes));
			ar(cereal::make_nvp("sub_modules", sub_modules));
		}
	};

	class sub_module_manifest_t : public manifest_base_t
	{

	public:

		sub_module_manifest_t(const ustring_t & name);
		sub_module_manifest_t(sub_module_manifest_t &) = delete;
		sub_module_manifest_t & operator= (sub_module_manifest_t&) = delete;
		sub_module_manifest_t(sub_module_manifest_t &&) = default;
		sub_module_manifest_t & operator= (sub_module_manifest_t&&) = default;
		
		enum class flag_t
		{
			forbid_any_module_but_required,
			forbid_any_sub_module_but_required,

			count
		};

		struct modules_t
		{
			ustring_collection_t modules;
		};

		struct sub_modules_t
		{
			ustring_collection_t sub_modules;
		};

		struct required_t
		{
			modules_t modules;
			sub_modules_t sub_modules;
		};

		struct forbidden_t
		{
			modules_t modules;
			sub_modules_t sub_modules;
			std::bitset<static_cast<std::size_t>(flag_t::count)> flags;

			bool is_flag(flag_t flag) const
			{
				return flags.test(static_cast<std::size_t>(flag));
			}

			void set_flag(flag_t flag, bool value)
			{
				if (value)
					flags = static_cast<flag_t>(flags | flag);
				else
					flags = static_cast<flag_t>(flags & ~flag);
			}
		};

		const required_t & get_required() const
		{
			return required;
		}

		const forbidden_t & get_forbidden() const
		{
			return forbidden;
		}

		required_t * get_required_for_edit()
		{
			return &required;
		}

		forbidden_t * get_forbidden_for_edit()
		{
			return &forbidden;
		}

		static ustring_t get_path()
		{
			return _U("sub_modules");
		}

		void resave();

	private:

		friend class cereal::access;

		required_t required;
		forbidden_t forbidden;

		template<class archive_t> void serialize(archive_t & ar)
		{
			ar(cereal::make_nvp("requires", required));
			ar(cereal::make_nvp("forbids", forbidden));
		}
	};

	class manifest_provider_t
	{

	public:

		virtual ~manifest_provider_t()
		{

		}

		virtual ustring_collection_t get_modules() = 0;
		virtual ustring_collection_t get_sub_modules() = 0;

	private:


	};

	class manifest_database_t
	{

	public:

		manifest_database_t();

		module_manifest_t * get_module(const ustring_t & name)
		{
			auto iter = module_manifests.find(name);
			if (iter == module_manifests.end()) return nullptr;
			return iter->second.get();
		}

		sub_module_manifest_t * get_sub_module(const ustring_t & name)
		{
			auto iter = sub_module_manifests.find(name);
			if (iter == sub_module_manifests.end()) return nullptr;
			return iter->second.get();
		}

		typedef std::map<ustring_t, std::unique_ptr<module_manifest_t>  > module_manifests_t;
		typedef std::map<ustring_t, std::unique_ptr<sub_module_manifest_t> > sub_module_manifests_t;

		const module_manifests_t * get_modules()
		{
			return &module_manifests;
		}

		const sub_module_manifests_t * get_sub_modules()
		{
			return &sub_module_manifests;
		}

		const module_problems_t & get_problems() const
		{
			return problems;
		}

		bool is_reload_requested()
		{
			return reload_requested;
		}

		void reload();

		void check_for_reload();

	private:

		void scan_for_available(const ustring_t & directory, ustring_collection_t & output);

		void append_problems(const module_problems_t & new_problems)
		{
			problems.insert(problems.end(), new_problems.begin(), new_problems.end());
		}

		std::recursive_mutex mutex_load;
		module_manifests_t module_manifests;
		sub_module_manifests_t sub_module_manifests;

		module_problems_t problems;
		bool reload_requested;
	};

	class module_information_t;

	class submodule_information_t
	{

	public:

		bool is_valid()
		{
			return problems.empty();
		}

		const module_problems_t & get_problems() const
		{
			return problems;
		}

		module_information_t * get_owner()
		{
			return owner;
		}

	private:

		friend class module_resolver_t;
		friend class module_state_t;

		module_information_t * owner;

		std::set<ustring_t> submodules_required;
		std::map<ustring_t, module_callstack_t> submodules_stack;
		std::set<ustring_t> modules_required;

		std::set<ustring_t> submodules_forbidden;
		std::set<ustring_t> modules_forbidden;

		module_problems_t problems;
	};

	class module_information_t
	{

	public:

		bool is_valid() const
		{
			return problems.empty();
		}

		const module_problems_t & get_problems() const
		{
			return problems;
		}
		
		const std::vector<module_callstack_t> & get_modules_stack() const
		{
			return modules_stack;
		}

	private:

		friend class module_resolver_t;
		friend class module_state_t;

		std::vector<module_callstack_t> modules_stack;
		std::map<ustring_t, submodule_information_t> submodules_allowed;
		std::map<ustring_t, std::vector<module_callstack_t> > submodules_forbidden_source;
		module_problems_t problems;
	};

	class module_resolver_t;

	class module_state_t
	{

	public:

		module_state_t(std::shared_ptr<module_resolver_t> resolver, const ustring_t & module, const ustring_collection_t & sub_modules = ustring_collection_t());


		const ustring_t & get_module()
		{
			return modules[0];
		}

		bool is_valid_module()
		{
			return !modules.empty();
		}

		const ustring_collection_t & get_modules()
		{
			return modules;
		}

		const ustring_collection_t & get_sub_modules()
		{
			return sub_modules;
		}

		const module_problems_t & get_problems()
		{
			return problems;
		}

		struct enable_result_t
		{
			module_problems_t problems;
			std::set<ustring_t> other_sub_modules_to_enable;

			bool is_valid()
			{
				return problems.empty();
			}
		};

		struct disable_result_t
		{
			std::set<ustring_t> other_sub_modules_to_disable;
		};

		enable_result_t enable_test(const ustring_t & submodule);
		enable_result_t enable(const ustring_t & submodule);
		disable_result_t disable_test(const ustring_t & submodule);
		disable_result_t disable(const ustring_t & submodule);

		void save_config(std::shared_ptr<config_t> config)
		{
			config->set(config_t::item_t::engine_module, modules[0]);
			config->set(config_t::item_t::engine_submodules, sub_modules);
		}

	private:

		void enable_internal(const ustring_t & submodule);

		std::shared_ptr<module_resolver_t> resolver;
		ustring_collection_t modules;
		ustring_collection_t sub_modules;
		module_problems_t problems;
	};

	class module_resolver_t : public std::enable_shared_from_this<module_resolver_t>
	{

	public:

		void reload();

		module_resolver_t(std::shared_ptr<logger_t> logger) : logger(logger)
		{
			reload();
		}

		module_state_t get_state(const ustring_t & module, const ustring_collection_t & sub_modules)
		{
			return module_state_t(shared_from_this(), module, sub_modules);
		}
		module_state_t get_state(std::shared_ptr<config_t> config)
		{
			return get_state(config->get<ustring_t>(config_t::item_t::engine_module), config->get<ustring_collection_t>(config_t::item_t::engine_submodules));
		}

		void check_for_reload()
		{
			database.check_for_reload();
		}

		bool is_reload_requested()
		{
			return database.is_reload_requested();
		}

		const module_problems_t & get_problems() const
		{
			return problems;
		}

	private:

		friend class module_state_t;

		module_information_t query_module_information(const ustring_t & module);
		submodule_information_t query_submodule_information(const ustring_t & submodule);

		std::map<ustring_t, module_information_t> modules_information;
		std::map<ustring_t, submodule_information_t> sub_modules_information;

		module_problems_t problems;

		struct module_to_check_t
		{
			module_to_check_t(const ustring_t & name, bool allow_sub_modules_from_this_module, const module_callstack_t & prev_callstack = module_callstack_t()) : name(name), allow_sub_modules_from_this_module(allow_sub_modules_from_this_module)
			{
				callstack = prev_callstack;
				callstack.push_back(name);
			}
			ustring_t name;
			bool allow_sub_modules_from_this_module;
			module_callstack_t callstack;
		};

		struct sub_module_to_check_t
		{
			sub_module_to_check_t(const ustring_t & name, const module_callstack_t & prev_callstack = module_callstack_t()) : name(name)
			{
				callstack = prev_callstack;
				callstack.push_back(name);
			}
			ustring_t name;
			module_callstack_t callstack;
		};

		void append_problems(const module_problems_t & new_problems)
		{
			problems.insert(problems.end(), new_problems.begin(), new_problems.end());
		}

		manifest_database_t database;
		std::shared_ptr<logger_t> logger;
	};
	
	/////////////////////////////////////////////////////////////////////////////////////////////////

	template<class archive_t> void save(archive_t & ar, module_manifest_t::includes_t const & m)
	{
		for (auto & iter : m.includes)
		{
			ar(cereal::make_nvp("include", iter));
		}
	}

	template<class archive_t> void load(archive_t & ar, module_manifest_t::includes_t & m)
	{
		for (;;)
		{
			const auto name = ar.getNodeName();
			if (!name)
				break;

			ustring_t value;
			ar(value);

			m.includes.push_back(value);
		}
	}

	template<class archive_t> void save(archive_t & ar, module_manifest_t::submodules_t const & m)
	{
		std::string flag_from_included_modules = "deny";
		if (m.flags & module_manifest_t::allow_from_included_modules)
			flag_from_included_modules = "allow";

		ar(cereal::make_nvp("from_included_modules", flag_from_included_modules));

		std::string type = "";

		switch (m.type)
		{
		case module_manifest_t::allow_any_listed_submodule: type = "allow_any_listed_below"; break;
		case module_manifest_t::allow_any_not_listed_submodule: type = "allow_any_not_listed_below"; break;
		}

		ar(cereal::make_nvp("type", type));

		for (auto & iter : m.submodules)
		{
			ar(cereal::make_nvp("sub_module", iter));
		}
	}

	template<class archive_t> void load(archive_t & ar, module_manifest_t::submodules_t & m)
	{
		m.flags = module_manifest_t::flag_t::none;
		m.type = module_manifest_t::allow_any_not_listed_submodule;

		for (;;)
		{
			const auto name = ar.getNodeName();
			if (!name)
				break;

			std::string name_str = name;

			ustring_t value;
			ar(value);
			value = value.trim().to_lower();

			if (name_str == "from_included_modules")
				m.flags = static_cast<module_manifest_t::flag_t>(m.flags | ((value == _U("allow")) ? module_manifest_t::allow_from_included_modules : 0));
			else if (name_str == "type")
			{
				if (value == _U("allow_any_listed_below"))
					m.type = module_manifest_t::allow_any_listed_submodule;
				else
					m.type = module_manifest_t::allow_any_not_listed_submodule;
			}
			else
				m.submodules.push_back(value);
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////

	template<class archive_t> void save(archive_t & ar, sub_module_manifest_t::modules_t const & m)
	{
		for (auto & iter : m.modules)
		{
			ar(cereal::make_nvp("module", iter));
		}
	}

	template<class archive_t> void load(archive_t & ar, sub_module_manifest_t::modules_t & m)
	{
		for (;;)
		{
			const auto name = ar.getNodeName();
			if (!name)
				break;

			ustring_t value;
			ar(value);

			m.modules.push_back(value);
		}
	}

	template<class archive_t> void save(archive_t & ar, sub_module_manifest_t::sub_modules_t const & m)
	{
		for (auto & iter : m.sub_modules)
		{
			ar(cereal::make_nvp("sub_module", iter));
		}
	}

	template<class archive_t> void load(archive_t & ar, sub_module_manifest_t::sub_modules_t & m)
	{
		for (;;)
		{
			const auto name = ar.getNodeName();
			if (!name)
				break;

			ustring_t value;
			ar(value);

			m.sub_modules.push_back(value);
		}
	}

	template<class archive_t> void save(archive_t & ar, sub_module_manifest_t::required_t const & m)
	{
		ar(cereal::make_nvp("modules", m.modules));
		ar(cereal::make_nvp("sub_modules", m.sub_modules));
	}

	template<class archive_t> void load(archive_t & ar, sub_module_manifest_t::required_t & m)
	{
		ar(cereal::make_nvp("modules", m.modules));
		ar(cereal::make_nvp("sub_modules", m.sub_modules));
	}

	template<class archive_t> void save(archive_t & ar, sub_module_manifest_t::forbidden_t const & m)
	{
		std::string any_module_but_required = "false";
		if (m.flags & sub_module_manifest_t::forbid_any_module_but_required)
			any_module_but_required = "true";

		std::string any_sub_module_but_required = "false";
		if (m.flags & sub_module_manifest_t::forbid_any_sub_module_but_required)
			any_sub_module_but_required = "true";

		ar(cereal::make_nvp("any_module_but_required", any_module_but_required));
		ar(cereal::make_nvp("any_sub_module_but_required", any_sub_module_but_required));
		ar(cereal::make_nvp("modules", m.modules));
		ar(cereal::make_nvp("sub_modules", m.sub_modules));
	}

	template<class archive_t> void load(archive_t & ar, sub_module_manifest_t::forbidden_t & m)
	{
		ustring_t any_module_but_required;
		ar(cereal::make_nvp("any_module_but_required", any_module_but_required));
		bool is_any_module_but_required = from_string<bool>(any_module_but_required.trim().to_lower());

		m.flags = sub_module_manifest_t::flag_t::none;

		if (is_any_module_but_required)
			m.flags = static_cast<sub_module_manifest_t::flag_t>(m.flags | sub_module_manifest_t::forbid_any_module_but_required);

		ustring_t any_sub_module_but_required;
		ar(cereal::make_nvp("any_sub_module_but_required", any_sub_module_but_required));
		bool is_any_sub_module_but_required = from_string<bool>(any_sub_module_but_required.trim().to_lower());

		if (is_any_sub_module_but_required)
			m.flags = static_cast<sub_module_manifest_t::flag_t>(m.flags | sub_module_manifest_t::forbid_any_sub_module_but_required);

		ar(cereal::make_nvp("modules", m.modules));
		ar(cereal::make_nvp("sub_modules", m.sub_modules));
	}
}

#endif
#endif