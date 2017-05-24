#ifndef ENGINE_COMMON_CONFIG_IO_PROVIDER_BASE_HPP
#define ENGINE_COMMON_CONFIG_IO_PROVIDER_BASE_HPP
#pragma once

#include <cstddef>
#include "common/config.hpp"

namespace engine
{

	namespace config_io
	{

		class provider_base_t
		{

		public:

			virtual ~provider_base_t()
			{

			}

			virtual void init_update()
			{

			}

			virtual bool is_updated() = 0;
			virtual void reload() = 0;
			virtual void resave() = 0;

		protected:

			provider_base_t(std::shared_ptr<config_t> config) : config(config)
			{

			}

			std::shared_ptr<config_t> get_config()
			{
				return config;
			}

		private:

			std::shared_ptr<config_t> config;

		};

	}

}

#endif