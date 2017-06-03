#ifndef ENGINE_COMMON_LOGGER_OUTPUT_PROVIDER_TEXT_STDOUT_HPP
#define ENGINE_COMMON_LOGGER_OUTPUT_PROVIDER_TEXT_STDOUT_HPP
#pragma once

#include "common/logger.hpp"
#include "common/config.hpp"
#include "common/logger_output/provider_text_base.hpp"

namespace engine
{

	namespace logger_output
	{

		class provider_stdout_t final : public provider_text_base_t
		{

		public:

			provider_stdout_t(std::shared_ptr<logger_t> logger, std::shared_ptr<platform_t> platform) : provider_text_base_t(logger, provider_text_base_t::item_output_format_t::reduced), platform(platform)
			{

			}

			~provider_stdout_t()
			{

			}

			void output_line(richtext_t line) final
			{
				auto & parts = line.get();

				for (auto & part : parts)
				{
					platform_color_output(part);
					fputs(part.get_text().get_cstring(), stdout);
				}
				puts("");
			}

		private:

			void platform_color_output(const richtext_t::part_t & part);
			
			std::shared_ptr<platform_t> platform;

		};

	}

}

#endif