#ifndef ENGINE_COMMON_LOGGER_CONTAINER_HPP
#define ENGINE_COMMON_LOGGER_CONTAINER_HPP
#pragma once

#include "common/logger_output/providers.hpp"
#include "common/logger.hpp"
#include "common/config.hpp"
#include <memory.h>

namespace engine
{
	class logger_t;
	class config_t;

	class logger_container_t
	{

	public:

		logger_container_t(std::shared_ptr<logger_t> logger, std::shared_ptr<engine::logger_output::providers_t> providers) : logger(logger), providers(providers)
		{
			logger->set_output_providers(providers);
		}

		~logger_container_t()
		{

		}

		std::shared_ptr<logger_t> get()
		{
			return logger;
		}

		void update();

	private:

		std::shared_ptr<logger_t> logger;
		std::shared_ptr<engine::logger_output::providers_t> providers;

	};
}

#endif