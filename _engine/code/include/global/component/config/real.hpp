#ifndef ENGINE_COMPONENT_CONFIG_REAL_HPP
#define ENGINE_COMPONENT_CONFIG_REAL_HPP
#pragma once

#include "global/component/config.hpp"
#include "global/component/logger.hpp"
#include "utility/pattern/provider.hpp"
#include "global/core/messenger/messenger.hpp"
#include "utility/pattern/flags.hpp"
#include "utility/pattern/enum.hpp"
#include "global/component/config_common/changed_provider.hpp"

namespace engine
{
	class config_provider_base_t
	{

	public:

		virtual ~config_provider_base_t()
		{

		}

		enum class set_result_t
		{
			success,
			no_change
		};
        
#define ENGINE_CONFIG_GLOBAL(type, name) \
		virtual type get_global_##name() const = 0; \
		virtual set_result_t set_global_##name(type val) = 0;

#define ENGINE_CONFIG_ONLY_FOR_APP(type, app, name) \
		virtual type get_app_##app##_##name() const = 0; \
		virtual set_result_t set_app_##app##_##name(type val) = 0;

#define ENGINE_CONFIG_LOCAL(type, name) \
		virtual type get_local_##name(app_t::kind_t app) const = 0; \
		virtual set_result_t set_local_##name(app_t::kind_t app, type val) = 0;
#include "def/config.def"

	};

	class config_changed_t
	{
		
	};

	REGISTER_PROVIDER_BASE_TYPE(config_t, config_provider_base_t)
	REGISTER_PROVIDER_BASE_TYPE(config_changed_t, config_changed_provider_base_t)

	class config_real_t : public config_t
	{

	public:

		config_real_t(std::unique_ptr<holder_t<config_t> > config_provider, std::unique_ptr<holder_t<config_changed_t> > config_changed_provider, std::shared_ptr<logger_t> logger) :
			messenger_config(messenger_config), config_provider(std::move(config_provider)), config_changed_provider(std::move(config_changed_provider)), logger(logger)
		{
			logger->log_global_msg(config, "Config component has started"_u);
		}

		~config_real_t()
		{
			logger->log_global_msg(config, "Config component has concluded"_u);
		}
	
#define ENGINE_CONFIG_GLOBAL(type, name) \
		type get_global_##name() const final { std::lock_guard<std::recursive_mutex> guard(mutex); return config_provider->get_provider()->get_global_##name(); } \
		void set_global_##name(type val) final { std::lock_guard<std::recursive_mutex> guard(mutex); if(config_provider->get_provider()->set_global_##name(val) == config_provider_base_t::set_result_t::success) notify_on_change(config_global_t::name, to_string(val)); }

#define ENGINE_CONFIG_ONLY_FOR_APP(type, app, name) \
		type get_app_##app##_##name() const final { std::lock_guard<std::recursive_mutex> guard(mutex); return config_provider->get_provider()->get_app_##app##_##name(); } \
		void set_app_##app##_##name(type val) final { std::lock_guard<std::recursive_mutex> guard(mutex); if(config_provider->get_provider()->set_app_##app##_##name(val) == config_provider_base_t::set_result_t::success) notify_on_change(config_app_specific_t::app##_##name, to_string(val)); }

#define ENGINE_CONFIG_LOCAL(type, name) \
		type get_local_##name(app_t::kind_t app) const final { std::lock_guard<std::recursive_mutex> guard(mutex); return config_provider->get_provider()->get_local_##name(app); } \
		void set_local_##name(app_t::kind_t app, type val) final { std::lock_guard<std::recursive_mutex> guard(mutex); if(config_provider->get_provider()->set_local_##name(app, val) == config_provider_base_t::set_result_t::success) notify_on_change(app, config_local_t::name, to_string(val)); }
#include "def/config.def"

	private:

		void notify_on_change(engine::config_global_t global, ustring_t val)
		{
			for(int i = 0; i < config_changed_provider->get_providers_count(); i++)
			{
				config_changed_provider->get_provider(i)->notify_on_change(global, val, messenger::msg_config_t::source_t::internal);
			}
		}
		void notify_on_change(engine::config_app_specific_t app_specific, ustring_t val)
		{
			for(int i = 0; i < config_changed_provider->get_providers_count(); i++)
			{
				config_changed_provider->get_provider(i)->notify_on_change(app_specific, val, messenger::msg_config_t::source_t::internal);
			}
		}
		void notify_on_change(engine::app_t::kind_t app, engine::config_local_t local, ustring_t val)
		{
			for(int i = 0; i < config_changed_provider->get_providers_count(); i++)
			{
				config_changed_provider->get_provider(i)->notify_on_change(app, local, val, messenger::msg_config_t::source_t::internal);
			}
		}
		
		std::shared_ptr<messenger_config_t> messenger_config;
		std::unique_ptr<holder_t<config_changed_t> > config_changed_provider;
		std::unique_ptr<holder_t<config_t> > config_provider;
		std::shared_ptr<logger_t> logger;

		mutable std::recursive_mutex mutex;

	};
}

#include "global/component/config/provider/config_storage.hpp"

#endif