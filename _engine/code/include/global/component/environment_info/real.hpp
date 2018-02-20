#ifndef ENGINE_COMPONENT_ENVIRONMENT_INFO_REAL_HPP
#define ENGINE_COMPONENT_ENVIRONMENT_INFO_REAL_HPP
#pragma once

#include "global/component/environment_info.hpp"
#include "utility/pattern/provider.hpp"
#include "utility/pattern/class_settings.hpp"
#include "utility/manifest/version.hpp"
#include <array>

namespace engine
{
	class environment_info_output_provider_base_t
	{

	public:

		virtual ~environment_info_output_provider_base_t()
		{

		}

		virtual void output(environment_info_t::key_t key, environment_info_t::status_t status, const ustring_t & value) const = 0;

	private:
	};

	class environment_info_output_t
	{

	};

	class environment_info_real_t : public environment_info_t
	{

	public:

		environment_info_real_t(std::shared_ptr<platform::info_t> platform_info, std::shared_ptr<manifest_version_t> manifest_version, std::unique_ptr<holder_t<environment_info_output_t> > environment_info_output_providers);

		const ustring_t & get(key_t key) const final
		{
			return items[value_of(key)].get_value();
		}

		status_t get_status(key_t key) const final
		{
			return items[value_of(key)].get_status();
		}

	private:

		std::unique_ptr<holder_t<environment_info_output_t> > environment_info_output_providers;

		void set_info(key_t key, const ustring_t & val, status_t status = status_t::normal)
		{
			items[value_of(key)].set(val, status);
		}

		void gather_info();
		void output_info();

		class item_t
		{

		public:

			item_t() : status(status_t::not_yet_populated)
			{

			}

			status_t get_status() const
			{
				return status;
			}

			const ustring_t & get_value() const
			{
				return value;
			}


		private:

			friend class environment_info_real_t;

			void set(const ustring_t & value, status_t status = status_t::normal)
			{
				this->status = status;
				this->value = value;
			}

			status_t status;
			ustring_t value;
		};

		std::array<item_t, value_of(key_t::count)> items;
		std::shared_ptr<manifest_version_t> manifest_version;
		std::shared_ptr<platform::info_t> platform_info;
	};


	REGISTER_PROVIDER_BASE_TYPE(environment_info_output_t, environment_info_output_provider_base_t)

	SETTINGS_TABLE_START(environment_info_output_t)

		SETTINGS_TABLE_ENTRY(ustring_t, file_start)
		SETTINGS_TABLE_ENTRY(ustring_t, file_end)
		SETTINGS_TABLE_ENTRY(ustring_t, terminal_start)
		SETTINGS_TABLE_ENTRY(ustring_t, terminal_end)

#define ENGINE_ENVIRONMENT_INFO_STATUS_DEF(status) SETTINGS_TABLE_ENTRY(ustring_t, format_file_##status) SETTINGS_TABLE_ENTRY(ustring_t, format_terminal_##status)
#include "def/environment_info.def"

	SETTINGS_TABLE_END()
}

#include "global/component/environment_info/output_provider/file.hpp"
#include "global/component/environment_info/output_provider/terminal.hpp"

#endif