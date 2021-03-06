#ifndef ENGINE_COMPONENT_CONFIG_MONITOR_REAL_HPP
#define ENGINE_COMPONENT_CONFIG_MONITOR_REAL_HPP
#pragma once

#include "global/component/config.hpp"
#include "global/component/logger.hpp"
#include "global/component/config_monitor.hpp"
#include "global/core/messenger.hpp"
#include "global/core/process/service.hpp"
#include "global/component/profiler.hpp"
#include "utility/pattern/factory.hpp"
#include "utility/container/concurrent_queue.hpp"

namespace engine
{
	class config_monitor_actual_t
	{

	public:

		config_monitor_actual_t(std::shared_ptr<config_t> config, std::shared_ptr<messenger_config_t> messenger_config, std::unique_ptr<holder_t<config_changed_t> > config_changed_provider, std::shared_ptr<logger_t> logger, std::shared_ptr<profiler_t> profiler) : 
			config(config), messenger_config(messenger_config), config_changed_provider(std::move(config_changed_provider)), logger(logger), profiler(profiler)
		{
			std::lock_guard<std::mutex> guard(mutex);

			logger->log_global_msg(config, "Config monitor actual component has started"_u);
			auto task_id = logger->log_global_task_start(config, "Running initial scan inside actual config monitor"_u);

#define ENGINE_CONFIG_GLOBAL_IMPL(name, type) set_global_##name(config->get_global_##name());
#define ENGINE_CONFIG_ONLY_FOR_APP_IMPL(name, type, app) set_app_##app##_##name(config->get_app_##app##_##name());
#define ENGINE_CONFIG_LOCAL_IMPL(name, type) for(std::size_t app = 0; app < value_of(app_t::kind_t::count); app++) set_local_##name(static_cast<app_t::kind_t>(app), config->get_local_##name(static_cast<app_t::kind_t>(app)));
#define ENGINE_CONFIG_GLOBAL(...) DEFINE_TYPE_PASS(ENGINE_CONFIG_GLOBAL_IMPL, __VA_ARGS__)
#define ENGINE_CONFIG_ONLY_FOR_APP(...) DEFINE_TYPE_PASS(ENGINE_CONFIG_ONLY_FOR_APP_IMPL, __VA_ARGS__)
#define ENGINE_CONFIG_LOCAL(...) DEFINE_TYPE_PASS(ENGINE_CONFIG_LOCAL_IMPL, __VA_ARGS__)
#include "def/config.def"

			logger->log_global_task_done(task_id);

			messenger_instance = messenger_config->register_callback([this](engine::messenger::msg_config_t* msg){on_config_changed(msg);});
		}

		~config_monitor_actual_t()
		{
			logger->log_global_msg(config, "Config monitor actual component has concluded"_u);
		}

		void rescan()
		{
    		profiler_t::section_t section(profiler, "Rescanning config");
			
#define ENGINE_CONFIG_GLOBAL_IMPL(name, type) if(set_global_##name(config->get_global_##name()) == result_t::changed) notify_on_change(engine::config_global_t::name, to_string(global_##name));
#define ENGINE_CONFIG_ONLY_FOR_APP_IMPL(name, type, app) if(set_app_##app##_##name(config->get_app_##app##_##name()) == result_t::changed) notify_on_change(engine::config_app_specific_t::app##_##name, to_string(app_##app##_##name));
#define ENGINE_CONFIG_LOCAL_IMPL(name, type) for(std::size_t app = 0; app < value_of(app_t::kind_t::count); app++) { if(set_local_##name(static_cast<app_t::kind_t>(app), config->get_local_##name(static_cast<app_t::kind_t>(app))) == result_t::changed) notify_on_change(static_cast<app_t::kind_t>(app), engine::config_local_t::name, to_string(local_##name[app])); }
#define ENGINE_CONFIG_GLOBAL(...) DEFINE_TYPE_PASS(ENGINE_CONFIG_GLOBAL_IMPL, __VA_ARGS__)
#define ENGINE_CONFIG_ONLY_FOR_APP(...) DEFINE_TYPE_PASS(ENGINE_CONFIG_ONLY_FOR_APP_IMPL, __VA_ARGS__)
#define ENGINE_CONFIG_LOCAL(...) DEFINE_TYPE_PASS(ENGINE_CONFIG_LOCAL_IMPL, __VA_ARGS__)
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
#define ENGINE_CONFIG_GLOBAL_IMPL(name, type) case engine::config_global_t::name: set_global_##name(config->get_global_##name()); break;
#define ENGINE_CONFIG_GLOBAL(...) DEFINE_TYPE_PASS(ENGINE_CONFIG_GLOBAL_IMPL, __VA_ARGS__)
#include "def/config.def"
					}
				break;
				case engine::messenger::msg_config_t::type_t::app_specific:
					switch(msg->get_app_specific()->app_specific)
					{
#define ENGINE_CONFIG_ONLY_FOR_APP_IMPL(name, type, app) case engine::config_app_specific_t::app##_##name: set_app_##app##_##name(config->get_app_##app##_##name()); break;
#define ENGINE_CONFIG_ONLY_FOR_APP(...) DEFINE_TYPE_PASS(ENGINE_CONFIG_ONLY_FOR_APP_IMPL, __VA_ARGS__)
#include "def/config.def"
					}
				break;
				case engine::messenger::msg_config_t::type_t::local:
					switch(msg->get_local()->local)
					{
#define ENGINE_CONFIG_LOCAL_IMPL(name, type) case engine::config_local_t::name: set_local_##name(msg->get_local()->app, config->get_local_##name(msg->get_local()->app)); break;
#define ENGINE_CONFIG_LOCAL(...) DEFINE_TYPE_PASS(ENGINE_CONFIG_LOCAL_IMPL, __VA_ARGS__)
#include "def/config.def"
					}
				break;
			}
		}

		std::mutex mutex;

