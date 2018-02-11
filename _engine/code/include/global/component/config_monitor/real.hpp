#ifndef ENGINE_COMPONENT_CONFIG_MONITOR_REAL_HPP
#define ENGINE_COMPONENT_CONFIG_MONITOR_REAL_HPP
#pragma once

#include "global/component/config.hpp"
#include "global/component/logger.hpp"
#include "global/component/config_monitor.hpp"
#include "global/core/messenger/messenger.hpp"
#include "global/core/process/service.hpp"
#include "utility/pattern/factory.hpp"
#include "utility/container/concurrent_queue.hpp"

namespace engine
{
	class config_monitor_actual_t
	{

	public:

		config_monitor_actual_t(std::shared_ptr<config_t> config, std::shared_ptr<messenger_config_t> messenger_config, std::unique_ptr<holder_t<config_changed_t> > config_changed_provider, std::shared_ptr<logger_t> logger) : 
			config(config), messenger_config(messenger_config), config_changed_provider(std::move(config_changed_provider)), logger(logger)
		{
			std::lock_guard<std::mutex> guard(mutex);

			logger->log_global_msg(config, "Config monitor actual component has started"_u);
			auto task_id = logger->log_global_task_start(config, "Running initial scan inside actual config monitor"_u);

#define ENGINE_CONFIG_GLOBAL(type, name) set_global_##name(config->get_global_##name());
#define ENGINE_CONFIG_ONLY_FOR_APP(type, app, name) set_app_##app##_##name(config->get_app_##app##_##name());
#define ENGINE_CONFIG_LOCAL(type, name) for(std::size_t app = 0; app < value_of(app_t::kind_t::count); app++) set_local_##name(static_cast<app_t::kind_t>(app), config->get_local_##name(static_cast<app_t::kind_t>(app)));
#include "def/config.def"

			logger->log_global_task_done(task_id);

			messenger_instance = messenger_config->register_callback([this](engine::messenger::msg_config_t* msg){on_config_changed(msg);});
		}

		void rescan()
		{
#define ENGINE_CONFIG_GLOBAL(type, name) if(set_global_##name(config->get_global_##name()) == result_t::changed) notify_on_change(engine::config_global_t::name, to_string(global_##name));
#define ENGINE_CONFIG_ONLY_FOR_APP(type, app, name) if(set_app_##app##_##name(config->get_app_##app##_##name()) == result_t::changed) notify_on_change(engine::config_app_specific_t::##app##_##name, to_string(app_##app##_##name));
#define ENGINE_CONFIG_LOCAL(type, name) for(std::size_t app = 0; app < value_of(app_t::kind_t::count); app++) { if(set_local_##name(static_cast<app_t::kind_t>(app), config->get_local_##name(static_cast<app_t::kind_t>(app))) == result_t::changed) notify_on_change(static_cast<app_t::kind_t>(app), engine::config_local_t::name, to_string(local_##name[app])); }
#include "def/config.def"
		}

	private:

		void notify_on_change(engine::config_global_t global, ustring_t val)
		{
			for(int i = 0; i < config_changed_provider->get_providers_count(); i++)
			{
				config_changed_provider->get_provider(i)->notify_on_change(global, val, messenger::msg_config_t::source_t::external);
			}
		}
		void notify_on_change(engine::config_app_specific_t app_specific, ustring_t val)
		{
			for(int i = 0; i < config_changed_provider->get_providers_count(); i++)
			{
				config_changed_provider->get_provider(i)->notify_on_change(app_specific, val, messenger::msg_config_t::source_t::external);
			}
		}
		void notify_on_change(engine::app_t::kind_t app, engine::config_local_t local, ustring_t val)
		{
			for(int i = 0; i < config_changed_provider->get_providers_count(); i++)
			{
				config_changed_provider->get_provider(i)->notify_on_change(app, local, val, messenger::msg_config_t::source_t::external);
			}
		}

		void on_config_changed(engine::messenger::msg_config_t* msg)
		{
			std::lock_guard<std::mutex> guard(mutex);
			switch(msg->get_type())
			{
				case engine::messenger::msg_config_t::type_t::global:
					switch(msg->get_global()->global)
					{
#define ENGINE_CONFIG_GLOBAL(type, name) case engine::config_global_t::name: set_global_##name(config->get_global_##name()); break;
#include "def/config.def"
					}
				break;
				case engine::messenger::msg_config_t::type_t::app_specific:
					switch(msg->get_app_specific()->app_specific)
					{
#define ENGINE_CONFIG_ONLY_FOR_APP(type, app, name) case engine::config_app_specific_t::##app##_##name: set_app_##app##_##name(config->get_app_##app##_##name()); break;
#include "def/config.def"
					}
				break;
				case engine::messenger::msg_config_t::type_t::local:
					switch(msg->get_local()->local)
					{
#define ENGINE_CONFIG_LOCAL(type, name) case engine::config_local_t::name: set_local_##name(msg->get_local()->app, config->get_local_##name(msg->get_local()->app)); break;
#include "def/config.def"
					}
				break;
			}
		}

