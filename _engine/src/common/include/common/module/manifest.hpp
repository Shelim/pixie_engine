#ifndef ENGINE_COMMON_MODULE_MANIFEST_HPP
#define ENGINE_COMMON_MODULE_MANIFEST_HPP
#pragma once

#include "common/module/problem.hpp"
#include "common/filesystem.hpp"
#include "common/virtual_path.hpp"
#include "common/data/input.hpp"
#include "common/data/output.hpp"
#include "common/data/item.hpp"
#include <bitset>

namespace engine
{

	namespace module
	{

		class item_manifest_base_t : public data::item_state_t
		{

		public:

			virtual ~item_manifest_base_t()
			{

			}

			const name_full_t & get_name() const
			{
				return name;
			}

			const virtual_path_t & get_virtual_path() const
			{
				return virtual_path;
			}

			const problems_t & get_problems() const
			{
				return problems;
			}
			
			bool is_valid() const
			{
				return problems.is_valid();
			}

			bool force_sync_reload() final
			{
				return true;
			}

			void reload_async(data::input_t * input) final;
			void resave_async(data::output_t * output) final;

			static virtual_path_t create_path(const name_t & name, virtual_path_t::type_t type)
			{
				virtual_path_t::path_t ret;
				ret.append(name);
				ret.append(_U("/"));
				ret.append(manifest_filename());

				return virtual_path_t(ret, type);
			}

			static ustring_t manifest_filename()
			{
				return _U("_manifest.info");
			}

		protected:

			item_manifest_base_t(const name_full_t & name, virtual_path_t::type_t type) : name(name), virtual_path(create_path(name.get_name(), type))
			{

			}

		private:

			name_full_t name;
			virtual_path_t virtual_path;
			problems_t problems;
		};


		class item_module_manifest_t final : public item_manifest_base_t
		{

		public:

			item_module_manifest_t(const name_t & name) : item_manifest_base_t(name_full_t(name, name_full_t::type_t::module), virtual_path_t::type_t::modules)
			{

			}
			
			class submodules_t
			{

			public:

				enum type_t : uint8_t
				{
					allow_any_listed_submodule,
					allow_any_not_listed_submodule
				};

				enum class flag_t
				{
					allow_from_included_modules,

					count
				};

				typedef ustring_collection_t collection_t;

				const collection_t & get_submodules() const
				{
					return submodules;
				}

				collection_t & get_submodules()
				{
					return submodules;
				}

				const type_t get_type() const
				{
					return type;
				}

				void set_type(type_t value)
				{
					type = value;
				}

				const bool is_flag(flag_t flag) const
				{
					return flags.test(static_cast<std::size_t>(flag));
				}

				void set_flag(flag_t flag, bool value)
				{
					flags.set(static_cast<std::size_t>(flag), value);
				}

				submodules_t() : type(allow_any_not_listed_submodule)
				{

				}

			private:

				friend class cereal::access;

				template<class archive_t> void save(archive_t & ar) const
				{
					std::string flag_from_included_modules = "deny";
					if (is_flag(flag_t::allow_from_included_modules))
						flag_from_included_modules = "allow";

					ar(cereal::make_nvp("from_included_modules", flag_from_included_modules));

					std::string type = "";

					switch (get_type())
					{
					case allow_any_listed_submodule: type = "allow_any_listed_below"; break;
					case allow_any_not_listed_submodule: type = "allow_any_not_listed_below"; break;
					}

					ar(cereal::make_nvp("type", type));

					for (auto & iter : submodules)
					{
						ar(cereal::make_nvp("submodule", iter));
					}
				}

				template<class archive_t> void load(archive_t & ar)
				{
					flags.reset();
					type = allow_any_not_listed_submodule;
					submodules.clear();

					for (;;)
					{
						const auto name = ar.getNodeName();
						if (!name)
							break;

						std::string name_str = name;

						ustring_t value;
						ar(value);
						value = value.trim().to_lower();

						if (name_str == "from_included_modules")
						{
							if (value == _U("allow"))
								set_flag(flag_t::allow_from_included_modules, true);
						}
						else if (name_str == "type")
						{
							if (value == _U("allow_any_listed_below"))
								type = allow_any_listed_submodule;
							else
								type = allow_any_not_listed_submodule;
						}
						else
							submodules.push_back(value);
					}
				}

				collection_t submodules;
				type_t type;
				std::bitset<static_cast<std::size_t>(flag_t::count)> flags;

			};

			class includes_t
			{

			public:

				typedef ustring_collection_t collection_t;

				const collection_t & get_includes() const
				{
					return includes;
				}

				collection_t & get_includes()
				{
					return includes;
				}

			private:

				friend class cereal::access;

				collection_t includes;

				template<class archive_t> void save(archive_t & ar) const
				{
					for (auto & iter : includes)
					{
						ar(cereal::make_nvp("include", iter));
					}
				}

				template<class archive_t> void load(archive_t & ar)
				{
					includes.clear();
					for (;;)
					{
						const auto name = ar.getNodeName();
						if (!name)
							break;

						ustring_t value;
						ar(value);

						includes.push_back(value);
					}
				}

			};

			const includes_t & get_includes() const
			{
				return includes;
			}

			includes_t & get_includes()
			{
				return includes;
			}

			const submodules_t & get_submodules() const
			{
				return submodules;
			}

			submodules_t & get_submodules()
			{
				return submodules;
			}

		private:

			friend class cereal::access;

			includes_t includes;
			submodules_t submodules;

			template<class archive_t> void serialize(archive_t & ar)
			{
				ar(cereal::make_nvp("includes", includes));
				ar(cereal::make_nvp("submodules", submodules));
			}
		};

		class item_submodule_manifest_t final : public item_manifest_base_t
		{

