#ifndef ENGINE_COMMON_DATA_DATABASE_HPP
#define ENGINE_COMMON_DATA_DATABASE_HPP
#pragma once

#include "common/data/database_state.hpp"
#include "common/data/database_changes.hpp"
#include "common/data/scanners.hpp"
#include "common/data/item.hpp"
#include "common/platform.hpp"
#include "common/logger_container.hpp"
#include "common/config.hpp"
#include "common/data/database_items.hpp"
#include <memory>
#include <queue>
#include <mutex>

namespace engine
{
	namespace data
	{
		class database_t final
		{

		public:

			database_t(std::shared_ptr<logger_t> logger, std::shared_ptr<platform_t> platform, std::shared_ptr<config_t> config, std::unique_ptr<database_items_t> database_items) : logger(logger), platform(platform), config(config), database_items(std::move(database_items)), requested_rescan(true)
			{
				auto task = logger->p_task_start(_U("Initializing database engine"));

				rescan();
				refresh_changes();
				log_changes();
				refresh_virtual_path_type_changes();
				requested_rescan = true;
				rescan();

				logger->p_task_done(task);

				end_scanning = false;
				end_update_data = false;
				check_filesystem_thread = std::thread([this] { check_filesystem(); });
				update_items_thread = std::thread([this] { update_items(); });
			}

			~database_t()
			{
				end_update_data = true;
				end_scanning = true;
				update_items_thread.join();
				check_filesystem_thread.join();
				database_items->update();
			}

			const database_changes_t & get_changes() const
			{
				return changes;
			}

			void init_update()
			{
				changes.clear();
				refresh_changes();
				log_changes();
				refresh_virtual_path_type_changes();
				database_items->update();
			}

			std::unique_ptr<input_t> get_input(const virtual_path_t & filename)
			{
				std::lock_guard<std::recursive_mutex> guard(mutex_database_current);

				provider_t * provider = get_state()->get_provider(filename);
				if (!provider)
					return std::move(std::make_unique<input_void_t>(filename));

				return std::move(provider->construct_input());
			}

			std::unique_ptr<output_t> get_output(const virtual_path_t & filename)
			{
				std::lock_guard<std::recursive_mutex> guard(mutex_database_current);

				provider_t * provider = get_state()->get_provider(filename);
				if (!provider)
					return std::move(std::make_unique<output_void_t>(filename));

				return std::move(provider->construct_output());
			}

			typedef std::set<virtual_path_t::type_t> types_t;

			types_t get_virtual_path_type_changes() const
			{
				return type_changes;
			}

			bool has_virtual_path_type_changed(virtual_path_t::type_t type)
			{
				return type_changes.find(type) != type_changes.end();
			}

			database_items_t * get_database_items()
			{
				return database_items.get();
			}

		private:

			database_state_t * get_state()
			{
				return &current;
			}

			void log_changes()
			{
				for (auto & change : changes)
				{
					switch (change.get_type())
					{
					case database_change_t::added: logger->p_msg(_U("DataObject '#1#' was added!"), change.get_path()); break;
					case database_change_t::updated: logger->p_msg(_U("DataObject '#1#' was updated!"), change.get_path()); break;
					case database_change_t::deleted: logger->p_msg(_U("DataObject '#1#' was deleted!"), change.get_path()); break;
					}
				}
			}

			void check_filesystem()
			{
				logger->p_msg(_U("Will monitor filesystem for asset changes"));
				for (;;)
				{
					if (end_scanning) break;

					std::this_thread::sleep_for(std::chrono::milliseconds(10));
					rescan();
				}
				logger->p_msg(_U("Finished monitoring filesystem for asset changes"));
			}

			void update_items()
			{
				logger->p_msg(_U("Starting data thread..."));
				for (;;)
				{
					if (end_update_data) break;

					database_items->update_async();
					std::this_thread::sleep_for(std::chrono::milliseconds(10));
				}
				logger->p_msg(_U("Data threaded completed!"));
			}
			
			std::thread check_filesystem_thread;
			std::thread update_items_thread;
			bool end_scanning;
			bool end_update_data;

			std::unique_ptr<scanners_t::collection_t> platform_get_scanners_collection();
			bool requested_rescan;

			void rescan();

			void refresh_virtual_path_type_changes();

			void refresh_changes()
			{
				if (!requested_rescan)
				{
					if (mutex_database_next.try_lock())
					{
						changes = database_state_t::calculate_changes(next, current);
						{
							std::lock_guard<std::recursive_mutex> guard(mutex_database_current);
							std::swap(current, next);
						}
						requested_rescan = true;
						mutex_database_next.unlock();
					}
				}
			}

			std::recursive_mutex mutex_database_current;
			std::recursive_mutex mutex_database_next;

			std::shared_ptr<logger_t> logger;
			std::shared_ptr<platform_t> platform;
			std::shared_ptr<config_t> config;

			std::unique_ptr<database_items_t> database_items;

			database_changes_t changes;
			types_t type_changes;


			database_state_t current;
			database_state_t next;
		};
	}
}

#endif