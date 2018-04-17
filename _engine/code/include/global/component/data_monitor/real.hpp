#ifndef ENGINE_COMPONENT_DATA_MONITOR_REAL_HPP
#define ENGINE_COMPONENT_DATA_MONITOR_REAL_HPP
#pragma once

#include "global/component/data_monitor.hpp"
#include "utility/concurrention/signal.hpp"

namespace engine
{
	class data_monitor_actual_t
	{

	public:

			
		void rescan(data::scanners_t & scanners)
		{
			for(auto & item : scanners)
			{
//				item->scan();
			}
		}

	};

	class data_monitor_executor_t
	{

	public:

		data_monitor_executor_t(std::shared_ptr<data_monitor_actual_t> actual) : actual(actual)
		{

		}

		void request_rescan(data::scanners_t && scanners)
		{
			{
				std::lock_guard<std::mutex> guard(mutex);
				if(flags.is_flag(flag_t::next_rescanning_requested))
				{
					for(std::size_t i = 0; i < scanners.size(); i++)
					{
						auto & scanner = scanners[i];
						if(std::find_if(this->scanners.begin(), this->scanners.end(), [&scanner](std::unique_ptr<data::scanner_t> & prev){ return *scanner==*prev; }) == this->scanners.end())
							this->scanners.push_back(std::move(scanner));
					}
					return;
				}
				else
				{
					flags.set_flag(flag_t::next_rescanning_requested, true);
					std::swap(this->scanners, scanners);
				}
			}
			messages.push(message_t::rescan_requested);
		}

		void rescan(data::scanners_t && scanners)
		{
			std::lock_guard<std::mutex> guard(scan_mutex);
			actual->rescan(scanners);
			scanners.clear();
		}

		void terminate_pool()
		{
			messages.push(message_t::terminate_monitor);
		}

		task_base_t::result_t run()
		{
			for(;;)
			{
				data::scanners_t scanners;
				message_t msg = messages.pop();
				switch(msg)
				{
					case message_t::rescan_requested:
					{
						{
							std::lock_guard<std::mutex> guard(mutex);
							flags.set_flag(flag_t::next_rescanning_requested, false);
							std::swap(scanners, this->scanners);
						}
						rescan(std::move(scanners));
					}
					break;
					case message_t::terminate_monitor:
						return task_base_t::result_t::completed;
				}
			}
		}

	private:
		
		enum class message_t
		{
			rescan_requested,
			terminate_monitor
		};

		enum class flag_t
		{
			next_rescanning_requested,
			count
		};

		flags_t<flag_t> flags;
		std::mutex mutex;
		std::mutex scan_mutex;

		concurrent_queue_t<message_t> messages;
		
		data::scanners_t scanners;
		

		std::shared_ptr<data_monitor_actual_t> actual;

	};

	class data_monitor_service_t : public service_base_t
	{

	public:

		data_monitor_service_t(std::shared_ptr<logger_t> logger, std::shared_ptr<messenger_data_monitor_t> messenger_data_monitor, std::shared_ptr<data_monitor_executor_t> data_monitor_executor) : logger(logger), messenger_data_monitor(messenger_data_monitor), data_monitor_executor(data_monitor_executor)
		{
			logger->log_global_msg(data, "Data monitor service has started"_u);
			messenger_instance = messenger_data_monitor->register_callback([this](messenger::msg_data_monitor_t* msg)
			{
				this->data_monitor_executor->request_rescan(std::move(msg->extract_scanners()));
			});
		}

		~data_monitor_service_t()
		{
			logger->log_global_msg(data, "Data monitor service has concluded"_u);
		}

		ustring_t get_name() const final
		{
			return "Data monitor service"_u;
		}

		task_base_t::result_t run() final
		{
			return data_monitor_executor->run();
		}

		void on_end_requested() final
		{
			data_monitor_executor->terminate_pool();
		}

	private:

		std::shared_ptr<logger_t> logger;
		std::shared_ptr<messenger_data_monitor_t> messenger_data_monitor;
		std::unique_ptr<messenger::instance_t<messenger::msg_data_monitor_t> > messenger_instance;
		std::shared_ptr<data_monitor_executor_t> data_monitor_executor;

	};

	class data_monitor_real_t : public data_monitor_t
	{

	public:

		data_monitor_real_t(std::shared_ptr<logger_t> logger, std::unique_ptr<service_t<data_monitor_service_t>> service) : logger(logger), service(std::move(service))
		{
			auto task_id = logger->log_global_task_start(data, "Initializing monitor [data]"_u);
			this->service->start();
			logger->log_global_task_done(task_id);
		}

		~data_monitor_real_t()
		{
			auto task_id = logger->log_global_task_start(data, "Monitor [data] is being disposed"_u);
			this->service->end();
			logger->log_global_task_done(task_id);
		}

	private:

		std::shared_ptr<logger_t> logger;
		std::unique_ptr<service_t<data_monitor_service_t>> service;


	};
}


#endif