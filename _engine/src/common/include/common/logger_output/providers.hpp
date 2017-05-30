#ifndef ENGINE_COMMON_LOGGER_OUTPUT_PROVIDER_COLLECTION_HPP
#define ENGINE_COMMON_LOGGER_OUTPUT_PROVIDER_COLLECTION_HPP
#pragma once

#include "common/logger_output/provider_base.hpp"
#include "common/logger_output/provider_console.hpp"
#include "common/logger_output/provider_data.hpp"
#include "common/config.hpp"

namespace engine
{
	class logger_t;
	class config_t;
	
	namespace logger_output
	{
		class providers_t final
		{

		public:

			
			providers_t
			(
#define LOGGER_OUTPUT_STD(logger_output_var) std::shared_ptr<engine::logger_output::provider_##logger_output_var##_t > provider_##logger_output_var,
#include "common/std/logger_output_std.hpp"
				std::shared_ptr<environment_info_t> environment_info
			) :
#define LOGGER_OUTPUT_STD(logger_output_var) provider_##logger_output_var(provider_##logger_output_var),
#include "common/std/logger_output_std.hpp"
				environment_info(environment_info)
			{

#define LOGGER_OUTPUT_STD(logger_output_var) provider_##logger_output_var->output_environment_info(environment_info); provider_##logger_output_var->begin_process_items();
#include "common/std/logger_output_std.hpp"

			}

			~providers_t()
			{

#define LOGGER_OUTPUT_STD(logger_output_var) provider_##logger_output_var->end_process_items();
#include "common/std/logger_output_std.hpp"
			}

			void process_item(const logger_t::item_t & item)
			{
#define LOGGER_OUTPUT_STD(logger_output_var) provider_##logger_output_var->process_item(item);
#include "common/std/logger_output_std.hpp"

			}

#define LOGGER_OUTPUT_STD(logger_output_var) std::shared_ptr<engine::logger_output::provider_##logger_output_var##_t> get_provider_##logger_output_var() { return provider_##logger_output_var; };
#include "common/std/logger_output_std.hpp"

			void update()
			{
#define LOGGER_OUTPUT_STD(logger_output_var) provider_##logger_output_var->update();
#include "common/std/logger_output_std.hpp"
			}

		private:

#define LOGGER_OUTPUT_STD(logger_output_var) std::shared_ptr<engine::logger_output::provider_##logger_output_var##_t> provider_##logger_output_var;
#include "common/std/logger_output_std.hpp"

			std::shared_ptr<environment_info_t> environment_info;

		};

	}

}

#endif