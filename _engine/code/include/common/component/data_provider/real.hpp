#ifndef ENGINE_COMPONENT_DATA_PROVIDER_REAL_HPP
#define ENGINE_COMPONENT_DATA_PROVIDER_REAL_HPP
#pragma once

#include "component/data_provider.hpp"
#include "component/logger.hpp"
#include "utility/text/ustring.hpp"
#include "provider/scanners.hpp"

namespace engine
{

	class data_provider_real_t : public data_provider_t
	{

	public:

		data_provider_real_t(std::shared_ptr<logger_t> logger, std::shared_ptr<config_t> config, std::shared_ptr<save_location_provider_t> save_location_provider, std::shared_ptr<scanners_provider_t> scanners_provider) : logger(logger), config(config), save_location_provider(save_location_provider), scanners_provider(scanners_provider), requested_rescan(true)
		{
			auto task = logger->log_task_start(data, "Initializing database providers engine"_u);

			rescan();
			refresh_changes();
			log_changes();
			refresh_virtual_path_type_changes();
			requested_rescan = true;
			rescan();

			logger->log_task_done(task);

			end_scanning = false;
			check_filesystem_thread = std::thread([this] { check_filesystem(); });
		}

		~data_provider_real_t()
		{
			end_scanning = true;
			check_filesystem_thread.join();
		}

		const data::changes_t & get_changes() const final
		{
			return changes;
		}

		void update() final
		{
			changes.clear();
			refresh_changes();
			log_changes();
			refresh_virtual_path_type_changes();
		}

		void delete_item(const virtual_path_t & filename) final
		{
			data::provider_t * provider = get_state()->get_provider(filename);
			if (provider)
				provider->delete_all();
		}

		void delete_item_newest(const virtual_path_t & filename) final
		{
			data::provider_t * provider = get_state()->get_provider(filename);
			if (provider)
				provider->delete_newest();
		}

		void copy_item(const virtual_path_t & src, const virtual_path_t & dst) final
		{
			data::input_t::buffer_t buffer;
			{
				std::unique_ptr<data::input_t> input = get_input(src);
				buffer = input->read_buffer();;
			}
			{
				std::unique_ptr<data::output_t> output = get_output(dst);
				output->write_buffer(buffer);
			}
		}

		std::unique_ptr<data::input_t> get_input(const virtual_path_t & filename) final
		{
			std::lock_guard<std::recursive_mutex> guard(mutex_database_current);

			data::provider_t * provider = get_state()->get_provider(filename);
			if (!provider)
				return std::move(std::make_unique<data::input_void_t>(filename));

			return std::move(provider->construct_input());
		}

		std::unique_ptr<data::output_t> get_output(const virtual_path_t & filename) final
		{
			std::lock_guard<std::recursive_mutex> guard(mutex_database_current);

			data::provider_t * provider = get_state()->get_provider(filename);
			if (!provider)
				return std::move(engine::platform::create_new(filename, save_location_provider));

			return std::move(provider->construct_output());
		}

		types_t get_virtual_path_type_changes() const final
		{
			return type_changes;
		}

	private:

		data::state_t * get_state()
		{
			return &current;
		}

		void log_changes()
		{
			for (auto & change : changes)
			{
				switch (change.get_type())
				{
				case data::change_t::added: logger->log_msg(data, "DataObject '#1#' was added!"_u, change.get_path()); break;
				case data::change_t::updated: logger->log_msg(data, "DataObject '#1#' was updated!"_u, change.get_path()); break;
				case data::change_t::deleted: logger->log_msg(data, "DataObject '#1#' was deleted!"_u, change.get_path()); break;
				}
			}
		}

		void check_filesystem()
		{
			logger->log_msg(data, "Will monitor filesystem for asset changes"_u);
			for (;;)
			{
				if (end_scanning) break;

				std::this_thread::sleep_for(std::chrono::milliseconds(10));
				rescan();
			}
			logger->log_msg(data, "Finished monitoring filesystem for asset changes"_u);
		}

		std::thread check_filesystem_thread;
		bool end_scanning;

		bool requested_rescan;

		void rescan();
		void refresh_virtual_path_type_changes();

		void refresh_changes()
		{
			if (!requested_rescan)
			{
				if (mutex_database_next.try_lock())
				{
					changes = data::state_t::calculate_changes(next, current);
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
		std::shared_ptr<config_t> config;
		std::shared_ptr<save_location_provider_t> save_location_provider;
		std::shared_ptr<scanners_provider_t> scanners_provider;

		data::changes_t changes;
		types_t type_changes;


		data::state_t current;
		data::state_t next;
	};
}

#endif