#ifndef ENGINE_COMMON_LOGGER_OUTPUT_PROVIDER_DATA_HPP
#define ENGINE_COMMON_LOGGER_OUTPUT_PROVIDER_DATA_HPP
#pragma once

#include "common/logger.hpp"
#include "common/logger_output/provider_base.hpp"
#include "common/execution_info.hpp"
#include "common/data/database.hpp"
#include "common/data/database_items.hpp"
#include "common/xml_pixie_archive.hpp"
#include "common/platform.hpp"
#include <cereal/types/vector.hpp>
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
		class content_t final : public engine::data::item_content_base_t
		{
		public:

			content_t(engine::data::item_generic_t * item) : item_content_base_t(item)
			{
				item->allow_changes_when_deleted();
				item->mark_as_auto_resave();
				item->mark_as_do_not_log_operations();
			}

			content_t(destroyed_t destroyed)
			{

			}

			void query_logger(logger_t * logger, bool force)
			{
				if (force)
				{
					std::lock_guard<std::mutex> guard(mutex);
					logger->query_snapshot(&snapshot_next);
				}
				else
				{
					if (mutex.try_lock())
					{
						logger->query_snapshot(&snapshot_next);
						mutex.unlock();
					}
				}
				this->set_dirty();
			}
			

		private:

			logger_t::snapshot_t snapshot;
			logger_t::snapshot_t snapshot_next;

			content_t(const content_t & other) : snapshot(other.snapshot), snapshot_next(other.snapshot_next)
			{

			}

			content_t * clone() const final
			{
				return new content_t(*this);
			}

			bool resave(engine::data::output_t * output) final
			{
				engine::data::output_streambuf_t streambuf(output);

				std::ostream os(&streambuf);
				{
					cereal::XMLPixieOutputArchive archive(os, cereal::XMLPixieOutputArchive::Options::WithXsl("./engine/log/log.xsl"));
					archive(cereal::make_nvp("logger", snapshot));
				}

				return true;
			}

			bool execute_output_operation(const engine::data::item_operation_t::step_t & step, engine::data::item_operation_t * operation) final
			{
				if (step.get_id() == 0)
				{
					operation->add_step('lock', engine::data::item_operation_t::step_t::caller_t::sync_force_main_thread);
					operation->add_step('save', engine::data::item_operation_t::step_t::caller_t::async);
				}
				else if (step.get_id() == 'lock')
				{
					std::lock_guard<std::mutex> guard(mutex);
					snapshot = snapshot_next;
				}
				else if (step.get_id() == 'save')
				{
					resave(operation->get_output());
				}
				return true;
			}

			std::mutex mutex;
		};

		typedef data::item_t<content_t > item_content_t;

		class provider_data_t final : public provider_base_t
		{

		public:

			provider_data_t(std::shared_ptr<engine::data::database_items_t> database_item, std::shared_ptr<logger_t> logger) : provider_base_t(logger), database_item(database_item)
			{

#define GAME_LOGGER_DATA_VIRTUAL_PATH_STD(path) data_item = database_item->get_item<content_t>(virtual_path_t(path, virtual_path_t::type_t::log));
#include "common/std/virtual_path_std.hpp"
			}

			~provider_data_t()
			{

			}

			void force_resave() final;
			

		private:

			void on_item_changed_local(const std::size_t item_changed) final
			{
				data_item->get()->query_logger(get_logger().get(), false);
			}

			std::weak_ptr<engine::data::database_items_t> database_item;

			std::shared_ptr<item_content_t> data_item;


		};


	}

}

#endif