		std::mutex mutex;

#define ENGINE_CONFIG_GLOBAL(type, name) type global_##name;
#define ENGINE_CONFIG_ONLY_FOR_APP(type, app, name) type app_##app##_##name;
#define ENGINE_CONFIG_LOCAL(type, name) type local_##name[value_of(app_t::kind_t::count)];
#include "def/config.def"

		enum class result_t
		{
			no_change,
			changed
		};

#define ENGINE_CONFIG_GLOBAL(type, name) \
		result_t set_global_##name(type val) \
		{ result_t ret = result_t::no_change; \
		if(global_##name != val) ret = result_t::changed; \
		global_##name = val; \
		return ret; }
		
#define ENGINE_CONFIG_ONLY_FOR_APP(type, app, name) \
		result_t set_app_##app##_##name(type val) \
		{ result_t ret = result_t::no_change; \
		if(app_##app##_##name != val) ret = result_t::changed; \
		app_##app##_##name = val; \
		return ret; }

#define ENGINE_CONFIG_LOCAL(type, name) \
		result_t set_local_##name(app_t::kind_t app, type val) \
		{ result_t ret = result_t::no_change; \
		if(local_##name[value_of(app)] != val) ret = result_t::changed; \
			local_##name[value_of(app)] = val; \
		return ret; }
#include "def/config.def"

		std::shared_ptr<config_t> config;
		std::shared_ptr<messenger_config_t> messenger_config;
		std::unique_ptr<messenger::instance_t<messenger::msg_config_t> > messenger_instance;
		std::unique_ptr<holder_t<config_changed_t> > config_changed_provider;
		std::shared_ptr<logger_t> logger;

	};

	class config_monitor_executor_t
	{

	public:

		config_monitor_executor_t(std::shared_ptr<config_monitor_actual_t> actual) : actual(actual)
		{

		}

		void request_rescan()
		{
			messages.push(message_t::rescan_requested);
		}

		void terminate_pool()
		{
			messages.push(message_t::terminate_monitor);
		}

		task_base_t::result_t run()
		{
			for(;;)
			{
				message_t msg = messages.pop();
				switch(msg)
				{
					case message_t::rescan_requested: actual->rescan(); break;
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

		concurrent_queue_t<message_t> messages;
		std::shared_ptr<config_monitor_actual_t> actual;

	};


	class config_monitor_service_t : public service_base_t
	{

	public:

		config_monitor_service_t(std::shared_ptr<logger_t> logger, std::shared_ptr<messenger_config_storage_t> messenger_config_storage, std::shared_ptr<config_monitor_executor_t> config_monitor_executor) : logger(logger), messenger_config_storage(messenger_config_storage), config_monitor_executor(config_monitor_executor)
		{
			logger->log_global_msg(config, "Config monitor service has launched"_u);
			messenger_instance = messenger_config_storage->register_callback([this](messenger::msg_config_storage_t* msg)
			{
				if(msg->get_type() == messenger::msg_config_storage_t::type_t::source_updated)
					this->config_monitor_executor->request_rescan();
			});
		}

		~config_monitor_service_t()
		{
			logger->log_global_msg(config, "Config monitor service has concluded"_u);
		}

		ustring_t get_name() const final
		{
			return "Config monitor service"_u;
		}

		task_base_t::result_t run() final
		{
			return config_monitor_executor->run();
		}

		void on_end_requested() final
		{
			config_monitor_executor->terminate_pool();
		}

	private:

		std::shared_ptr<logger_t> logger;
		std::shared_ptr<messenger_config_storage_t> messenger_config_storage;
		std::unique_ptr<messenger::instance_t<messenger::msg_config_storage_t> > messenger_instance;
		std::shared_ptr<config_monitor_executor_t> config_monitor_executor;

	};

	class config_monitor_real_t : public config_monitor_t
	{

	public:

		config_monitor_real_t(std::shared_ptr<logger_t> logger, std::unique_ptr<service_t<config_monitor_service_t>> service) : logger(logger), service(std::move(service))
		{
			auto task_id = logger->log_global_task_start(config, "Initializing monitor [config]"_u);
			this->service->start();
			logger->log_global_task_done(task_id);
		}

		~config_monitor_real_t()
		{
			auto task_id = logger->log_global_task_start(config, "Monitor [config] is being disposed"_u);
			this->service->end();
			logger->log_global_task_done(task_id);
		}

	private:

		std::shared_ptr<logger_t> logger;
		std::unique_ptr<service_t<config_monitor_service_t>> service;

	};
}


#endif