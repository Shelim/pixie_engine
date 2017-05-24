#ifndef ENGINE_COMMON_ASSET_DATABASE_HPP
#define ENGINE_COMMON_ASSET_DATABASE_HPP
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

namespace engine
{

	namespace asset
	{

		class database_t
		{

		public:

			database_t(std::shared_ptr<data::database_t> data_database) : data_database(data_database)
			{

			}

			void init_update()
			{

			}

		private:

			std::shared_ptr<data::database_t> data_database;

		};

	}

}
#endif