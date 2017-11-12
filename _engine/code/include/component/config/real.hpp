#ifndef ENGINE_COMPONENT_CONFIG_REAL_HPP
#define ENGINE_COMPONENT_CONFIG_REAL_HPP
#pragma once

#include "component/config.hpp"
#include "component/logger.hpp"
#include "utility/pattern/provider.hpp"
#include "utility/messenger/messenger.hpp"
#include "component/config/msg_config_updated.hpp"
#include "utility/pattern/flags.hpp"
#include "utility/pattern/enum.hpp"

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

#define ENGINE_CONFIG_GLOBAL_STD(type, name) virtual type get_global_##name() const = 0;  virtual set_result_t set_global_##name(const type & val) = 0;
#define ENGINE_CONFIG_LOCAL_STD(type, app, name) virtual type get_app_##app##_##name() const = 0;  virtual set_result_t set_app_##app##_##name(const type & val) = 0;
#define ENGINE_CONFIG_STD(type, name) virtual type get_cfg_##name(manifest_app_t::app_t app) const = 0;  virtual set_result_t set_cfg_##name(manifest_app_t::app_t app, const type & val) = 0;
#include "std/config_std.hpp"

		class provider_default_t
		{

		public:

#define ENGINE_CONFIG_GLOBAL_STD(type, name) type get_default_global_##name() const; 
#define ENGINE_CONFIG_LOCAL_STD(type, app, name) type get_default_local_##app##_##name() const;
#define ENGINE_CONFIG_STD(type, name) type get_default_cfg_##name(manifest_app_t::app_t app) const;
#include "std/config_std.hpp"

			provider_default_t(std::unique_ptr<settings_t<config_t>> configuration) : configuration(std::move(configuration))
			{

			}

		private:


			std::unique_ptr<settings_t<config_t>> configuration;
		};

	protected:



	private:


	};

	REGISTER_PROVIDER_BASE_TYPE(config_t, config_provider_base_t)

	class config_real_t : public config_t
	{

	public:

		config_real_t(std::shared_ptr<manifest_app_t> manifest_app, std::shared_ptr<messenger_t> messenger, std::unique_ptr<holder_t<config_t> > config_provider, std::shared_ptr<logger_t> logger);

		~config_real_t();

#define ENGINE_CONFIG_GLOBAL_STD(type, name) type get_global_##name() const final { return config_provider->get_provider()->get_global_##name(); }; void set_global_##name(type val) final { if(config_provider->get_provider()->set_global_##name(val) == config_provider_base_t::set_result_t::success) notify_on_change(item_t::global_##name); }
#define ENGINE_CONFIG_LOCAL_STD(type, app, name) type get_app_##app##_##name() const final { return config_provider->get_provider()->get_app_##app##_##name(); }; void set_app_##app##_##name(type val) final { if(config_provider->get_provider()->set_app_##app##_##name(val) == config_provider_base_t::set_result_t::success) notify_on_change(item_t::app_##app##_##name); }
#define ENGINE_CONFIG_STD(type, name) type get_cfg_##name(manifest_app_t::app_t app) const final { return config_provider->get_provider()->get_cfg_##name(app); }; void set_cfg_##name(manifest_app_t::app_t app, type val) final { if(config_provider->get_provider()->set_cfg_##name(app, val) == config_provider_base_t::set_result_t::success) notify_on_change(item_t::cfg_##name); }
#include "std/config_std.hpp"

	private:

		void on_change_from_provider(msg_base_t * msg);

		void notify_on_change(item_t item);
		void notify_on_initial_value(item_t item);
		
		std::shared_ptr<messenger_t> messenger;
		std::unique_ptr<holder_t<config_t> > config_provider;
		std::shared_ptr<logger_t> logger;

		callback_container_t callbacks_container;

	};
}

#include "component/config/provider/storage.hpp"

#endif