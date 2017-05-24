#ifndef ENGINE_COMMON_LOGGER_OUTPUT_PROVIDER_CONSOLE_HPP
#define ENGINE_COMMON_LOGGER_OUTPUT_PROVIDER_CONSOLE_HPP
#pragma once

#include "common/logger.hpp"
#include "common/logger_output/provider_base.hpp"

namespace engine
{

	namespace logger_output
	{

		class provider_console_t final : public provider_base_t
		{

		public:

			provider_console_t(std::shared_ptr<platform_t> platform, std::shared_ptr<manifest_app_t> manifest, std::shared_ptr<logger_t> logger) :  provider_base_t(logger), platform(platform), manifest(manifest), is_console_opened(false)
			{

			}

			~provider_console_t()
			{

			}

			void force_resave() final
			{
				// Do nothing!
			}

		private:

			void on_item_changed_local(const std::size_t item_changed) final
			{
				platform_output(item_changed);
			}

			void enable_local() final
			{
				open_console();
			}

			void disable_local() final
			{
				close_console();
			}

			bool is_console_opened;

			void open_console()
			{
				if (!is_console_opened)
				{
					platform_open_console();
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

			std::shared_ptr<platform_t> platform;
			std::shared_ptr<manifest_app_t> manifest;

			void platform_open_console();
			void platform_close_console();
			void platform_output(const std::size_t item_changed);

		};

	}

}

#endif