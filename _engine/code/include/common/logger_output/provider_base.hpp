#ifndef ENGINE_COMMON_LOGGER_OUTPUT_PROVIDER_BASE_HPP
#define ENGINE_COMMON_LOGGER_OUTPUT_PROVIDER_BASE_HPP
#pragma once

#include <cstddef>
#include "common/logger.hpp"
#include "common/environment_info.hpp"
#include "common/utility/text/richtext.hpp"

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

			virtual void process_item(const logger_t::item_t & item) = 0;

			virtual void process_environment_info(std::shared_ptr<environment_info_t> environment_info) = 0;

			virtual void update()
			{

			}

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

		};

	}

}

#endif