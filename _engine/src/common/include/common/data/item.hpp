#ifndef ENGINE_COMMON_DATA_ITEM_HPP
#define ENGINE_COMMON_DATA_ITEM_HPP
#pragma once

#include "common/ustring.hpp"
#include "common/filesystem.hpp"
#include "common/virtual_path.hpp"
#include "common/data/input.hpp"
#include "common/data/output.hpp"
#include "common/data/provider.hpp"
#include "common/filesystem.hpp"
#include "common/data/item_content.hpp"
#include <bitset>
#include <cereal/access.hpp>

namespace engine
{
	namespace data
	{
		class database_t;

		class item_base_t
		{
			
		public:

			virtual ~item_base_t()
			{

			}

			enum class flag_t
			{
				is_dirty,

				count
			};

			void set_dirty()
			{
				set_flag(flag_t::is_dirty, true);
			}
			
			bool is_dirty()
			{
				return is_flag(flag_t::is_dirty);
			}

			const virtual_path_t & get_path() const
			{
				return path;
			}

		protected:

			item_base_t(const virtual_path_t & path) : path(path)
			{

			}

		private:

			std::bitset<static_cast<std::size_t>(flag_t::count)> flags;

			void set_flag(flag_t flag, bool value)
			{
				flags.set(static_cast<std::size_t>(flag), value);
			}

			bool is_flag(flag_t flag) const
			{
				return flags.test(static_cast<std::size_t>(flag));
			}

			virtual_path_t path;

		};

		class item_t final : public item_base_t
		{

		public:

			item_content_base_t * get_base()
			{
				return content.get();
			}

			template<class T> T * get()
			{
				return static_cast<T*>(get_base());
			}

			friend class database_items_t;

		private:

			item_t() : item_base_t(virtual_path_t()) // for cereal
			{

			}

			typedef std::function<std::unique_ptr<item_content_base_t>(const virtual_path_t &, item_t *)> create_content_t;

			item_t(const virtual_path_t & path, create_content_t create_content_func) :
				item_base_t(path), create_content_func(create_content_func)
			{
				content = std::move(create_content_func(path, this));
			}

			void destroy()
			{
				content->destroy();
			}

			template<class archive_t> void save(archive_t & ar) const
			{
				ar(*get_content_save());
			}

			template<class archive_t> void load(archive_t & ar)
			{
				ar(*get_content_load());
			}

			template<class T> static std::unique_ptr<item_content_base_t> create_content(const virtual_path_t & path, item_t * owner)
			{
				return std::move(std::make_unique<T>(owner));
			}

			template<class T> static std::shared_ptr<item_t> create_item(const virtual_path_t & path)
			{
				std::shared_ptr<item_t> ret = std::shared_ptr<item_t>(new item_t(path, create_content<T>));
				ret->destroy();

				return ret;
			}

			create_content_t create_content_func;

			item_content_base_t::result_t reload_sync(std::unique_ptr<input_t> input, database_items_t * database_items, bool force = false);
			item_content_base_t::result_t reload_async_init(std::unique_ptr<input_t> input, database_items_t * database_items, bool force = false);
			item_content_base_t::result_t reload_async(database_items_t * database_items);
			item_content_base_t::result_t reload_async_end(database_items_t * database_items);
			bool is_reloading();

			item_content_base_t * get_content_save()
			{
				auto policy = content->get_resave_policy();

				if (policy == item_content_base_t::policy_io_t::implicit_async_copy || policy == item_content_base_t::policy_io_t::explicit_async_copy)
				{
					return content_save.get();
				}

				return content.get();
			}

			item_content_base_t * get_content_load()
			{
				auto policy = content->get_reload_policy();

				if (policy == item_content_base_t::policy_io_t::implicit_async_copy || policy == item_content_base_t::policy_io_t::explicit_async_copy)
				{
					return content_load.get();
				}

				return content.get();
			}

			std::unique_ptr<item_content_base_t> content;
			std::unique_ptr<item_content_base_t> content_save;
			std::unique_ptr<item_content_base_t> content_load;
		};

		class items_collection_t final : public item_base_t
		{

		public:

			friend class database_items_t;

		private:

		};
	}

}

#endif