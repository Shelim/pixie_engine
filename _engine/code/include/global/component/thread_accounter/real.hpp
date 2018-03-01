#ifndef ENGINE_COMPONENT_THREAD_ACCOUNTER_REAL_HPP
#define ENGINE_COMPONENT_THREAD_ACCOUNTER_REAL_HPP
#pragma once

#include "global/component/thread_accounter.hpp"
#include "global/core/messenger.hpp"
#include "global/component/logger.hpp"
#include "global/core/process/service.hpp"

namespace engine
{
	
	class thread_accounter_provider_base_t
	{

	public:

		virtual ~thread_accounter_provider_base_t()
		{
			
		}
		
		virtual void thread_created(std::shared_ptr<thread_meta_t> thread_meta) = 0;
		virtual void thread_destroyed(std::shared_ptr<thread_meta_t> thread_meta) = 0;

	};

	REGISTER_PROVIDER_BASE_TYPE(thread_accounter_t, thread_accounter_provider_base_t)
	
	class thread_accounter_real_t : public thread_accounter_t
	{

	public:

		thread_accounter_real_t(std::shared_ptr<logger_t> logger, std::shared_ptr<messenger_accountable_thread_t> messenger_accountable_thread, std::unique_ptr<holder_t<thread_accounter_t> > thread_accounter_provider) : logger(logger), messenger_accountable_thread(messenger_accountable_thread), thread_accounter_provider(std::move(thread_accounter_provider))
		{
			auto task_id = logger->log_global_task_start(threads, "Initializing accounter [thread]"_u);
			messenger_instance = messenger_accountable_thread->register_callback([this](messenger::msg_accountable_thread_t* msg){on_accountable_changed(msg);}, messenger::history_t::dump_if_available);
			logger->log_global_task_done(task_id);
		}

		~thread_accounter_real_t()
		{
			messenger_instance.reset();
			{
				std::lock_guard<std::mutex> guard(mutex);
				auto task_id = logger->log_global_task_start(threads, "Accounter [thread] is being disposed"_u);
				if(threads_collection.size() == 0)
					logger->log_global_msg(threads, "All threads have been accounted for. Clean exit"_u);
				else
				{
					if(threads_collection.size() == 1)
						logger->log_global_msg(threads, "#1# thread is still not accounted for. Will release it after accounter has died. The name of it follows:"_u, threads_collection.size());
					else
						logger->log_global_msg(threads, "#1# threads are still not accounted for. Will release them after accounter has died. List of them follows:"_u, threads_collection.size());
					for(auto & thread_meta : threads_collection)
					{
						if(thread_meta->are_stats_available())
							logger->log_global_msg(threads, "Thread #1# [id: #2#]; Total time: #3#/#4# (#5#% CPU) in #6####7# is not yet accounted for"_u, thread_meta->get_name(), thread_meta->get_id(), thread_meta->get_total_busy_time(), thread_meta->get_total_time(), static_cast<int>((thread_meta->get_total_busy_time() / thread_meta->get_total_time()) * 100.0), thread_meta->get_app(), thread_meta->get_app_instance_id());
						else
							logger->log_global_msg(threads, "Thread #1# [id: #2#]; Total time: #3# (unknown CPU) in #4####5# is not yet accounted for"_u, thread_meta->get_name(), thread_meta->get_id(), thread_meta->get_total_time(), thread_meta->get_app(), thread_meta->get_app_instance_id());
					}
				}
				logger->log_global_task_done(task_id);	
			}
		}

		threads_meta_t get_running_threads() final
		{
			std::lock_guard<std::mutex> guard(mutex);
			return threads_collection;
		}

	private:
		std::mutex mutex;

		std::shared_ptr<logger_t> logger;
		std::shared_ptr<messenger_accountable_thread_t> messenger_accountable_thread;
		std::unique_ptr<holder_t<thread_accounter_t> > thread_accounter_provider;
		std::unique_ptr<messenger::instance_t<messenger::msg_accountable_thread_t> > messenger_instance;
		threads_meta_t threads_collection;
		
		void on_accountable_created(messenger::msg_accountable_thread_t * msg)
		{
			std::lock_guard<std::mutex> guard(mutex);

			if(msg->get_object()->get_thread_meta()->get_app() == app_t::kind_t::_engine)
				logger->log_global_msg(threads, "New global thread was just created: #1# [id: #2#]"_u, msg->get_object()->get_thread_meta()->get_name(), msg->get_object()->get_thread_meta()->get_id());
			else
				logger->log_global_msg(threads, "New thread was just created for #3####4#: #1# [id: #2#]"_u, msg->get_object()->get_thread_meta()->get_name(), msg->get_object()->get_thread_meta()->get_id(), msg->get_object()->get_thread_meta()->get_app(), msg->get_object()->get_thread_meta()->get_app_instance_id());
			
			for(auto & thread_meta : threads_collection)
			{
				if(thread_meta == msg->get_object()->get_thread_meta() &&
					thread_meta->get_id() == msg->get_object()->get_thread_meta()->get_id())
					{
						logger->log_global_warn(threads, "The thread #1# [id: #2#]; Total time: #3#/#4# (#5#% CPU) attempted to account itself second time (original name: #6# [id: #7#]; Total time: #8#/#9# (#10#% CPU))"_u, msg->get_object()->get_thread_meta()->get_name(), msg->get_object()->get_thread_meta()->get_id(), msg->get_object()->get_thread_meta()->get_total_busy_time(), msg->get_object()->get_thread_meta()->get_total_time(), static_cast<int>((msg->get_object()->get_thread_meta()->get_total_busy_time() / msg->get_object()->get_thread_meta()->get_total_time()) * 100.0), thread_meta->get_name(), thread_meta->get_id(), thread_meta->get_total_busy_time(), thread_meta->get_total_time(), static_cast<int>((thread_meta->get_total_busy_time() / thread_meta->get_total_time()) * 100.0));
					}
			}

			threads_collection.push_back(msg->get_object()->get_thread_meta());
			for(std::size_t i = 0; i < thread_accounter_provider->get_providers_count(); i++)
			{
				thread_accounter_provider->get_provider(i)->thread_created(msg->get_object()->get_thread_meta());
			}
		}

