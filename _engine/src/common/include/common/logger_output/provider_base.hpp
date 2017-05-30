#ifndef ENGINE_COMMON_LOGGER_OUTPUT_PROVIDER_BASE_HPP
#define ENGINE_COMMON_LOGGER_OUTPUT_PROVIDER_BASE_HPP
#pragma once

#include <cstddef>
#include "common/logger.hpp"
#include "common/environment_info.hpp"

namespace engine
{

	namespace logger_output
	{

		class provider_base_t
		{

		public:

			virtual ~provider_base_t()
			{

			}

			virtual void output_environment_info(std::shared_ptr<environment_info_t> environment_info)
			{
				begin_environment_info();
				process_environment_info(environment_info);
				end_environment_info();
			}

			virtual void begin_environment_info()
			{
				write_section_header(_U("ENVIRONMENT INFO"));
			}

			virtual void process_environment_info(std::shared_ptr<environment_info_t> environment_info)
			{

			}

			virtual void end_environment_info()
			{

			}

			virtual void begin_process_items()
			{

			}

			virtual void process_item(const logger_t::item_t & item)
			{
				begin_item();


			}

			virtual void end_process_items()
			{
				end_item();
			}

			virtual void update()
			{

			}

			enum class output_color_t
			{
				red_dark,
				red_light,

				green_dark,
				green_light,

				blue_dark,
				blue_light,

				gray_dark,
				gray_light,

				white,
				black,

				yellow_dark,
				yellow_light,

				pink_dark,
				pink_light,

				teal_dark,
				teal_light,

				////////////////////////////////

				background_default = black,

				color_section_header = blue_dark,
				color_section_header_name = blue_light,
				background_section_header = blue_dark,
				background_section_header_name = blue_light,

			};

		protected:

			provider_base_t(std::shared_ptr<logger_t> logger) : logger(logger)
			{

			}

			std::shared_ptr<logger_t> get_logger()
			{
				return logger;
			}

		private:

			std::shared_ptr<logger_t> logger;

			virtual void begin_item()
			{

			}
			virtual void color_next_output(output_color_t color, output_color_t background = output_color_t::background_default)
			{

			}
			virtual void write_section_header(const ustring_t & str)
			{
				std::string hashes(str.len() + 3 + 1 + 1 + 3, '#');

				color_next_output(output_color_t::color_section_header);
				output_string(ustring_t::from_utf8(hashes.c_str()));
				end_line();

				output_string(_U("### "));
				color_next_output(output_color_t::color_section_header_name); output_string(str);
				color_next_output(output_color_t::color_section_header); output_string(_U(" ###"));
				end_line();

				output_string(ustring_t::from_utf8(hashes.c_str()));
				end_line();
			}
			virtual void output_string(const ustring_t & str) = 0;
			virtual void end_line()
			{
				output_string(_U("\n"));
			}
			virtual void end_item()
			{
				end_line();
			}
		};

	}

}

#endif