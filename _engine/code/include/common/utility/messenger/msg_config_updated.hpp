#ifndef ENGINE_COMMON_UTILITY_MESSENGER_MSG_CONFIG_UPDATED_HPP
#define ENGINE_COMMON_UTILITY_MESSENGER_MSG_CONFIG_UPDATED_HPP
#pragma once

#include <cstdint>
#include "utility/messenger/msg_base.hpp"
#include "component/config.hpp"

namespace engine
{

	class msg_config_provider_updated_t : public msg_base_t
	{

	public:

		static const uint32_t type = 'cfgp';

		msg_config_provider_updated_t(config_t::item_t item) : msg_base_t(type), item(item)
		{

		}

		config_t::item_t get_item() const
		{
			return item;
		}

	private:

		config_t::item_t item;

	};

	class msg_config_updated_t : public msg_base_t
	{

	public:

		static const uint32_t type = 'cfgu';

		msg_config_updated_t(config_t::item_t item, config_t * config) : msg_base_t(type), item(item), config(config)
		{

		}

		config_t::item_t get_item() const
		{
			return item;
		}

		config_t * get_config() const
		{
			return config;
		}

	private:
		
		config_t::item_t item;
		config_t * config;

	};

}

#endif