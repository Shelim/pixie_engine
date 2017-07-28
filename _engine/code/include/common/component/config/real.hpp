#ifndef ENGINE_COMPONENT_CONFIG_REAL_HPP
#define ENGINE_COMPONENT_CONFIG_REAL_HPP
#pragma once

#include "component/config.hpp"
#include "component/logger.hpp"
#include "utility/pattern/provider.hpp"
#include "utility/messenger/messenger.hpp"
#include "utility/messenger/msg_config_updated.hpp"
#include "utility/pattern/flags.hpp"

namespace engine
{
	class config_provider_base_t
	{

	public:

		virtual ~config_provider_base_t()
		{

		}

#define GAME_CONFIG_STD(type, name) virtual const type & get_##name() const = 0;
#include "std/config_std.hpp"
#define GAME_CONFIG_STD(type, name) virtual bool set_##name(const type & val) = 0;
#include "std/config_std.hpp"

	protected:

#define GAME_CONFIG_STD(type, name) const type & get_default_##name() const;
#include "std/config_std.hpp"

		config_provider_base_t(std::unique_ptr<settings_t<config_t>> configuration) : configuration(std::move(configuration))
		{

		}

	private:

		std::unique_ptr<settings_t<config_t>> configuration;

	};

	REGISTER_PROVIDER_BASE_TYPE(config_t, config_provider_base_t)

	class config_real_t : public config_t
	{

	public:

		config_real_t(std::shared_ptr<messenger_t> messenger, std::unique_ptr<holder_t<config_t> > config_provider, std::shared_ptr<logger_t> logger);

		~config_real_t();

#define GAME_CONFIG_STD(type, name) const type & get_##name() const final { return config_provider->get_provider()->get_##name(); }; void set_##name(const type & val) final { if(config_provider->get_provider()->set_##name(val)) notify_on_change(item_t::name); }
#include "std/config_std.hpp"

	private:

		void on_change_from_provider(msg_base_t * msg);

		void notify_on_change(item_t item);
		void notify_on_initial_value(item_t item);
		
		std::shared_ptr<messenger_t> messenger;
		std::unique_ptr<holder_t<config_t> > config_provider;
		std::shared_ptr<logger_t> logger;

	};
}

#include "component/config/provider/data.hpp"
#include "component/config/provider/storage.hpp"

#endif