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
			logger_move_thread = std::thread([this] { logger_move(); });
		}

		~logger_container_t()
		{
			logger->kill_item_queue();
			logger_move_thread.join();
		}

		std::shared_ptr<logger_t> get()
		{
			return logger;
		}

		void update();

	private:

		std::thread logger_move_thread;

		void logger_move()
		{
			for (;;)
			{				
				logger_t::item_t item = logger->query_next_item();
				if (item.get_level() == logger_t::item_t::level_t::finished)
					break;

				providers->process_item(item);
			}
		}

		std::shared_ptr<logger_t> logger;
		std::shared_ptr<engine::logger_output::providers_t> providers;

	};
}

#endif