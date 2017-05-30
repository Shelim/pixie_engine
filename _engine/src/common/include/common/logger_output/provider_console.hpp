#ifndef ENGINE_COMMON_LOGGER_OUTPUT_PROVIDER_CONSOLE_HPP
#define ENGINE_COMMON_LOGGER_OUTPUT_PROVIDER_CONSOLE_HPP
#pragma once

#include "common/logger.hpp"
#include "common/config.hpp"
#include "common/logger_output/provider_base.hpp"

namespace engine
{

	namespace logger_output
	{

		class provider_console_t final : public provider_base_t
		{

		public:

			provider_console_t(std::shared_ptr<config_t> config, std::shared_ptr<platform_t> platform, std::shared_ptr<manifest_app_t> manifest, std::shared_ptr<logger_t> logger, std::shared_ptr<environment_info_t> environment_info) : provider_base_t(logger), config(config), platform(platform), manifest(manifest), environment_info(environment_info), is_console_opened(false)
			{
				open_console(); // ToDo, remove me!
			}

			~provider_console_t()
			{

			}

			void update() final
			{
				if (config->get<bool>(config_t::item_t::game_has_console))
					open_console();
				else
					close_console();
			}

		private:

			bool is_console_opened;

			void open_console()
			{
				if (!is_console_opened)
				{
					platform_open_console();

					output_environment_info(environment_info);

					auto items = get_logger()->get_items();
					for (std::size_t i = 0; i < items.size(); i++)
					{
						process_item(items[i]);
					}

					is_console_opened = true;
				}
			}

			void close_console()
			{
				if (is_console_opened)
				{
					platform_close_console();
					is_console_opened = false;
				}
			}

			void color_next_output(output_color_t color, output_color_t background = output_color_t::background_default) final;

			void output_string(const ustring_t & str) final
			{
				printf(str.get_cstring());
			}

			std::shared_ptr<config_t> config;
			std::shared_ptr<platform_t> platform;
			std::shared_ptr<manifest_app_t> manifest;
			std::shared_ptr<environment_info_t> environment_info;

			void platform_open_console();
			void platform_close_console();
			void platform_output(const logger_t::item_t & item);

		};

	}

}

#endif