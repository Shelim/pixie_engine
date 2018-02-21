#ifndef ENGINE_COMPONENT_THREAD_ACCOUNTER_REAL_HPP
#define ENGINE_COMPONENT_THREAD_ACCOUNTER_REAL_HPP
#pragma once

#include "global/component/thread_accounter.hpp"
#include "global/core/messenger/messenger.hpp"

namespace engine
{

	class thread_accounter_provider_base_t
	{

	public:

		virtual ~thread_accounter_provider_base_t()
		{

		}

		class thread_info_t
		{

		public:

			thread_info_t(thread_t * thread) : thread(thread), creation_time(std::chrono::steady_clock::now())
			{

			}

			virtual ~thread_info_t()
			{

			}

			thread_t * get_thread()
			{
				return thread;
			}

			std::chrono::duration<double> get_total_time() const
			{
				return std::chrono::steady_clock::now() - creation_time;
			}

			virtual std::chrono::duration<double> get_cpu_usage_in_last_second() const = 0;

		private:

			thread_t * thread;
			std::chrono::steady_clock::time_point creation_time;

		};

		typedef std::vector<std::unique_ptr<thread_info_t> > threads_info_t;

		void add_thread(thread_t * thread)
		{
			for(auto & thread_info : threads_info)
			{
				if(thread_info->get_thread() == thread &&
					thread_info->get_thread()->get_id() == thread->get_id())
					{
						logger->log_global_warn(threads, "The thread #1# [id: #2#] attempted to account itself second time (original name: #3# called in #4####5#)"_u, thread->get_name(), thread->get_id(), thread_info->get_thread()->get_name(), thread_info->get_thread()->get_app(), thread_info->get_thread()->get_app_instance_id());
						return;
					}
			}

			threads_info.push_back(create_thread_info(thread));
		}

		void remove_thread(thread_t * thread)
		{
			for(auto iter = threads_info.begin(); iter != threads_info.end(); iter++)
			{
				if((*iter)->get_thread() == thread &&
					(*iter)->get_thread()->get_id() == thread->get_id())
				{
					threads_info.erase(iter);
					return;
				}
			}
			
			logger->log_global_warn(threads, "The thread #1# [id: #2#] attempted to free itself, while it was not registered in the first place"_u, thread->get_name(), thread->get_id());
		}

		const threads_info_t & get_threads() const
		{
			return threads_info;
		}

	protected:

		thread_accounter_provider_base_t(std::shared_ptr<logger_t> logger) : logger(logger)
		{

		}

		std::shared_ptr<logger_t> get_logger()
		{
			return logger;
		}

	private:

		virtual std::unique_ptr<thread_info_t> create_thread_info(thread_t * thread) = 0;
		threads_info_t threads_info;
		std::shared_ptr<logger_t> logger;
	
	};

	REGISTER_PROVIDER_BASE_TYPE(thread_accounter_t, thread_accounter_provider_base_t)

	class thread_accounter_actual_t
	{

	public:

		thread_accounter_actual_t(std::shared_ptr<logger_t> logger, std::shared_ptr<messenger_accountable_thread_t> messenger_accountable_thread, std::unique_ptr<holder_t<thread_accounter_t> > thread_accounter_provider) : logger(logger), messenger_accountable_thread(messenger_accountable_thread), thread_accounter_provider(std::move(thread_accounter_provider))
		{
			logger->log_global_msg(threads, "Thread accounter actual component has started"_u);

			messenger_instance = messenger_accountable_thread->register_callback([this](messenger::msg_accountable_thread_t* msg){on_accountable_changed(msg);}, messenger::history_t::dump_if_available);
		}

		~thread_accounter_actual_t()
		{
			messenger_instance.reset();
			{
				std::lock_guard<std::mutex> guard(mutex);
				auto task_id = logger->log_global_task_start(threads, "Thread accounter actual component is being disposed"_u);
				auto & threads_info = thread_accounter_provider->get_provider()->get_threads();
				if(threads_info.size() == 0)
					logger->log_global_msg(threads, "All threads have been accounted for. Clean exit"_u);
				else
				{
					if(threads_info.size() == 1)
						logger->log_global_msg(threads, "#1# thread is still not accounted for. Will release it after accounter has died. The name of it follows:"_u, threads_info.size());
					else
						logger->log_global_msg(threads, "#1# threads are still not accounted for. Will release them after accounter has died. List of them follows:"_u, threads_info.size());
					for(auto & thread_info : threads_info)
					{
						logger->log_global_msg(threads, "Thread #1# [id: #2#] in #3####4# is not yet accounted for"_u, thread_info->get_thread()->get_name(), thread_info->get_thread()->get_id(), thread_info->get_thread()->get_app(), thread_info->get_thread()->get_app_instance_id());
					}
				}
				logger->log_global_task_done(task_id);	
			}
		}

		thread_accounter_t::threads_info_t get_snapshot()
		{
			std::lock_guard<std::mutex> guard(mutex);
			thread_accounter_t::threads_info_t ret;

			for(auto & thread_info : thread_accounter_provider->get_provider()->get_threads())
			{
				ret.emplace_back(thread_info->get_thread()->get_id(), thread_info->get_thread()->get_app(), thread_info->get_thread()->get_app_instance_id(), thread_info->get_thread()->get_name(), thread_info->get_total_time(), thread_info->get_cpu_usage_in_last_second());
			}

			return ret;
		}

	private:

		std::mutex mutex;