#define ENGINE_CONFIG_GLOBAL_IMPL(name, type) type global_##name;
#define ENGINE_CONFIG_ONLY_FOR_APP_IMPL(name, type, app) type app_##app##_##name;
#define ENGINE_CONFIG_LOCAL_IMPL(name, type) type local_##name[value_of(app_t::kind_t::count)];
#define ENGINE_CONFIG_GLOBAL(...) DEFINE_TYPE_PASS(ENGINE_CONFIG_GLOBAL_IMPL, __VA_ARGS__)
#define ENGINE_CONFIG_ONLY_FOR_APP(...) DEFINE_TYPE_PASS(ENGINE_CONFIG_ONLY_FOR_APP_IMPL, __VA_ARGS__)
#define ENGINE_CONFIG_LOCAL(...) DEFINE_TYPE_PASS(ENGINE_CONFIG_LOCAL_IMPL, __VA_ARGS__)
#include "def/config.def"

		enum class result_t
		{
			no_change,
			changed
		};

#define ENGINE_CONFIG_GLOBAL_IMPL(name, type) \
		result_t set_global_##name(type val) \
		{ result_t ret = result_t::no_change; \
		if(global_##name != val) ret = result_t::changed; \
		global_##name = val; \
		return ret; }
#define ENGINE_CONFIG_GLOBAL(...) DEFINE_TYPE_PASS(ENGINE_CONFIG_GLOBAL_IMPL, __VA_ARGS__)
		
#define ENGINE_CONFIG_ONLY_FOR_APP_IMPL(name, type, app) \
		result_t set_app_##app##_##name(type val) \
		{ result_t ret = result_t::no_change; \
		if(app_##app##_##name != val) ret = result_t::changed; \
		app_##app##_##name = val; \
		return ret; }
#define ENGINE_CONFIG_ONLY_FOR_APP(...) DEFINE_TYPE_PASS(ENGINE_CONFIG_ONLY_FOR_APP_IMPL, __VA_ARGS__)

#define ENGINE_CONFIG_LOCAL_IMPL(name, type) \
		result_t set_local_##name(app_t::kind_t app, type val) \
		{ result_t ret = result_t::no_change; \
		if(local_##name[value_of(app)] != val) ret = result_t::changed; \
			local_##name[value_of(app)] = val; \
		return ret; }
#define ENGINE_CONFIG_LOCAL(...) DEFINE_TYPE_PASS(ENGINE_CONFIG_LOCAL_IMPL, __VA_ARGS__)
#include "def/config.def"

		std::shared_ptr<config_t> config;
		std::shared_ptr<messenger_config_t> messenger_config;
		std::unique_ptr<messenger::instance_t<messenger::msg_config_t> > messenger_instance;
		std::unique_ptr<holder_t<config_changed_t> > config_changed_provider;
		std::shared_ptr<logger_t> logger;
		std::shared_ptr<profiler_t> profiler;

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
			logger->log_global_msg(config, "Config monitor service has started"_u);
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