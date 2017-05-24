#ifndef ENGINE_COMMON_MODULE_PROBLEM_HPP
#define ENGINE_COMMON_MODULE_PROBLEM_HPP
#pragma once

#include "common/ustring.hpp"
#include "common/filesystem.hpp"
#include "common/virtual_path.hpp"
#include "common/asset/path.hpp"
#include "common/data/input.hpp"
#include "common/data/output.hpp"
#include "common/data/database.hpp"
#include "common/logger.hpp"
#include <cereal/access.hpp>
#include <cereal/types/polymorphic.hpp>

#include <cereal/archives/binary.hpp>
#include <cereal/archives/xml.hpp>
#include <cereal/archives/json.hpp>
#include "common/xml_pixie_archive.hpp"
#include "common/ustring.hpp"

namespace engine
{
	namespace module
	{
		typedef ustring_t name_t;

		class name_full_t final
		{

		public:

			enum class type_t : uint8_t
			{
				unknown,
				module,
				submodule
			};

			explicit name_full_t(const name_t & name, type_t type = type_t::unknown) : name(name), type(type)
			{

			}

			const name_t & get_name() const
			{
				return name;
			}

			type_t get_type() const
			{
				return type;
			}

		private:

			name_t name;
			type_t type;

		};
	}

	template<> inline ustring_t to_string<module::name_full_t>(const module::name_full_t & item)
	{
		ustring_t ret = item.get_name();

		switch (item.get_type())
		{
		case module::name_full_t::type_t::unknown: ret.append(_U(" (???)")); break;
		case module::name_full_t::type_t::module: ret.append(_U(" (module)")); break;
		case module::name_full_t::type_t::submodule: ret.append(_U(" (submodule)")); break;
		}

		return ret;
	}
	
	namespace module
	{

		class callstack_t final
		{

		public:

			void add(const name_full_t & item)
			{
				items.push_back(item);
			}

			typedef std::vector<name_full_t> items_t;

			const items_t & get_items() const
			{
				return items;
			}

		private:

			items_t items;

		};
	}

	template<> inline ustring_t to_string<module::callstack_t>(const module::callstack_t & items)
	{
		ustring_t ret;
		for (auto & item : items.get_items())
		{
			if (!ret.is_empty()) ret.append(_U(" -> "));

			ret.append(to_string(item));
		}
		return ret;
	}

	namespace module
	{

		class problem_t
		{

		public:

			enum class type_t : uint8_t
			{
				none,
#define GAME_MODULE_PROBLEM_STD(x) x,
#include "common/std/problems_std.hpp"
				count
			};

			static ustring_t get_type_key(type_t type)
			{
				switch (type)
				{
#define GAME_MODULE_PROBLEM_STD(x) case type_t::x: return _U(#x);
#include "common/std/problems_std.hpp"
				}
				return _U("unknown");
			}

			type_t get_type() const
			{
				return type;
			}

			const name_full_t & get_target() const
			{
				return target;
			}
			
			const callstack_t & get_callstack() const
			{
				return callstack;
			}

			const ustring_t & get_debug_txt() const
			{
				return debug_txt;
			}

			problem_t(type_t type, const name_full_t & target, const callstack_t & callstack = callstack_t(), const ustring_t & debug_txt = _U("")) : type(type), target(target), callstack(callstack), debug_txt(debug_txt)
			{

			}

			ustring_t to_debug_str() const
			{
				ustring_t callstack_str = _U("");
				if (!callstack.get_items().empty())
				{
					callstack_str.append(_U(" included from: ["));
					callstack_str.append(to_string(callstack));
					callstack_str.append(_U("]"));
				}
				ustring_t debug_txt_str = _U("");
				if (!debug_txt.is_empty())
				{
					debug_txt_str = _U(" ERROR: ");
					debug_txt_str.append(debug_txt);
				}
				return format_string(_U("#1# #2##3##4#"), get_type_key(type), target, callstack_str, debug_txt_str);
			}

			void log(logger_t * logger) const
			{
				logger->p_err(to_debug_str());
			}

		private:

			type_t type;
			name_full_t target;
			callstack_t callstack;
			ustring_t debug_txt;


		};

		class problems_t final
		{

		public:

			typedef std::vector<problem_t> items_t;

			problems_t()
			{

			}

			const items_t &  get_items() const
			{
				return items;
			}

			bool is_valid() const
			{
				return items.empty();
			}

			void add_problem(const problem_t & problem)
			{
				items.push_back(problem);
			}

			void merge_problems(const problems_t & problems)
			{
				items.insert(items.end(), problems.items.begin(), problems.items.end());
			}

			void clear()
			{
				items.clear();
			}

		private:

			items_t items;
		};

	}
}

#endif