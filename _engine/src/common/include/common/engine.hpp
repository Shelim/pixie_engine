#ifndef ENGINE_COMMON_ENGINE_HPP
#define ENGINE_COMMON_ENGINE_HPP
#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>
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
	class engine_t;

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

		}

	private:

		void application_init();
		void application_update();
		void application_render();
		void application_shutdown();

		std::shared_ptr<engine::platform_t> platform;
		std::shared_ptr<engine::data::database_providers_t> database_providers;
		std::shared_ptr<engine::data::database_items_t> database_items;
		std::shared_ptr<engine::logger_container_t> logger_container;
		std::shared_ptr<engine::config_container_t> config_container;
		std::shared_ptr<engine::tasks_t> tasks;
	};

	class engine_container_t final
	{

	public:

		engine_container_t()
		{
			main_thread_id = std::this_thread::get_id();
		}

		static engine_container_t * get()
		{
			static engine_container_t engine_container;

			return &engine_container;
		}

		engine_t * get_engine()
		{
			return engine.get();
		}

		void run();

		~engine_container_t()
		{

		}

		void force_kill()
		{
			if (std::this_thread::get_id() == main_thread_id)
			{
				set_flag(flag_t::is_requested_shutdown, true);
				exit(EXIT_FAILURE);
			}
			else
			{
				set_flag(flag_t::is_requested_shutdown, true);

				std::unique_lock<std::mutex> lock(mutex);
				condition_variable.wait(lock, [this] {return is_flag(flag_t::is_completed_shutdown);});
				exit(EXIT_FAILURE);
			}
		}

	private:

		static std::string platform_get_executable_filename();

		std::thread::id main_thread_id;

		std::mutex mutex;
		std::condition_variable condition_variable;


		enum class flag_t
		{
			is_requested_shutdown,
			is_completed_shutdown,

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


		std::unique_ptr<engine_t> engine;

	};
}

#endif