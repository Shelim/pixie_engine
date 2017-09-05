#ifndef ENGINE_COMPONENT_DATA_PROVIDER_REAL_HPP
#define ENGINE_COMPONENT_DATA_PROVIDER_REAL_HPP
#pragma once

#include "component/data_archives.hpp"
#include "component/data_source.hpp"
#include "component/logger.hpp"
#include "utility/text/ustring.hpp"
#include "provider/scanners.hpp"

namespace engine
{
	class data_source_real_actual_t
	{


	public:

		data_source_real_actual_t(std::shared_ptr<logger_t> logger, std::shared_ptr<config_t> config, std::shared_ptr<save_location_provider_t> save_location_provider, std::shared_ptr<scanners_provider_t> scanners_provider, std::shared_ptr<messenger_t> messenger, std::shared_ptr<data_archives_t> data_archives) : logger(logger), config(config), save_location_provider(save_location_provider), scanners_provider(scanners_provider), messenger(messenger), requested_rescan(true)
		{
			auto task = logger->log_task_start(data, "Initializing database providers engine"_u);

			rescan();
			refresh_changes();
			log_changes();
			refresh_virtual_path_type_changes();
			requested_rescan = true;
			rescan();

			logger->log_task_done(task);

		}

		void service()
		{
			rescan();
		}

		const data::changes_t & get_changes() const
		{
			return changes;
		}

		void update()
		{
			changes.clear();
			refresh_changes();
			log_changes();
			refresh_virtual_path_type_changes();
		}

		void delete_item(const virtual_path_t & filename)
		{
			data::provider_t * provider = get_state()->get_provider(filename);
			if (provider)
				provider->delete_all();
		}

		void delete_item_newest(const virtual_path_t & filename)
		{
			data::provider_t * provider = get_state()->get_provider(filename);
			if (provider)
				provider->delete_newest();
		}

		void copy_item(const virtual_path_t & src, const virtual_path_t & dst)
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

		std::unique_ptr<data::input_t> get_input(const virtual_path_t & filename)
		{
			std::lock_guard<std::recursive_mutex> guard(mutex_database_current);

			data::provider_t * provider = get_state()->get_provider(filename);
			if (!provider)
				return std::move(std::make_unique<data::input_void_t>(filename));

			return std::move(provider->construct_input());
		}

		std::unique_ptr<data::output_t> get_output(const virtual_path_t & filename)
		{
			std::lock_guard<std::recursive_mutex> guard(mutex_database_current);

			data::provider_t * provider = get_state()->get_provider(filename);
			if (!provider)
				return std::move(engine::platform::create_new(filename, save_location_provider));

			return std::move(provider->construct_output());
		}

		data_source_t::types_t get_virtual_path_type_changes() const
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
		std::shared_ptr<messenger_t> messenger;

		data::changes_t changes;
		data_source_t::types_t type_changes;


		data::state_t current;
		data::state_t next;
	};


	class data_source_update_task_t : public task_base_t
	{

	public:

		data_source_update_task_t(std::shared_ptr<data_source_real_actual_t> actual) : actual(actual)
		{

		}

		result_t run(process::token_t * token) final
		{
			actual->update();
			return result_t::running;
		}

		ustring_t get_name() const final
		{
			return "Data provider update task"_u;
		}

	private:

		std::shared_ptr<data_source_real_actual_t> actual;
	};
	
	class data_source_real_service_t : public service_base_t
	{

	public:

		data_source_real_service_t(std::shared_ptr<process::runner_engine_thread_t<process::runner_thread_t::engine_update> > runner_update_task, std::unique_ptr<data_source_update_task_t> update_task, std::shared_ptr<data_source_real_actual_t> actual) : actual(actual)
		{
			runner_update_task->add_looped_task(std::move(update_task));
		}

		ustring_t get_name() const final
		{
			return "Data provider Storage"_u;
		}

		task_base_t::result_t run() final
		{
			actual->service();
			return task_base_t::result_t::running;
		}

		std::chrono::duration<double> get_sleep_after() const final
		{
			return std::chrono::milliseconds(250);
		}

	private:

		std::shared_ptr<data_source_real_actual_t> actual;

	};

	class data_source_real_t : public data_source_t
	{

	public:

		data_source_real_t(std::unique_ptr<service_t<data_source_real_service_t>> service, std::shared_ptr<data_source_real_actual_t> actual) : service(std::move(service)), actual(actual)
		{
			this->service->start();
		}

		~data_source_real_t()
		{
			this->service->end();
		}

		const data::changes_t & get_changes() const final
		{
			return actual->get_changes();
		}

		void delete_item(const virtual_path_t & filename) final
		{
			actual->delete_item(filename);
		}

		void delete_item_newest(const virtual_path_t & filename) final
		{
			actual->delete_item_newest(filename);
		}

		void copy_item(const virtual_path_t & src, const virtual_path_t & dst) final
		{
			actual->copy_item(src, dst);
		}

		std::unique_ptr<data::input_t> get_input(const virtual_path_t & filename) final
		{
			return std::move(actual->get_input(filename));
		}

		std::unique_ptr<data::output_t> get_output(const virtual_path_t & filename) final
		{
			return std::move(actual->get_output(filename));
		}

		types_t get_virtual_path_type_changes() const final
		{
			return actual->get_virtual_path_type_changes();
		}

	private:

		std::unique_ptr<service_t<data_source_real_service_t>> service;
		std::shared_ptr<data_source_real_actual_t> actual;
	};
}

#endif