		std::shared_ptr<logger_t> logger;
		std::shared_ptr<messenger_accountable_thread_t> messenger_accountable_thread;
		std::unique_ptr<holder_t<thread_accounter_t> > thread_accounter_provider;
		std::unique_ptr<messenger::instance_t<messenger::msg_accountable_thread_t> > messenger_instance;

		void on_accountable_created(messenger::msg_accountable_thread_t * msg)
		{
			if(msg->get_object()->get_app() == app_t::kind_t::_engine)
				logger->log_global_msg(threads, "New global thread was just created: #1# [id: #2#]"_u, msg->get_object()->get_name(), msg->get_object()->get_id());
			else
				logger->log_global_msg(threads, "New thread was just created for #3####4#: #1# [id: #2#]"_u, msg->get_object()->get_name(), msg->get_object()->get_id(), msg->get_object()->get_app(), msg->get_object()->get_app_instance_id());
			
			thread_accounter_provider->get_provider()->add_thread(msg->get_object());
		}

        void on_accountable_destroyed(messenger::msg_accountable_thread_t * msg)
		{
			if(msg->get_object()->get_app() == app_t::kind_t::_engine)
				logger->log_global_msg(threads, "Global thread was just destroyed: #1# [id: #2#]"_u, msg->get_object()->get_name(), msg->get_object()->get_id());
			else
				logger->log_global_msg(threads, "Thread was just destroyed for #3####4#: #1# [id: #2#]"_u, msg->get_object()->get_name(), msg->get_object()->get_id(), msg->get_object()->get_app(), msg->get_object()->get_app_instance_id());
			
			thread_accounter_provider->get_provider()->remove_thread(msg->get_object());
		}
		
		void on_accountable_changed(messenger::msg_accountable_thread_t* msg)
		{
			std::lock_guard<std::mutex> guard(mutex);
			switch(msg->get_type())
			{
				case messenger::accountable_type_t::create: 
                    on_accountable_created(msg);
				break;
				case messenger::accountable_type_t::destroy: 
                    on_accountable_destroyed(msg);
				break;
			}
		}

	};

	class thread_accounter_service_t : public service_base_t
	{

	public:

		thread_accounter_service_t(std::unique_ptr<settings_t<thread_accounter_t>> settings, std::shared_ptr<logger_t> logger, std::shared_ptr<thread_accounter_actual_t> thread_accounter_actual, std::shared_ptr<messenger_threads_snapshot_updated_t> messenger_threads_snapshot_updated) : settings(std::move(settings)), logger(logger), thread_accounter_actual(thread_accounter_actual), messenger_threads_snapshot_updated(messenger_threads_snapshot_updated)
		{
			logger->log_global_msg(threads, "Thread accounter service has launched"_u);
		}

		~thread_accounter_service_t()
		{
			logger->log_global_msg(threads, "Thread accounter service has concluded"_u);
		}

		ustring_t get_name() const final
		{
			return "Thread accounter service"_u;
		}

		task_base_t::result_t run() final
		{
			thread_accounter_t::threads_info_t snapshot = thread_accounter_actual->get_snapshot();
			{
				std::lock_guard<std::mutex> guard(mutex);
				std::swap(snapshot, this->snapshot);
			}
			messenger_threads_snapshot_updated->write(std::make_shared<messenger::msg_threads_snapshot_updated_t>());

			if(flags.is_flag(flag_t::requested_shutdown))
				return task_base_t::result_t::completed;
			return task_base_t::result_t::running;
		}

		void on_end_requested() final
		{
			flags.set_flag(flag_t::requested_shutdown, true);
		}
		
		std::chrono::duration<double> get_sleep_after() const final
		{
			return settings->get()->snapshot_refresh_interval();
		}
		
		thread_accounter_t::threads_info_t get_snapshot()
		{
			std::lock_guard<std::mutex> guard(mutex);
			return snapshot;
		}

	private:

		enum class flag_t
		{
			requested_shutdown,
			count
		};

		flags_t<flag_t> flags;

		std::mutex mutex;
		std::unique_ptr<settings_t<thread_accounter_t>> settings;
		std::shared_ptr<logger_t> logger;
		std::shared_ptr<thread_accounter_actual_t> thread_accounter_actual;
		std::shared_ptr<messenger_threads_snapshot_updated_t> messenger_threads_snapshot_updated;
		thread_accounter_t::threads_info_t snapshot;

	};
	
	class thread_accounter_real_t : public thread_accounter_t
	{

	public:

		thread_accounter_real_t(std::shared_ptr<logger_t> logger, std::unique_ptr<service_t<thread_accounter_service_t>> service) : logger(logger), service(std::move(service))
		{
			auto task_id = logger->log_global_task_start(threads, "Initializing accounter [thread]"_u);
			this->service->start();
			logger->log_global_task_done(task_id);
		}

		~thread_accounter_real_t()
		{
			auto task_id = logger->log_global_task_start(threads, "Accounter [thread] is being disposed"_u);
			this->service->end();
			logger->log_global_task_done(task_id);
		}

		threads_info_t get_snapshot() final
		{
			return this->service->get_object()->get_snapshot();
		}

	private:

		std::shared_ptr<logger_t> logger;
		std::unique_ptr<service_t<thread_accounter_service_t>> service;

	};
}

#include "global/component/thread_accounter/provider/generic.hpp"
#include "global/component/thread_accounter/provider/windows.hpp"

#endif