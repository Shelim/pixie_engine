#ifndef ENGINE_COMPONENT_CONFIG_PROVIDER_WINDOWS_REGISTRY_HPP
#define ENGINE_COMPONENT_CONFIG_PROVIDER_WINDOWS_REGISTRY_HPP
#pragma once

#if PIXIE_WINDOWS

#include "global/component/config_storage/real.hpp"
#include "global/core/manifest/manifest.hpp"
#include "global/core/process/service.hpp"
#include "global/core/messenger/messenger.hpp"

namespace engine
{

	class config_storage_provider_windows_registry_rescan_service_t : public service_base_t
	{

		public:

			config_storage_provider_windows_registry_rescan_service_t(std::shared_ptr<messenger_config_storage_t> messenger_config_storage) : messenger_config_storage(messenger_config_storage)
			{

			}

			ustring_t get_name() const final
			{
				return "Config storage provider (Windows Registry) rescan service"_u;
			}

			task_base_t::result_t run() final
			{
				messenger_config_storage->write(std::make_shared<messenger::msg_config_storage_t>(messenger::msg_config_storage_t::type_t::source_updated));
				if(flags.is_flag(flag_t::requested_shutdown))
					return task_base_t::result_t::completed;
				return task_base_t::result_t::running;
			}

			std::chrono::duration<double> get_sleep_after() const final
			{
				return std::chrono::milliseconds(250);
			}

			void on_end_requested() final
			{
				flags.set_flag(flag_t::requested_shutdown, true);
			}

		private:

			enum class flag_t
			{
				requested_shutdown,
				count
			};

			flags_t<flag_t> flags;

			std::shared_ptr<messenger_config_storage_t> messenger_config_storage;
	};

	class config_storage_provider_windows_registry_t : public config_storage_provider_base_t
	{

	public:

		config_storage_provider_windows_registry_t(std::shared_ptr<manifest_windows_t> manifest_windows, std::unique_ptr<service_t<config_storage_provider_windows_registry_rescan_service_t> > config_storage_provider_windows_registry_rescan_service);

		void store(const ustring_t & key, bool val) final;
		void store(const ustring_t & key, uint8_t val) final;
		void store(const ustring_t & key, uint16_t val) final;
		void store(const ustring_t & key, uint32_t val) final;
		void store(const ustring_t & key, uint64_t val) final;
		void store(const ustring_t & key, int8_t val) final;
		void store(const ustring_t & key, int16_t val) final;
		void store(const ustring_t & key, int32_t val) final;
		void store(const ustring_t & key, int64_t val) final;
		void store(const ustring_t & key, const ustring_t & val) final;

		bool retrieve(const ustring_t & key, bool def_val) final;
		uint8_t retrieve(const ustring_t & key, uint8_t def_val) final;
		uint16_t retrieve(const ustring_t & key, uint16_t def_val) final;
		uint32_t retrieve(const ustring_t & key, uint32_t def_val) final;
		uint64_t retrieve(const ustring_t & key, uint64_t def_val) final;
		int8_t retrieve(const ustring_t & key, int8_t def_val) final;
		int16_t retrieve(const ustring_t & key, int16_t def_val) final;
		int32_t retrieve(const ustring_t & key, int32_t def_val) final;
		int64_t retrieve(const ustring_t & key, int64_t def_val) final;
		ustring_t retrieve(const ustring_t & key, const ustring_t & def_val) final;

	private:

		std::shared_ptr<manifest_windows_t> manifest_windows;
		std::unique_ptr<service_t<config_storage_provider_windows_registry_rescan_service_t> > config_storage_provider_windows_registry_rescan_service;

	};

}

#endif
#endif