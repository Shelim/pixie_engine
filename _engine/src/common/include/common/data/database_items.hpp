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

			enum class mode_reload_t
			{
				use_item_policy,
				force_sync,
				force_sync_now, // reload in the same thread, even if calling object is not from main thread
				deffered
			};

			void save(std::shared_ptr<item_t> item)
			{
				// ToDo
			}

			void save_as(std::shared_ptr<item_t> item, const virtual_path_t & path)
			{
				// ToDo
			}

			std::shared_ptr<item_t> save_copy_as(std::shared_ptr<item_t> item, const virtual_path_t & path)
			{
				// ToDo
			}

			void reload(std::shared_ptr<item_t> item)
			{
				// ToDo
			}

			void revert(std::shared_ptr<item_t> item)
			{
				// ToDo
			}

			std::shared_ptr<item_t> deatach(std::shared_ptr<item_t> item)
			{
				// ToDo
			}

			template<class T> std::shared_ptr<item_t> get_item(const virtual_path_t & path, mode_reload_t mode = mode_reload_t::use_item_policy)
			{
				// ToDo
				std::thread::id calling_thread_id = std::this_thread::get_id();

				std::shared_ptr<item_t> ret;

				auto iter = items.find(path);
				if (iter != items.end() && (ret = iter->second.lock()) != nullptr)
				{
					return ret;
				}

				ret = item_t::create_item<T>(path);
				auto policy = ret->get_base()->get_reload_policy();
				if (policy == item_content_base_t::policy_io_t::forbidden)
					return ret;

				if (mode == mode_reload_t::deffered)
				{
					items_deffered.push(ret);
				}
				else if (mode == mode_reload_t::force_sync || mode == mode_reload_t::force_sync_now || policy == item_content_base_t::policy_io_t::implicit_sync || policy == item_content_base_t::policy_io_t::explicit_sync)
				{
					if (calling_thread_id != main_thread_id && mode != mode_reload_t::force_sync_now)
					{
						items_to_reload.enqueue_to_sync(ret);
					}
					else
					{
						ret->
					}
				}
				else
				{
					if (calling_thread_id != main_thread_id)
					{
						items_to_reload.enqueue_to_sync(ret);
					}
				}


				return ret;
			}

			template<class T> std::shared_ptr<items_collection_t > get_items_collection(const virtual_path_t & name)
			{
				// ToDo
			}

			auto deffered_left()
			{
				return items_deffered.size();
			}
			void init_update()
			{
				// ToDo
			}

		private:

			void update_async()
			{
				// ToDo
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

			class items_progress_t
			{

			public:

				std::shared_ptr<item_t> get_item_to_init()
				{
					if (items_to_init.is_empty()) return nullptr;
					
					return items_to_init.pop();
				}

				std::shared_ptr<item_t> get_item_to_async()
				{
					if (items_to_async.is_empty()) return nullptr;

					return items_to_async.pop();
				}

				std::shared_ptr<item_t> get_item_to_end()
				{
					if (items_to_end.is_empty()) return nullptr;

					return items_to_end.pop();
				}

				std::shared_ptr<item_t> get_item_to_sync()
				{
					if (items_to_sync.is_empty()) return nullptr;

					return items_to_sync.pop();
				}

				void enqueue_to_init(std::shared_ptr<item_t> value)
				{
					items_to_init.push(value);
				}

				void enqueue_to_async(std::shared_ptr<item_t> value)
				{
					items_to_async.push(value);
				}

				void enqueue_to_end(std::shared_ptr<item_t> value)
				{
					items_to_end.push(value);
				}

				void enqueue_to_sync(std::shared_ptr<item_t> value)
				{
					items_to_sync.push(value);
				}

			private:

				queue_t<std::shared_ptr<item_t> > items_to_init;
				queue_t<std::shared_ptr<item_t> > items_to_async;
				queue_t<std::shared_ptr<item_t> > items_to_end;

				queue_t<std::shared_ptr<item_t> > items_to_sync;

			};

			items_progress_t items_to_resave;
			items_progress_t items_to_reload;

			queue_t<std::shared_ptr<item_t> > items_deffered;

			std::map<virtual_path_t, std::weak_ptr<item_base_t> > items;

			std::shared_ptr<logger_t> logger;
			std::shared_ptr<database_t> database;

			bool end_update_data;
			std::thread update_items_thread;
		};
	}

}

#endif