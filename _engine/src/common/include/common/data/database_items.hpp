#ifndef ENGINE_COMMON_DATA_DATABASE_ITEMS_HPP
#define ENGINE_COMMON_DATA_DATABASE_ITEMS_HPP
#pragma once

#include "common/virtual_path.hpp"
#include "common/queue.hpp"
#include "common/data/item.hpp"
#include "common/data/database.hpp"
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

			database_items_t(std::shared_ptr<logger_t> logger, std::shared_ptr<database_t> database) : logger(logger), database(database)
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

			std::shared_ptr<database_t> get_database()
			{
				return database;
			}

			void save(std::shared_ptr<item_generic_t> item);

			void save_as(std::shared_ptr<item_generic_t> item, const virtual_path_t & path)
			{
				{
					std::lock_guard<std::recursive_mutex> guard(mutex_items);
					auto iter = items.find(item->get_path());
					if (iter != items.end())
					{
						items.erase(iter);
					}

					iter = items.find(path);
					if (iter != items.end())
					{
						auto ptr = iter->second.lock();
						ptr->destroy();
					}
					items[path] = item;
				}
				item->path = path;
				save(item);
			}

			std::shared_ptr<item_generic_t> save_copy_as(std::shared_ptr<item_generic_t> item, const virtual_path_t & path)
			{
				std::shared_ptr<item_generic_t> clone = std::shared_ptr<item_generic_t>(item->clone(path));
				save(clone);

				std::shared_ptr<item_generic_t> ret;
				{
					std::lock_guard<std::recursive_mutex> guard(mutex_items);
					auto iter = items.find(path);
					if (iter != items.end() && (ret = std::static_pointer_cast<item_generic_t>(iter->second.lock())))
					{
						reload(ret);
						return ret;
					}
				}

				return clone;
			}

			void reload(std::shared_ptr<item_generic_t> item);

			void revert(std::shared_ptr<item_generic_t> item)
			{
				reload(item);
			}

			std::shared_ptr<item_generic_t> deatach(std::shared_ptr<item_generic_t> item)
			{
				if (item->is_deatached()) return item;

				return std::shared_ptr<item_generic_t>(item->deatach());
			}

			template<class T> std::shared_ptr<item_t<T> > get_item(const virtual_path_t & path)
			{
				std::thread::id calling_thread_id = std::this_thread::get_id();

				std::shared_ptr<item_t<T> > ret;
				{
					std::lock_guard<std::recursive_mutex> guard(mutex_items);
					auto iter = items.find(path);
					if (iter != items.end() && (ret = std::static_pointer_cast<item_t<T> >(iter->second.lock())))
					{
						return ret;
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

		private:

			void perform_destroy(const virtual_path_t & path);

			void update_async();

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
			
			queue_t<std::shared_ptr<item_generic_t> > items_reload_next;

			std::map<virtual_path_t, std::weak_ptr<item_generic_t> > items;
			std::recursive_mutex mutex_items;

			std::shared_ptr<logger_t> logger;
			std::shared_ptr<database_t> database;

			std::vector<std::unique_ptr<item_operation_t> > operations;

			void create_operation(std::shared_ptr<item_generic_t> item, std::unique_ptr<input_t> input, bool log)
			{
				std::lock_guard<std::recursive_mutex> guard(operations_mutex);
				operations.push_back(std::move(std::make_unique<item_operation_t>(item, std::move(input), log ? logger : nullptr)));
			}
			void create_operation(std::shared_ptr<item_generic_t> item, std::unique_ptr<output_t> output, bool log)
			{
				std::lock_guard<std::recursive_mutex> guard(operations_mutex);
				operations.push_back(std::move(std::make_unique<item_operation_t>(item, std::move(output), log ? logger : nullptr)));
			}
			void create_operation(std::shared_ptr<item_generic_t> item, item_operation_t::free_t free, bool log)
			{
				std::lock_guard<std::recursive_mutex> guard(operations_mutex);
				operations.push_back(std::move(std::make_unique<item_operation_t>(item, free, log ? logger : nullptr)));
			}
			void execute_operations(item_operation_t::step_t::caller_t caller)
			{
				std::lock_guard<std::recursive_mutex> guard(operations_mutex);
				for (std::size_t op = operations.size(); op--> 0;)
				{
					operations[op]->execute_steps(caller);
				}
			}
			void clear_completed_operations()
			{
				std::lock_guard<std::recursive_mutex> guard(operations_mutex);

				for (std::size_t op = operations.size(); op-- > 0;)
				{
					if (operations[op]->is_completed())
						operations.erase(operations.begin() + op);
				}
			}

			bool end_update_data;
			std::thread update_items_thread;
			std::recursive_mutex operations_mutex;
		};
	}

}

#endif