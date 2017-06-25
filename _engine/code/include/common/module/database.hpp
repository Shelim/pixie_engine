#ifndef ENGINE_COMMON_MODULE_DATABASE_HPP
#define ENGINE_COMMON_MODULE_DATABASE_HPP
#pragma once

#include "common/ustring.hpp"
#include "common/filesystem.hpp"
#include "common/virtual_path.hpp"
#include "common/asset/path.hpp"
#include "common/data/input.hpp"
#include "common/data/output.hpp"
#include "common/data/database.hpp"
#include "common/logger.hpp"
#include <cereal/access.hpp>
#include <cereal/types/polymorphic.hpp>

#include <cereal/archives/binary.hpp>
#include <cereal/archives/xml.hpp>
#include <cereal/archives/json.hpp>
#include "common/xml_pixie_archive.hpp"

#include "common/module/database_manifest.hpp"

namespace engine
{

	namespace module
	{

		class database_t
		{

		public:

			database_t(std::shared_ptr<data::database_t> database_data, std::shared_ptr<database_manifest_t> database_manifest, std::shared_ptr<logger_t> logger) : database_data(database_data), database_manifest(database_manifest), logger(logger)
			{
				problems_since_last_update.merge_problems(database_manifest->get_problems_since_last_update());
				log_problems_since_last_update();
			}

			void init_update()
			{
				problems_since_last_update.clear();

				database_manifest->init_update();

				problems_since_last_update.merge_problems(database_manifest->get_problems_since_last_update());
			}

			const problems_t & get_problems_since_last_update() const
			{
				return problems_since_last_update;
			}

			void log_problems_since_last_update()
			{
				for (auto & problem : problems_since_last_update.get_items())
				{
					problem.log(logger.get());
				}
			}

			std::shared_ptr<database_manifest_t> get_database_manifest()
			{
				return database_manifest;
			}

			void query_for_all_problems(problems_t * output)
			{
				database_manifest->query_for_all_problems(output);
			}
			
		private:

			std::shared_ptr<data::database_t> database_data;
			std::shared_ptr<database_manifest_t> database_manifest;
			std::shared_ptr<logger_t> logger;

			problems_t problems_since_last_update;

		};

	}

}
#endif