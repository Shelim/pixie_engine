#ifndef ENGINE_COMPONENT_APP_ACCOUNTER_REAL_HPP
#define ENGINE_COMPONENT_APP_ACCOUNTER_REAL_HPP
#pragma once

#include "global/component/app_accounter.hpp"
#include "global/core/messenger.hpp"
#include "global/component/logger.hpp"
#include "global/core/process/service.hpp"

namespace engine
{
	class app_accounter_provider_base_t
	{

	public:

		virtual ~app_accounter_provider_base_t()
		{
			
		}
		
		virtual void app_created(std::shared_ptr<app_meta_t> app_meta) = 0;
		virtual void app_destroyed(std::shared_ptr<app_meta_t> app_meta) = 0;

	};

	REGISTER_PROVIDER_BASE_TYPE(app_accounter_t, app_accounter_provider_base_t)

	class app_accounter_real_t : public app_accounter_t
	{

	public:
		
		app_accounter_real_t(std::shared_ptr<logger_t> logger, std::shared_ptr<messenger_accountable_app_t> messenger_accountable_app, std::unique_ptr<holder_t<app_accounter_t> > app_accounter_provider) : logger(logger), messenger_accountable_app(messenger_accountable_app), app_accounter_provider(std::move(app_accounter_provider))
		{
			auto task_id = logger->log_global_task_start(apps, "Initializing accounter [app]"_u);
			messenger_instance = messenger_accountable_app->register_callback([this](messenger::msg_accountable_app_t* msg){on_accountable_changed(msg);}, messenger::history_t::dump_if_available);
			logger->log_global_task_done(task_id);
		}

		~app_accounter_real_t()
		{
			messenger_instance.reset();
			{
				std::lock_guard<std::mutex> guard(mutex);
				auto task_id = logger->log_global_task_start(apps, "Accounter [app] is being disposed"_u);
				if(apps_collection.size() == 0)
					logger->log_global_msg(apps, "All apps have been accounted for. Clean exit"_u);
				else
				{
					if(apps_collection.size() == 1)
						logger->log_global_msg(apps, "#1# app is still not accounted for. Will release it after accounter has died. The name of it follows:"_u, apps_collection.size());
					else
						logger->log_global_msg(apps, "#1# apps are still not accounted for. Will release them after accounter has died. List of them follows:"_u, apps_collection.size());
					for(auto & app_meta : apps_collection)
						logger->log_global_msg(apps, "App #1#:#2#; Total time: #3# is not yet accounted for"_u, app_meta->get_app(), app_meta->get_instance_id(), app_meta->get_total_time());
				}
				logger->log_global_task_done(task_id);	
			}
		}

		apps_meta_t get_running_apps() final
		{
			std::lock_guard<std::mutex> guard(mutex);
			return apps_collection;
		}

	private:
		std::mutex mutex;

		std::shared_ptr<logger_t> logger;
		std::shared_ptr<messenger_accountable_app_t> messenger_accountable_app;
		std::unique_ptr<holder_t<app_accounter_t> > app_accounter_provider;
		std::unique_ptr<messenger::instance_t<messenger::msg_accountable_app_t> > messenger_instance;
		apps_meta_t apps_collection;
		
		void on_accountable_created(messenger::msg_accountable_app_t * msg)
		{
			std::lock_guard<std::mutex> guard(mutex);

			logger->log_global_msg(apps, "New app was just created: #1#:#2#"_u, msg->get_object()->get_meta()->get_app(), msg->get_object()->get_meta()->get_instance_id());
			
			for(auto & app_meta : apps_collection)
			{
				if(app_meta == msg->get_object()->get_meta() &&
					app_meta->get_instance_id() == msg->get_object()->get_meta()->get_instance_id())
					{
						logger->log_global_warn(apps, "The app #1#:#2# attempted to account itself second time (original total time: #3#)"_u, msg->get_object()->get_meta()->get_app(), msg->get_object()->get_meta()->get_instance_id(), app_meta->get_total_time());
					}
			}

			apps_collection.push_back(msg->get_object()->get_meta());
			for(std::size_t i = 0; i < app_accounter_provider->get_providers_count(); i++)
			{
				app_accounter_provider->get_provider(i)->app_created(msg->get_object()->get_meta());
			}
		}

        void on_accountable_destroyed(messenger::msg_accountable_app_t * msg)
		{
			std::lock_guard<std::mutex> guard(mutex);

			logger->log_global_msg(apps, "App #1#:#2#; Total time: #3# was just destroyed"_u, msg->get_object()->get_meta()->get_app(), msg->get_object()->get_meta()->get_instance_id(), msg->get_object()->get_meta()->get_total_time());
			
				
			for(auto iter = apps_collection.begin(); iter != apps_collection.end(); iter++)
			{
				if((*iter) == msg->get_object()->get_meta() &&
					(*iter)->get_instance_id() == msg->get_object()->get_meta()->get_instance_id())
				{
					apps_collection.erase(iter);

					for(std::size_t i = 0; i < app_accounter_provider->get_providers_count(); i++)
					{
						app_accounter_provider->get_provider(i)->app_destroyed(msg->get_object()->get_meta());
					}

					return;
				}
			}
			
			logger->log_global_warn(apps, "The app #1#:#2#; Total time: #3# attempted to free itself, while it was not registered in the first place"_u, msg->get_object()->get_meta()->get_app(), msg->get_object()->get_meta()->get_instance_id(), msg->get_object()->get_meta()->get_total_time());
		}
		
		void on_accountable_changed(messenger::msg_accountable_app_t* msg)
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

#include "global/component/app_accounter/provider/console.hpp"

#endif