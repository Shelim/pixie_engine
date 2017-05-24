#ifndef ENGINE_COMMON_ASSET_ITEM_HOLDER_HPP
#define ENGINE_COMMON_ASSET_ITEM_HOLDER_HPP
#pragma once

#include "common/asset/item.hpp"
#include "common/asset/collection.hpp"
#include <cereal/access.hpp>
#include <map>

namespace engine
{

	namespace asset
	{
		class database_t;
		
		class item_holder_t final
		{

		public:

			std::shared_ptr<item_t> get_item()
			{
				return item;
			}

			const path_t::canonized_t & get_path()
			{
				return path;
			}

			item_holder_t(const path_t::canonized_t & path, std::shared_ptr<database_t> database = nullptr) : path(path)
			{
				resolve_item(database);
			}

			item_holder_t(std::shared_ptr<item_t> item) : path(item->get_path().get_path()), item(item)
			{

			}

			bool is_deatached() const
			{
				return static_cast<bool>(item);
			}

			void deatach()
			{
				if (item->is_deatachable())
					item = item->deatach();
			}

			void resolve_item(std::shared_ptr<database_t> database);

		private:

			friend class cereal::access;
			
			item_holder_t()
			{

			}

			template<class archive_t> void save(archive_t & ar)
			{
				ar(path);

				bool is_deatached = false;

				if (item && item->is_deatached())
				{
					is_deatached = true;
				}

				ar(is_deatached);

				if (is_deatached)
					ar(item);
			}

			template<class archive_t> void load(archive_t & ar)
			{
				ar(path);

				bool is_deatached;

				ar(is_deatached);

				if (is_deatached)
				{
					ar(item);
				}
			}

			std::shared_ptr<item_t> item;
			path_t::canonized_t path;

		};

		class item_collection_holder_t
		{

		public:

			const path_t::canonized_t & get_path()
			{
				return path;
			}

			item_collection_holder_t(const path_t::canonized_t & path, std::shared_ptr<database_t> database = nullptr) : path(path), deatached(false)
			{
				resolve_collection(database);
			}

			item_collection_holder_t(std::shared_ptr<collection_base_t> collection) : path(collection->get_path()), collection(collection), deatached(false)
			{

			}

			void resolve_collection(std::shared_ptr<database_t> database);

			bool is_deatached() const
			{
				return deatached;
			}

			std::size_t get_count()
			{
				if (collection && !deatached)
					return collection->get_items().size();

				return items.size();
			}

			template<typename T> std::shared_ptr<T> get(const item_t::name_t & name, bool any_if_missing = false) const
			{
				static_assert(std::is_base_of<item_t, T>::value, "Assets requested must be of item_t class");

				if (collection && !deatached)
					return collection->get_item(name, any_if_missing);

				auto & iter = items_named.find(name.to_lower());
				if (iter == items_named.end())
				{
					if (any_if_missing) return get_any();
					return nullptr;
				}

				return std::dynamic_pointer_cast<T>(iter->second->get_item());
			}

			template<typename T> std::shared_ptr<T> get(std::size_t i) const
			{
				static_assert(std::is_base_of<item_t, T>::value, "Assets requested must be of item_t class");

				if (collection && !deatached)
					return collection->get_items()[i];
				
				return std::dynamic_pointer_cast<T>(items[i]->get_item());
			}

			template<typename T> std::shared_ptr<T> get_any() const
			{
				static_assert(std::is_base_of<item_t, T>::value, "Assets requested must be of item_t class");

				if (collection && !deatached)
					return collection->get_any();

				if (items.empty()) return nullptr;
				return *std::dynamic_pointer_cast<T>(items.begin());
			}

			typedef std::vector<std::unique_ptr<item_holder_t> > items_t;
			typedef std::map<item_t::name_t, item_holder_t* > items_named_t;
			
			void deatach()
			{
				if (!deatached)
				{
					deatached = true;

					if (collection)
					{
						auto & items = collection->get_items();
						this->items.clear();

						for (auto & item : items)
						{
							this->items.emplace_back(std::make_unique<item_holder_t>(item));
						}

						refresh_items_named();

						collection = nullptr;
					}
				}
			}

		private:

			friend class cereal::access;

			path_t::canonized_t path;
			bool deatached;
			std::shared_ptr<collection_base_t> collection;

			items_t items;
			items_named_t items_named;

			void refresh_items_named()
			{
				if (deatached)
				{
					items_named.clear();
					for (auto & item : items)
					{
						items_named[item->get_item()->get_name()] = item.get();
					}

				}
			}

			template<class archive_t> void save(archive_t & ar)
			{
				ar(path);

				ar(deatached);

				if (deatached)
				{
					ar(items);
				}
			}

			template<class archive_t> void load(archive_t & ar)
			{
				ar(path);

				ar(deatached);

				if (deatached)
				{
					ar(items);

					refresh_items_named();
				}
			}
		};
	}
}

#endif