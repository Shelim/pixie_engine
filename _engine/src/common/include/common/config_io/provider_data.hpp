#ifndef ENGINE_COMMON_CONFIG_IO_PROVIDER_DATA_HPP
#define ENGINE_COMMON_CONFIG_IO_PROVIDER_DATA_HPP
#pragma once

#include "common/config.hpp"
#include "common/config_io/provider_base.hpp"
#include "common/data/database_providers.hpp"
#include "common/data/database_items.hpp"
#include "common/xml_pixie_archive.hpp"
#include <mutex>
#include <thread>
#include <future>

namespace engine
{
	namespace data
	{
		class database_providers_t;
	}

	namespace config_io
	{

		class provider_data_t : public provider_base_t
		{

		public:

			provider_data_t( std::shared_ptr<config_t> config) : provider_base_t(config)
			{

			}

			~provider_data_t()
			{

			}
			
			void init_update() final
			{

			}

			bool is_updated() final
			{
				return false;
			}

			void reload() final
			{

			}

			void resave() final
			{

			}

		private:

		};

	}

}

#endif