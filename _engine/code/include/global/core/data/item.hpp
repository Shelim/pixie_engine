#ifndef ENGINE_COMMON_UTILITY_DATA_ITEM_HPP
#define ENGINE_COMMON_UTILITY_DATA_ITEM_HPP
#pragma once

#include "utility/text/ustring.hpp"
#include "global/core/vfs/filesystem.hpp"
#include "global/core/vfs/virtual_path.hpp"
#include "global/core/data/output.hpp"
#include "utility/pattern/flags.hpp"

#include <algorithm>

namespace engine
{
	namespace data
	{
		class item_base_t;
		
		class item_generic_t
		{

		public:

			virtual ~item_generic_t()
			{

			}

			virtual item_base_t * get_base() = 0;
			template<class item_actual_t> item_actual_t * get_actual()
			{
				return static_cast<item_actual_t*>(get_base());
			}

			enum class flag_t
			{
				is_copyable,
				is_moveable,
				is_destroyed,
				count
			};

			std::shared_ptr<item_generic_t> make_symlink(const virtual_path_t & link)
			{

			}
			std::shared_ptr<item_generic_t> copy(const virtual_path_t & path)
			{
				if (flags.is_flag(flag_t::is_copyable))
				{
					copy_local(path);
				}
			}
			void move(const virtual_path_t & path)
			{
				if (flags.is_flag(flag_t::is_moveable))
				{
					move_local(path);
				}
			}
			void destroy()
			{
				flags.set_flag(flag_t::is_destroyed, true);
				destroy_local();
			}

		protected:

			item_generic_t()
			{

			}

		private:

			flags_t<flag_t> flags;
			
			std::shared_ptr<item_generic_t> linked_item;
			
			virtual std::shared_ptr<item_generic_t> copy_local(const virtual_path_t & path) = 0;
			virtual void move_local(const virtual_path_t & path) = 0;
			virtual void destroy_local() = 0;

		};


		template<class item_actual_t> class item_t : public item_generic_t
		{

		public:

			item_base_t * get_base() final
			{
				return item.get();
			}

			item_actual_t * get()
			{
				return item.get();
			}

		private:

			std::unique_ptr<item_actual_t> item;
			std::unique_ptr<item_actual_t> item_loading;

		};

	}

}

#include "global/core/data/input/file.hpp"
#include "global/core/data/input/ustring.hpp"
#include "global/core/data/input/buffer.hpp"
#include "global/core/data/input/void.hpp"

#endif