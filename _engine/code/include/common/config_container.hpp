#ifndef ENGINE_COMMON_CONFIG_CONTAINER_HPP
#define ENGINE_COMMON_CONFIG_CONTAINER_HPP
#pragma once

#include "common/logger.hpp"
#include "common/config.hpp"
#include "common/config_io/provider_base.hpp"
#include "common/config_io/provider_data.hpp"

namespace engine
{
	class config_t;

	class config_container_t
	{

	public:

		config_container_t(std::shared_ptr<config_t> config, std::shared_ptr<engine::config_io::provider_base_t> provider) : config(config), provider(provider)
		{
			config->set_provider(provider);
		}

		~config_container_t()
		{

		}

		std::shared_ptr<config_t> get()
		{
			return config;
		}

		void init_update()
		{
			provider->init_update();
			if (provider->is_updated())
			{
				provider->reload();
			}
		}

	private:

		std::shared_ptr<config_t> config;
		std::shared_ptr<engine::config_io::provider_base_t> provider;

	};
}

#endif