		public:

			item_submodule_manifest_t(const ustring_t & name) : item_manifest_base_t(name_full_t(name, name_full_t::type_t::submodule), virtual_path_t::type_t::submodules)
			{

			}
			
			class modules_t final
			{

			public:

				typedef ustring_collection_t collection_t;

				const collection_t & get_modules() const
				{
					return modules;
				}

				collection_t & get_modules()
				{
					return modules;
				}

			private:

				friend class cereal::access;

				template<class archive_t> void save(archive_t & ar) const
				{
					for (auto & iter : modules)
					{
						ar(cereal::make_nvp("module", iter));
					}
				}

				template<class archive_t> void load(archive_t & ar)
				{
					modules.clear();
					for (;;)
					{
						const auto name = ar.getNodeName();
						if (!name)
							break;

						ustring_t value;
						ar(value);

						modules.push_back(value);
					}
				}

				collection_t modules;
			};

			class submodules_t final
			{

			public:

				typedef ustring_collection_t collection_t;

				const collection_t & get_submodules() const
				{
					return submodules;
				}

				collection_t & get_submodules()
				{
					return submodules;
				}

			private:

				friend class cereal::access;

				template<class archive_t> void save(archive_t & ar) const
				{
					for (auto & iter : submodules)
					{
						ar(cereal::make_nvp("submodule", iter));
					}
				}

				template<class archive_t> void load(archive_t & ar)
				{
					submodules.clear();
					for (;;)
					{
						const auto name = ar.getNodeName();
						if (!name)
							break;

						ustring_t value;
						ar(value);

						submodules.push_back(value);
					}
				}

				collection_t submodules;
			};

			class permission_t
			{

			public:

				virtual ~permission_t()
				{

				}

				const modules_t & get_modules() const
				{
					return modules;
				}

				modules_t & get_modules()
				{
					return modules;
				}

				const submodules_t & get_submodules() const
				{
					return submodules;
				}

				submodules_t & get_submodules()
				{
					return submodules;
				}

			protected:

				modules_t modules;
				submodules_t submodules;

			private:
			};

			class required_t final : public permission_t
			{

			public:


			private:

				friend class cereal::access;

				template<class archive_t> void save(archive_t & ar) const
				{
					ar(cereal::make_nvp("modules", modules));
					ar(cereal::make_nvp("submodules", submodules));
				}

				template<class archive_t> void load(archive_t & ar)
				{
					ar(cereal::make_nvp("modules", modules));
					ar(cereal::make_nvp("submodules", submodules));
				}

			};

			class forbidden_t final : public permission_t
			{

			public:

				enum class flag_t
				{
					forbid_any_module_but_required,
					forbid_any_sub_module_but_required,
					count
				};

				bool is_flag(flag_t flag) const
				{
					return flags.test(static_cast<std::size_t>(flag));
				}

				void set_flag(flag_t flag, bool value)
				{
					flags.set(static_cast<std::size_t>(flag), value);
				}

				forbidden_t()
				{
					flags.reset();
				}

			private:

				friend class cereal::access;

				template<class archive_t> void save(archive_t & ar) const
				{
					std::string any_module_but_required = "false";
					if (is_flag(flag_t::forbid_any_module_but_required))
						any_module_but_required = "true";

					std::string any_submodule_but_required = "false";
					if (is_flag(flag_t::forbid_any_sub_module_but_required))
						any_submodule_but_required = "true";

					ar(cereal::make_nvp("any_module_but_required", any_module_but_required));
					ar(cereal::make_nvp("any_submodule_but_required", any_submodule_but_required));

					ar(cereal::make_nvp("modules", modules));
					ar(cereal::make_nvp("submodules", submodules));
				}

				template<class archive_t> void load(archive_t & ar)
				{
					ustring_t any_module_but_required;
					ar(cereal::make_nvp("any_module_but_required", any_module_but_required));
					bool is_any_module_but_required = from_string<bool>(any_module_but_required.trim().to_lower());

					flags.reset();

					if (is_any_module_but_required)
						set_flag(flag_t::forbid_any_module_but_required, true);

					ustring_t any_submodule_but_required;
					ar(cereal::make_nvp("any_submodule_but_required", any_submodule_but_required));
					bool is_any_sub_module_but_required = from_string<bool>(any_submodule_but_required.trim().to_lower());

					if (is_any_sub_module_but_required)
						set_flag(flag_t::forbid_any_sub_module_but_required, true);

					ar(cereal::make_nvp("modules", modules));
					ar(cereal::make_nvp("submodules", submodules));
				}

				std::bitset<static_cast<std::size_t>(flag_t::count)> flags;
			};

			const required_t & get_required() const
			{
				return required;
			}

			const forbidden_t & get_forbidden() const
			{
				return forbidden;
			}

			required_t & get_required()
			{
				return required;
			}

			forbidden_t & get_forbidden()
			{
				return forbidden;
			}

		private:

			friend class cereal::access;

			required_t required;
			forbidden_t forbidden;

			template<class archive_t> void serialize(archive_t & ar)
			{
				ar(cereal::make_nvp("requires", required));
				ar(cereal::make_nvp("forbids", forbidden));
			}
		};

		class module_manifest_t : public data::item_holder_t<item_module_manifest_t>
		{

		public:

			module_manifest_t(const virtual_path_t & path, data::database_t * database): item_holder_t<item_module_manifest_t>(path, database)
			{

			}


		private:


		};

		class submodule_manifest_t : public data::item_holder_t<item_submodule_manifest_t>
		{

		public:

			submodule_manifest_t(const virtual_path_t & path, data::database_t * database) : item_holder_t(path, database)
			{

			}


		private:


		};
	}
}

#endif