#ifndef ENGINE_COMMON_MODULE_INFO_HPP
#define ENGINE_COMMON_MODULE_INFO_HPP
#pragma once

#include "common/module/problem.hpp"

namespace engine
{

	namespace module
	{
		class database_manifest_t;
		class module_information_t;

		class submodule_information_t
		{

		public:

			typedef std::set<name_t> items_t;

			bool is_valid()
			{
				return problems.is_valid();
			}

			const problems_t & get_problems() const
			{
				return problems;
			}

			const items_t & get_submodules_required() const
			{
				return submodules_required;
			}

			const items_t & get_modules_required() const
			{
				return modules_required;
			}

			const items_t & get_submodules_forbidden() const
			{
				return submodules_forbidden;
			}

			const items_t & get_modules_forbidden() const
			{
				return modules_forbidden;
			}

			void query_submodule_source(const name_t & name, callstack_t * output)
			{
				auto & iter = submodules_stack.find(name);
				if (iter != submodules_stack.end() && output)
					*output = iter->second;
			}

			submodule_information_t(const name_t & name, database_manifest_t * database_manifest);

		private:

			class sub_module_to_check_t
			{

			public:

				sub_module_to_check_t(const name_full_t & name, const callstack_t & prev_callstack = callstack_t()) : name(name)
				{
					callstack = prev_callstack;
					callstack.add(name);
				}

				const name_full_t & get_name() const
				{
					return name;
				}

				const callstack_t & get_callstack() const
				{
					return callstack;
				}

			private:

				name_full_t name;
				callstack_t callstack;
			};
			
			items_t submodules_required;
			std::map<name_t, callstack_t > submodules_stack;
			items_t modules_required;

			items_t submodules_forbidden;
			items_t modules_forbidden;

			problems_t problems;
		};

		class module_information_t
		{

		public:

			typedef std::set<name_t> items_t;

			bool is_valid() const
			{
				return problems.is_valid();
			}

			const problems_t & get_problems() const
			{
				return problems;
			}

			const callstack_t & get_modules_stack() const
			{
				return modules_stack;
			}


			const items_t & get_submodules_allowed() const
			{
				return submodules_allowed;
			}
			const items_t & get_submodules_forbidden() const
			{
				return submodules_forbidden;
			}

			void query_submodule_forbidden_source(const name_t & name, callstack_t * output)
			{
				auto & iter = submodules_forbidden_source.find(name);
				if (iter != submodules_forbidden_source.end() && output)
					*output = iter->second;
			}

			module_information_t(const name_t & name, database_manifest_t * database_manifest);

		private:

			class module_to_check_t
			{

			public:

				module_to_check_t(const name_full_t & name, bool allow_sub_modules_from_this_module, const callstack_t & prev_callstack = callstack_t()) : name(name), allow_sub_modules_from_this_module(allow_sub_modules_from_this_module)
				{
					callstack = prev_callstack;
					callstack.add(name);
				}

				const name_full_t & get_name() const
				{
					return name;
				}

				bool is_allowed_sub_modules_from_this_module() const
				{
					return allow_sub_modules_from_this_module;
				}

				const callstack_t & get_callstack() const
				{
					return callstack;
				}

			private:

				name_full_t name;
				bool allow_sub_modules_from_this_module;
				callstack_t callstack;
			};
			
			callstack_t modules_stack;
			items_t submodules_allowed;
			items_t submodules_forbidden;
			std::map<name_t, callstack_t > submodules_forbidden_source;
			problems_t problems;
		};
	}
}

#endif