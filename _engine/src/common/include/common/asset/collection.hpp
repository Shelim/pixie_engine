#ifndef ENGINE_COMMON_ASSET_COLLECTION_HPP
#define ENGINE_COMMON_ASSET_COLLECTION_HPP
#pragma once


#include "common/asset/item.hpp"
#include <memory>

namespace engine
{

	namespace asset
	{
		class database_t;

		class collection_base_t
		{

		public:

			virtual ~collection_base_t() {}

			collection_base_t& operator=(collection_base_t const&) = delete;
			collection_base_t(collection_base_t const& ) = delete;

			const path_t::canonized_t & get_path() const
			{
				return path;
			}

			typedef std::vector<std::shared_ptr<item_t> > items_t;
			typedef std::map<item_t::name_t, std::shared_ptr<item_t> > items_named_t;

			const items_t & get_items() const
			{
				return items;
			}

			template<typename T> std::shared_ptr<T> get(const item_t::name_t & name, bool any_if_missing = false) const
			{
				static_assert(std::is_base_of<item_t, T>::value, "Assets requested must be of item_t class");
				auto & iter = items_named.find(name.to_lower());
				if (iter == items_named.end())
				{
					if (any_if_missing) return get_any();
					return nullptr;
				}

				return std::static_pointer_cast<T>(iter->second);

			}

			template<typename T> std::shared_ptr<T> get_any() const
			{
				static_assert(std::is_base_of<item_t, T>::value, "Assets requested must be of item_t class");
				if (items.empty()) return nullptr;
				return *std::static_pointer_cast<T>(items.begin());
			}

			friend class assets;

			typedef ustring_t extension_t;

		protected:

			collection_base_t(const path_t::canonized_t & path, extension_t extension = _U("")) : path(path), extension(extension) { }

			const extension_t & get_extension()
			{
				return extension;
			}

			items_t & get_items_for_mod()
			{
				return items;
			}

		private:

			void refresh_items_named()
			{
				items_named.clear();
				for (auto & item : items)
				{
					items_named[item->get_name()] = item;
				}
			}

			items_t items;
			items_named_t items_named;

			path_t::canonized_t path;
			extension_t extension;

			template<class T> friend class collection_t;
			
			void reload(std::shared_ptr<database_t> database, std::shared_ptr<logger_t> logger)
			{
				reload_local(database, logger);
				refresh_items_named();
			}

			virtual void reload_local(std::shared_ptr<database_t> database, std::shared_ptr<logger_t> logger)
			{

			}
		};

		template<class T> class collection_t : public collection_base_t, public std::enable_shared_from_this<collection_t<T> >
		{

		public:

			static_assert(std::is_base_of<item_t, T>::value, "Assets in collection must be of item_t class");

			virtual ~collection_t() {}

			collection_t& operator=(collection_t const&) = delete;

			typedef std::vector<std::shared_ptr<T> > items_casted_t;

			const items_casted_t & get_items_casted() const
			{
				return items_casted;
			}

		protected:

			collection_t(const path_t::canonized_t & path, extension_t extension = _U("")) : collection_base_t(path, extension)
			{

			}

		private:

			items_casted_t items_casted;

			void refresh_items_casted()
			{
				items_casted.clear();

				items_t & items = get_items();

				items_casted.insert(items_casted.end(), std::dynamic_pointer_cast<T*>(*items.begin()), std::dynamic_pointer_cast<T*>(*items.end()));
			}

			void reload_local(std::shared_ptr<database_t> database, std::shared_ptr<logger_t> logger) final
			{
				
				refresh_items_casted(); // at the end!!!



				/*
				this->assets = assets.get();

				std::set<ustring> filenames;

				if (extension.is_empty())
					filenames = assets->iterate_directories(get_virtual_path());
				else
					filenames = assets->iterate_in_directory(get_virtual_path(), extension);

				for (auto item = items.begin(); item != items.end();)
				{
					ustring path = std::static_pointer_cast<asset>(*item)->get_virtual_path();
					int32_t last_slash = path.last_index_of_utf8(u8"/");
					if (last_slash != -1)
						path = path.substr(last_slash + 1);

					if (filenames.find(path) == filenames.end())
					{
						items_named.erase((*item)->get_name().to_lower());
						remove(*item);
						item = items.erase(item);
					}
					else
						item++;
				}

				for (auto filename = filenames.begin(); filename != filenames.end(); filename++)
				{
					bool found = false;
					for (auto item = items.begin(); item != items.end(); item++)
					{
						ustring path = std::static_pointer_cast<asset>(*item)->get_virtual_path();
						int32_t last_slash = path.last_index_of_utf8(u8"/");
						if (last_slash != -1)
							path = path.substr(last_slash + 1);

						if (path == *filename)
						{
							found = true;
							break;
						}
					}
					if (!found)
					{
						ustring full_filename = get_virtual_path();
						full_filename.append_utf8(u8"/");
						full_filename.append(*filename);

						std::shared_ptr<T> asset = assets->load<T>(full_filename);
						items_named[asset->get_name().to_lower()] = asset;
						items.push_back(asset);
						add(asset);
					}
				}

				assets->after_reload_collection(shared_from_this());
				*/
			}

			virtual void on_remove(std::shared_ptr<T> asset)
			{

			}

			virtual void on_add(std::shared_ptr<T> asset)
			{

			}

			virtual void on_reload_completed()
			{

			}

		};
	}

}

#endif