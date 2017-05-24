#ifndef ENGINE_COMMON_DATA_ITEM_HPP
#define ENGINE_COMMON_DATA_ITEM_HPP
#pragma once

#include "common/ustring.hpp"
#include "common/filesystem.hpp"
#include "common/virtual_path.hpp"
#include "common/data/input.hpp"
#include "common/data/output.hpp"
#include "common/data/provider.hpp"
#include "common/filesystem.hpp"
#include "common/data/item_content.hpp"
#include <bitset>
#include <cereal/access.hpp>

namespace engine
{
	namespace data
	{
		class database_t;

		class item_base_t
		{
			
		public:

			virtual ~item_base_t()
			{

			}

			enum class flag_t
			{
				is_dirty,

				count
			};

			void set_dirty()
			{
				set_flag(flag_t::is_dirty, true);
			}

			bool is_dirty()
			{
				return is_flag(flag_t::is_dirty);
			}

			const virtual_path_t & get_path() const
			{
				return path;
			}

		protected:

			item_base_t(const virtual_path_t & path) : path(path)
			{

			}

		private:

			std::bitset<static_cast<std::size_t>(flag_t::count)> flags;

			void set_flag(flag_t flag, bool value)
			{
				flags.set(static_cast<std::size_t>(flag), value);
			}

			bool is_flag(flag_t flag) const
			{
				return flags.test(static_cast<std::size_t>(flag));
			}

			virtual_path_t path;

		};

		class item_t final : public item_base_t
		{

		public:

			item_content_base_t * get_base()
			{
				return content.get();
			}

			const item_content_base_t * get_base() const
			{
				return content.get();
			}

			template<class T> T * get()
			{
				return static_cast<T*>(get_base());
			}

			template<class T> const T * get() const
			{
				return static_cast<const T*>(get_base());
			}

			friend class database_items_t;

			friend cereal::access;

		private:

			template<class archive_t> void save(archive_t & ar) const
			{
				// ToDo: save corresponding content to archive
			}

			template<class archive_t> void load(archive_t & ar)
			{
				// ToDo: load corresponding content to archive
			}

			template<class T> static std::shared_ptr<item_t> create_item(const virtual_path_t & path)
			{
				return std::make_shared<item_t>(std::move(std::make_unique<T>(this)));
			}

			item_t(const virtual_path_t & path, std::unique_ptr<item_content_base_t> content) :
				item_base_t(path), content(std::move(content))
			{

			}

			item_t() : item_base_t(virtual_path_t()) // for cereal
			{

			}

			std::unique_ptr<item_content_base_t> content;
			std::unique_ptr<item_content_base_t> content_save;
			std::unique_ptr<item_content_base_t> content_load;
		};

		class items_collection_t final : public item_base_t
		{

		public:

			friend class database_items_t;

		private:

		};
	}

}

#endif