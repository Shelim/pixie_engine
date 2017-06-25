#ifndef ENGINE_COMMON_ASSET_ITEM_HPP
#define ENGINE_COMMON_ASSET_ITEM_HPP
#pragma once

#include "common/utility/text/ustring.hpp"
#include "common/filesystem.hpp"
#include "common/utility/vfs/virtual_path.hpp"
#include "common/asset/path.hpp"
#include "common/data/input.hpp"
#include "common/data/output.hpp"
#include "common/data/database_providers.hpp"
#include "common/logger.hpp"
#include <cereal/access.hpp>
#include <cereal/types/polymorphic.hpp>

#include <cereal/archives/binary.hpp>
#include <cereal/archives/xml.hpp>
#include <cereal/archives/json.hpp>

namespace engine
{

	namespace asset
	{
		class database_providers_t;

		class item_t : public std::enable_shared_from_this<item_t>
		{

		public:

			virtual ~item_t() {}

			typedef ustring_t name_t;

			const path_t & get_path() const
			{
				return path;
			}

			const name_t & get_name() const
			{
				return path.get_name();
			}

			virtual bool is_deferrable() const
			{
				return true;
			}

			virtual bool is_deatachable() const
			{
				return false;
			}

			bool is_destroyed() const
			{
				return destroyed;
			}

			bool is_deatached() const
			{
				return deatached;
			}

			item_t(item_t && other) = delete;
			item_t& operator=(item_t const&) = delete;
			item_t& operator=(item_t &&) = delete;

			void save_item(std::shared_ptr<data::database_providers_t> database, std::shared_ptr<logger_t> logger)
			{
				if(!deatached)
					save_local(database->write_or_create(path.get_path_real()), logger);
			}

			void destroy()
			{
				if (!deatached)
				{
					destroy_local();
					destroyed = true;
				}
			}

			std::shared_ptr<item_t> deatach()
			{
				if (!is_deatachable() || is_deatached()) return shared_from_this();

				return std::shared_ptr<item_t>(deatach_local());
			}

		protected:

			item_t(const virtual_path_t & virtual_path) : path(virtual_path), destroyed(false), deatached(false)
			{

			}

			item_t(item_t const& other) : path(other.path), destroyed(other.destroyed), deatached(true)
			{

			}

		private:

			virtual item_t * deatach_local() const
			{
				return new item_t(*this);
			}

			friend class cereal::access;

			path_t path;
			bool destroyed;
			bool deatached;

			template<class archive_t> void save(archive_t & ar)
			{
				ar(path);
				ar(destroyed);
				ar(deatached);
			}

			template<class archive_t> void load(archive_t & ar)
			{
				ar(path);
				ar(destroyed);
				ar(deatached);
			}

			virtual void reload_local(std::shared_ptr<database_providers_t> database, std::unique_ptr<data::input_t> input, std::shared_ptr<logger_t> logger)
			{

			}
			virtual void save_local(std::unique_ptr<data::output_t> output, std::shared_ptr<logger_t> logger)
			{

			}
			virtual void destroy_local() { }
			
			friend class item_deatachable_base_t;

			void reload(std::shared_ptr<database_providers_t> database, std::unique_ptr<data::input_t> input, std::shared_ptr<logger_t> logger)
			{
				if (!deatached)
				{
					reload_local(database, std::move(input), logger);
					destroyed = false;
				}
			}

		};

	}

}

#endif