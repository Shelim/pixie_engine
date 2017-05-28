#ifndef ENGINE_COMMON_DATA_ITEM_CONTENT_HPP
#define ENGINE_COMMON_DATA_ITEM_CONTENT_HPP
#pragma once

#include "common/ustring.hpp"
#include "common/filesystem.hpp"
#include "common/virtual_path.hpp"
#include "common/data/input.hpp"
#include "common/data/output.hpp"
#include "common/data/item_operation.hpp"
#include <bitset>
#include <map>
#include <vector>
#include <string>
#include <unordered_set>
#include <thread>
#include <memory>
#include <queue>
#include <mutex>

namespace engine
{
	namespace data
	{
		class database_t;
		class database_items_t;

		class item_content_base_t
		{

		public:

			virtual ~item_content_base_t()
			{

			}
			
			virtual bool resave(output_t * output) = 0;

			friend class item_generic_t;

			bool is_destroyed();

			struct destroyed_t
			{

			};

		protected:

			virtual item_content_base_t * clone() const = 0;

			item_content_base_t() : owner(nullptr)
			{

			}

			item_content_base_t(item_generic_t * owner) : owner(owner)
			{

			}

			item_content_base_t(const item_content_base_t & other) : owner(other.owner)
			{

			}

			item_generic_t * get_owner()
			{
				return owner;
			}

			void set_dirty();

		private:
			
			item_generic_t * owner;

			virtual bool execute_operation(const item_operation_t::step_t & step, item_operation_t * operation)
			{
				if (operation->get_type() == item_operation_t::type_t::save)
					return execute_output_operation(step, operation);
				else if (operation->get_type() == item_operation_t::type_t::load)
					return execute_input_operation(step, operation);
				else if (operation->get_type() == item_operation_t::type_t::free)
				{
					execute_free_operation(step, operation);
					return true;
				}
				return false;
			}

			virtual bool is_sub_operation_pending()
			{
				return false;
			}

			virtual bool execute_input_operation(const item_operation_t::step_t & step, item_operation_t * operation)
			{
				return true;
			}
			virtual bool execute_output_operation(const item_operation_t::step_t & step, item_operation_t * operation)
			{
				return true;
			}
			virtual void execute_free_operation(const item_operation_t::step_t & step, item_operation_t * operation)
			{

			}

		};

	}

}

#endif