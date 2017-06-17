#ifndef ENGINE_COMMON_DATA_ITEM_CONTENT_HPP
#define ENGINE_COMMON_DATA_ITEM_CONTENT_HPP
#pragma once

#include "common/ustring.hpp"
#include "common/filesystem.hpp"
#include "common/virtual_path.hpp"
#include "common/data/input.hpp"
#include "common/data/output.hpp"
#include "common/data/item_task.hpp"
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
			
		};

	}

}

#endif