#ifndef ENGINE_COMMON_ENGINE_HPP
#define ENGINE_COMMON_ENGINE_HPP
#pragma once

#include <thread>
#include <memory>
#include <condition_variable>
#include "common/injector.hpp"
#include "common/platform.hpp"
#include "common/logger.hpp"
#include "common/config.hpp"
#include "common/data/database_providers.hpp"
#include "common/data/item_content.hpp"
#include "common/logger_container.hpp"
#include "common/logger_output/providers.hpp"
#include "common/manifest_app.hpp"
#include "common/data/database_items.hpp"
#include "common/config_container.hpp"
#include "common/task/tasks.hpp"


namespace engine
{

	class engine_t final
	{

	public:

		engine_t(std::shared_ptr<engine::platform_t> platform,
			std::shared_ptr<engine::data::database_providers_t> database_providers,
			std::shared_ptr<engine::data::database_items_t> database_items,
			std::shared_ptr<engine::logger_container_t> logger_container,
			std::shared_ptr<engine::config_container_t> config_container,
			std::shared_ptr<engine::tasks_t> tasks) :

			platform(platform),
			database_providers(database_providers),
			database_items(database_items),
			logger_container(logger_container),
			config_container(config_container),
			tasks(tasks)
		{

		}

		~engine_t()
		{
			while (!is_shutdown_completed())
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
		
		static engine_t * get();

		void run();

		void shutdown()
		{
			set_flag(flag_t::is_requested_shutdown, true);
		}

		bool is_shutdown_completed()
		{
			return is_flag(flag_t::shutdown_completed);
		}

	private:

		static std::string platform_get_executable_filename();

		std::shared_ptr<engine::platform_t> platform;
		std::shared_ptr<engine::data::database_providers_t> database_providers;
		std::shared_ptr<engine::data::database_items_t> database_items;
		std::shared_ptr<engine::logger_container_t> logger_container;
		std::shared_ptr<engine::config_container_t> config_container;
		std::shared_ptr<engine::tasks_t> tasks;

		enum class flag_t
		{
			is_requested_shutdown,
			shutdown_completed,

			count
		};

		std::bitset<static_cast<std::size_t>(flag_t::count)> flags;

		void set_flag(flag_t flag, bool value)
		{
			flags.set(static_cast<std::size_t>(flag), value);
		}

		bool is_flag(flag_t flag) const
		{
			return flags.test(static_cast<std::size_t>(flag));
		}
	};
}

#endif