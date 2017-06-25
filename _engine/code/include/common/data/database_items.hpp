#ifndef ENGINE_COMMON_DATA_DATABASE_ITEMS_HPP
#define ENGINE_COMMON_DATA_DATABASE_ITEMS_HPP
#pragma once

#include "common/utility/vfs/virtual_path.hpp"
#include "common/data/item.hpp"
#include "common/data/database_providers.hpp"
#include "common/task/tasks.hpp"
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
		class database_items_t final
		{

		public:

			database_items_t(std::shared_ptr<logger_t> logger, std::shared_ptr<database_providers_t> database_providers, std::shared_ptr<tasks_t> tasks) : logger(logger), database_providers(database_providers), tasks(tasks)
			{
				main_thread_id = std::this_thread::get_id();

				end_update_data = false;
				update_items_thread = std::thread([this] { update_items(); });
			}

			~database_items_t()
			{
				end_update_data = true;
				update_items_thread.join();
				init_update();
			}

			std::shared_ptr<database_providers_t> get_database_providers()
			{
				return database_providers;
			}

			void save(std::shared_ptr<item_generic_t> item);

			void save_as(std::shared_ptr<item_generic_t> item, const virtual_path_t & path)
			{
				{
					std::lock_guard<std::recursive_mutex> guard(mutex_items);
					logger->p_msg(_U("Saving '#1#' as '#2#'..."), item->get_path(), path);
					auto iter = items.find(item->get_path());
					if (iter != items.end())
					{
						items.erase(iter);
					}

					iter = items.find(path);
					if (iter != items.end())
					{
						iter->second->destroy();
					}
					items[path] = item;
				}
				item->path = path;
				save(item);
			}

			std::shared_ptr<item_generic_t> save_copy_as_generic(std::shared_ptr<item_generic_t> item, const virtual_path_t & path)
			{
				std::shared_ptr<item_generic_t> clone = std::shared_ptr<item_generic_t>(item->clone(path));
				save(clone);

				std::shared_ptr<item_generic_t> ret;
				{
					std::lock_guard<std::recursive_mutex> guard(mutex_items);
					logger->p_msg(_U("Cloning '#1#' as '#2#'..."), item->get_path(), path);
					auto iter = items.find(path);
					if (iter != items.end())
					{
						reload(iter->second);
						return iter->second;
					}
					items[path] = clone;
				}


				return clone;
			}

			template<class T> std::shared_ptr<item_t<T> > save_copy_as(std::shared_ptr<item_generic_t> item, const virtual_path_t & path)
			{
				return std::static_pointer_cast<item_t<T>> (save_copy_as_generic(item, path));
			}

			void reload(std::shared_ptr<item_generic_t> item);

			void revert(std::shared_ptr<item_generic_t> item)
			{
				reload(item);
			}

			std::shared_ptr<item_generic_t> deatach_generic(std::shared_ptr<item_generic_t> item)
			{
				if (item->is_deatached()) return item;

				logger->p_msg(_U("Detaching '#1#'..."), item->get_path());

				return std::shared_ptr<item_generic_t>(item->deatach());
			}

			template<class T> std::shared_ptr<item_t<T> > deatach(std::shared_ptr<item_generic_t> item)
			{
				return std::static_pointer_cast<item_t<T>> (deatach_generic(item));
			}

			template<class T> std::shared_ptr<item_t<T> > get_item(const virtual_path_t & path)
			{
				std::thread::id calling_thread_id = std::this_thread::get_id();

				std::shared_ptr<item_t<T> > ret;
				{
					std::lock_guard<std::recursive_mutex> guard(mutex_items);
					auto iter = items.find(path);
					if (iter != items.end())
					{
						return std::static_pointer_cast<item_t<T> > (iter->second);
					}

					ret = item_t<T>::create_item(this, path);
					items[path] = ret;
				}

				reload(std::static_pointer_cast<item_generic_t>(ret));

				return ret;
			}

			template<class T> std::shared_ptr<item_t<T> > load_item_detached(std::unique_ptr<input_t> input);

			template<class T> std::shared_ptr<items_collection_t > get_items_collection(const virtual_path_t & name)
			{
				// ToDo
			}

			void init_update();
		
			friend class item_generic_t;

			std::shared_ptr<logger_t> get_logger()
			{
				return logger;
			}

		private:

			void perform_destroy(const virtual_path_t & path);

			void update_async()
			{

			}

			void update_items()
			{
				logger->p_msg(_U("Starting data thread..."));
				for (;;)
				{
					if (end_update_data) break;

					update_async();
					std::this_thread::sleep_for(std::chrono::milliseconds(10));
				}
				logger->p_msg(_U("Data threaded completed!"));
			}

			std::thread::id main_thread_id;

			std::map<virtual_path_t, std::shared_ptr<item_generic_t> > items;
			std::recursive_mutex mutex_items;

			std::shared_ptr<logger_t> logger;
			std::shared_ptr<database_providers_t> database_providers;
			std::shared_ptr<tasks_t> tasks;

			bool end_update_data;
			std::thread update_items_thread;
		};
	}

}

#endif