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
#include "common/data/item_task.hpp"
#include <bitset>
#include <cereal/access.hpp>

namespace engine
{
	namespace data
	{
		class database_providers_t;
		class item_task_t;

		class item_base_t
		{
			
		public:

			virtual ~item_base_t();

			enum class flag_t
			{
				is_requested_save,
				is_requested_load,
				is_requested_free,
				is_deatached,
				do_not_auto_reload,
				auto_resave,
				do_not_log_operations,
				allow_changes_when_deleted,

				is_operation_pending,

				count
			};

			bool are_allowed_changes_when_deleted()
			{
				return is_flag(flag_t::allow_changes_when_deleted);
			}

			void allow_changes_when_deleted()
			{
				set_flag(flag_t::allow_changes_when_deleted, true);
			}

			bool do_not_auto_reload()
			{
				return is_flag(flag_t::do_not_auto_reload);
			}

			void mark_as_do_not_auto_reload()
			{
				set_flag(flag_t::do_not_auto_reload, true);
			}

			void mark_as_do_not_auto_manage()
			{
				set_flag(flag_t::do_not_auto_reload, true);
				set_flag(flag_t::auto_resave, false);
			}

			bool auto_resave()
			{
				return is_flag(flag_t::auto_resave);
			}

			void mark_as_auto_resave()
			{
				set_flag(flag_t::auto_resave, true);
				set_flag(flag_t::do_not_auto_reload, true);
			}

			void mark_as_both_auto_resave_and_auto_reload()
			{
				set_flag(flag_t::auto_resave, true);
				set_flag(flag_t::do_not_auto_reload, false);
			}

			bool do_not_log_operations()
			{
				return is_flag(flag_t::do_not_log_operations);
			}

			void mark_as_do_not_log_operations()
			{
				set_flag(flag_t::do_not_log_operations, true);
			}

			void request_save()
			{
				set_flag(flag_t::is_requested_save, true);
			}
			
			bool is_requested_save()
			{
				return is_flag(flag_t::is_requested_save);
			}

			void request_load()
			{
				set_flag(flag_t::is_requested_load, true);
			}

			bool is_requested_load()
			{
				return is_flag(flag_t::is_requested_load);
			}

			void request_free()
			{
				set_flag(flag_t::is_requested_free, true);
			}

			bool is_requested_free()
			{
				return is_flag(flag_t::is_requested_load);
			}

			bool is_deatached()
			{
				return is_flag(flag_t::is_deatached);
			}

			const virtual_path_t & get_path() const
			{
				return path;
			}

			virtual void resave(output_t * output) = 0;

			database_items_t * get_database_items()
			{
				return database_items;
			}

			bool is_operation_pending()
			{
				return is_flag(flag_t::is_operation_pending);
			}

		protected:

			item_base_t(database_items_t * database_items, const virtual_path_t & path) : path(path), database_items(database_items)
			{

			}

			void clear_requested_save()
			{
				set_flag(flag_t::is_requested_save, false);
			}

			void clear_requested_load()
			{
				set_flag(flag_t::is_requested_load, false);
			}

			void clear_requested_free()
			{
				set_flag(flag_t::is_requested_free, false);
			}

			void set_deatached_flag()
			{
				set_flag(flag_t::is_deatached, true);
			}

			void set_path(const virtual_path_t & path)
			{
				this->path = path;
			}

		private:
			friend class database_items_t;
			friend class item_task_t;

			std::bitset<static_cast<std::size_t>(flag_t::count)> flags;

			void set_flag(flag_t flag, bool value)
			{
				flags.set(static_cast<std::size_t>(flag), value);
			}

			bool is_flag(flag_t flag) const
			{
				return flags.test(static_cast<std::size_t>(flag));
			}

			void start_operation()
			{
				set_flag(flag_t::is_operation_pending, true);
			}

			void end_operation()
			{
				set_flag(flag_t::is_operation_pending, false);
			}

			virtual_path_t path;

			database_items_t * database_items;

		};

		class item_generic_t : public item_base_t
		{

		public:

			item_content_base_t * get_base()
			{
				if (content_destroyed && !are_allowed_changes_when_deleted()) return content_destroyed;
				return content.get();
			}

			template<class T> T * get_content()
			{
				return static_cast<T*>(get_base());
			}

			virtual ~item_generic_t()
			{

			}

			void resave(output_t * output) final
			{
				content->resave(output);
			}

			bool is_destroyed()
			{
				return content_destroyed;
			}

			virtual void destroy() = 0;

			friend class database_items_t;
			friend class item_content_base_t;
			friend class item_task_t;

		protected:

			typedef std::function<std::unique_ptr<item_content_base_t>(const virtual_path_t &, item_generic_t *)> create_content_t;

			item_generic_t(database_items_t * database_items, const virtual_path_t & path, create_content_t create_content_func) :
				item_base_t(database_items, path), create_content_func(create_content_func), content_destroyed(nullptr)
			{
				content = std::move(create_content_func(path, this));
			}

			item_generic_t(const item_generic_t & other) : item_base_t(other), create_content_func(other.create_content_func), content(other.content->clone())
			{

			}

			template<class T> static std::unique_ptr<item_content_base_t> create_content(const virtual_path_t & path, item_generic_t * owner)
			{
				return std::move(std::make_unique<T>(owner));
			}

			create_content_t get_create_content_func()
			{
				return create_content_func;
			}

			void destroy_self(item_content_base_t * content);

		private:

			void undestroy()
			{
				this->content_destroyed = nullptr;
			}

			virtual item_generic_t * clone(const virtual_path_t & path) const = 0;

			item_generic_t * deatach()
			{
				item_generic_t * ret = clone(get_path());
				ret->set_deatached_flag();

				return ret;
			}

			create_content_t create_content_func;

			std::unique_ptr<item_content_base_t> content;
			item_content_base_t * content_destroyed;
		};

		template<class T> class item_t final : public item_generic_t
		{

		public:

			T * get()
			{
				return static_cast<T*>(get_base());
			}

			friend class database_items_t;

		private:

			item_t<T> * clone(const virtual_path_t & path) const final
			{
				return new item_t<T>(*this, path);
			}

			item_t(database_items_t * database_items, const virtual_path_t & path, create_content_t create_content_func) : item_generic_t(database_items, path, create_content_func)
			{

			}

			item_t(const item_t<T> & other, const virtual_path_t & path) : item_generic_t(other)
			{
				set_path(path);
			}

			static std::shared_ptr<item_t<T>> create_item(database_items_t * database_items, const virtual_path_t & path)
			{
				std::shared_ptr<item_t<T>> ret = std::shared_ptr<item_t<T>>(new item_t<T>(database_items, path, create_content<T>));
				ret->destroy();

				return ret;
			}

			void destroy() final
			{
				if (!is_destroyed())
				{
					destroy_self(get_content_destroyed());
				}
			}

			T * get_content_destroyed()
			{
				static T ret = item_content_base_t::destroyed_t();

				return &ret;
			}

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