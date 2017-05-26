#ifndef ENGINE_COMMON_LOGGER_OUTPUT_PROVIDER_DATA_HPP
#define ENGINE_COMMON_LOGGER_OUTPUT_PROVIDER_DATA_HPP
#pragma once

#include "common/logger.hpp"
#include "common/logger_output/provider_base.hpp"
#include "common/execution_info.hpp"
#include "common/data/database.hpp"
#include "common/data/database_items.hpp"
#include "common/xml_pixie_archive.hpp"
#include <mutex>
#include <thread>
#include <future>

namespace engine
{
	namespace data
	{
		class database_t;
	}

	namespace logger_output
	{

		class provider_data_t final : public provider_base_t
		{

		public:

			provider_data_t(std::shared_ptr<engine::data::database_t> database, std::shared_ptr<logger_t> logger) : provider_base_t(logger), database(database)
			{

			}

			~provider_data_t()
			{

			}

			void force_resave() final
			{

			}

		private:

			void on_item_changed_local(const std::size_t item_changed) final
			{

			}

			void enable_local() final
			{

			}

			void disable_local() final
			{

			}

			std::shared_ptr<engine::data::database_t> database;

		};

#if 0

		class provider_data_t final : public provider_base_t
		{

		public:

			provider_data_t(std::shared_ptr<engine::data::database_t> database, std::shared_ptr<logger_t> logger) : provider_base_t(logger), database(database), requested_save(false), finish_thread(false)
			{
				save_thread = std::thread([this] { save_thread_func(); });
			}

			~provider_data_t()
			{
				finish_thread = true;
				save_thread.join();

				std::lock_guard<std::recursive_mutex> guard(mutex_snapshot);
				get_logger()->query_snapshot(&snapshot);
				save_local();
			}
			
			void force_resave() final
			{
				std::lock_guard<std::recursive_mutex> guard(mutex_snapshot);
				get_logger()->query_snapshot(&snapshot);
				save_local();
			}

		private:

			void on_item_changed_local(const std::size_t item_changed) final
			{
				if(is_enabled())
					save_async();
			}

			void enable_local() final
			{
				save_async();
			}

			void disable_local() final
			{
				save_async();
			}

			void save_async()
			{
				if (!requested_save)
				{
					std::lock_guard<std::recursive_mutex> guard(mutex_snapshot);
					get_logger()->query_snapshot(&snapshot);
				}
				requested_save = true;
			}

			void save_thread_func()
			{
				for (;;)
				{
					if (requested_save)
					{
						save_local();
						requested_save = false;
					}
					std::this_thread::sleep_for(std::chrono::milliseconds(10));
					if (finish_thread) break;
				}
			}

			std::thread save_thread;
			bool requested_save;
			bool finish_thread;

			std::recursive_mutex mutex_snapshot;
			logger_t::snapshot_t snapshot;
			std::shared_ptr<engine::data::database_t> database;

			void save_local()
			{
				std::lock_guard<std::recursive_mutex> guard(mutex_snapshot);
#define GAME_LOGGER_VIRTUAL_PATH_STD(path) auto output = database->write_or_create(virtual_path_t(path, virtual_path_t::type_t::log));
#include "common/std/virtual_path_std.hpp"
				
				engine::data::output_streambuf_t streambuf(output.get());

				std::ostream os(&streambuf);
				{
					XMLPixieOutputArchive archive(os, engine::XMLPixieOutputArchive::Options::WithXsl("./engine/log/log.xsl"));
					archive(cereal::make_nvp("logger", snapshot));
				}
			}

		};
#endif

	}

}

#endif