        void on_accountable_destroyed(messenger::msg_accountable_thread_t * msg)
		{
			std::lock_guard<std::mutex> guard(mutex);

			if(msg->get_object()->get_thread_meta()->get_app() == app_t::kind_t::_engine)
			{
				if(msg->get_object()->get_thread_meta()->are_stats_available())
					logger->log_global_msg(threads, "Global thread was just destroyed: #1# [id: #2#]; Total time: #3#/#4# (#5#% CPU)"_u, msg->get_object()->get_thread_meta()->get_name(), msg->get_object()->get_thread_meta()->get_id(), msg->get_object()->get_thread_meta()->get_total_busy_time(), msg->get_object()->get_thread_meta()->get_total_time(), static_cast<int>((msg->get_object()->get_thread_meta()->get_total_busy_time() / msg->get_object()->get_thread_meta()->get_total_time()) * 100.0));
				else
					logger->log_global_msg(threads, "Global thread was just destroyed: #1# [id: #2#]; Total time: #3# (unknown CPU)"_u, msg->get_object()->get_thread_meta()->get_name(), msg->get_object()->get_thread_meta()->get_id(), msg->get_object()->get_thread_meta()->get_total_time());
			}
			else
			{
				if(msg->get_object()->get_thread_meta()->are_stats_available())
					logger->log_global_msg(threads, "Thread was just destroyed for #6####7#: #1# [id: #2#]; Total time: #3#/#4# (#5#% CPU)"_u, msg->get_object()->get_thread_meta()->get_name(), msg->get_object()->get_thread_meta()->get_id(), msg->get_object()->get_thread_meta()->get_total_busy_time(), msg->get_object()->get_thread_meta()->get_total_time(), static_cast<int>((msg->get_object()->get_thread_meta()->get_total_busy_time() / msg->get_object()->get_thread_meta()->get_total_time()) * 100.0), msg->get_object()->get_thread_meta()->get_app(), msg->get_object()->get_thread_meta()->get_app_instance_id());
				else
					logger->log_global_msg(threads, "Thread was just destroyed for #6####7#: #1# [id: #2#]; Total time: #3# (unknown CPU)"_u, msg->get_object()->get_thread_meta()->get_name(), msg->get_object()->get_thread_meta()->get_id(), msg->get_object()->get_thread_meta()->get_total_time(), msg->get_object()->get_thread_meta()->get_app(), msg->get_object()->get_thread_meta()->get_app_instance_id());
			}
				
			for(auto iter = threads_collection.begin(); iter != threads_collection.end(); iter++)
			{
				if((*iter) == msg->get_object()->get_thread_meta() &&
					(*iter)->get_id() == msg->get_object()->get_thread_meta()->get_id())
				{
					threads_collection.erase(iter);

					for(std::size_t i = 0; i < thread_accounter_provider->get_providers_count(); i++)
					{
						thread_accounter_provider->get_provider(i)->thread_destroyed(msg->get_object()->get_thread_meta());
					}

					return;
				}
			}
			
			if(msg->get_object()->get_thread_meta()->are_stats_available())
				logger->log_global_warn(threads, "The thread #1# [id: #2#]; Total time: #3#/#4# (#5#% CPU) attempted to free itself, while it was not registered in the first place"_u, msg->get_object()->get_thread_meta()->get_name(), msg->get_object()->get_thread_meta()->get_id(), msg->get_object()->get_thread_meta()->get_total_busy_time(), msg->get_object()->get_thread_meta()->get_total_time(), static_cast<int>((msg->get_object()->get_thread_meta()->get_total_busy_time() / msg->get_object()->get_thread_meta()->get_total_time()) * 100.0));
			else
				logger->log_global_warn(threads, "The thread #1# [id: #2#]; Total time: #3# (unknown CPU) attempted to free itself, while it was not registered in the first place"_u, msg->get_object()->get_thread_meta()->get_name(), msg->get_object()->get_thread_meta()->get_id(), msg->get_object()->get_thread_meta()->get_total_time());
		}
		
		void on_accountable_changed(messenger::msg_accountable_thread_t* msg)
		{
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
}

#include "global/component/thread_accounter/provider/console.hpp"

#endif