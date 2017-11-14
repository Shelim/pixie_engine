#ifndef ENGINE_COMPONENT_CONFIG_PROVIDER_STORAGE_HPP
#define ENGINE_COMPONENT_CONFIG_PROVIDER_STORAGE_HPP
#pragma once

#include "component/config/real.hpp"
#include "utility/platform/config_storage.hpp"
#include <memory>
#include <chrono>
#include <cstdio>
#include "utility/process/service.hpp"
#include "utility/manifest/app.hpp"
#include "utility/pattern/enum.hpp"
#include <array>
#include <system_error>

namespace engine
{
	class config_provider_storage_actual_t
	{

	public:

		config_provider_storage_actual_t(std::shared_ptr<messenger_t> messenger, std::shared_ptr<platform::config_storage_t> config_storage, std::unique_ptr<config_provider_base_t::provider_default_t> defaults) : messenger(messenger), config_storage(config_storage), defaults(std::move(defaults))
		{
			rescan();
			rdy_vals = vals;
			swap_vals();
		}

		~config_provider_storage_actual_t()
		{
		}

		void service()
		{
			if (scanner_mutex.try_lock())
			{
				rescan();
				notify_on_differences();
				swap_vals();

				scanner_mutex.unlock();
			}
		}

#define ENGINE_CONFIG_GLOBAL_DEF(type_t, name) type_t get_global_##name() const { std::lock_guard<std::recursive_mutex> guard(ready_mutex); return rdy_vals.val_for_global_##name; };
#define ENGINE_CONFIG_LOCAL_DEF(type_t, app, name) type_t get_app_##app##_##name() const { std::lock_guard<std::recursive_mutex> guard(ready_mutex); return rdy_vals.val_for_app_##app##_##name; };
#define ENGINE_CONFIG_DEF(type_t, name) type_t get_cfg_##name(manifest_app_t::app_t app) const { std::lock_guard<std::recursive_mutex> guard(ready_mutex); return rdy_vals.val_for_cfg_##name[value_of(app)]; };
#include "def/config.def"
#define ENGINE_CONFIG_GLOBAL_DEF(type_t, name) config_provider_base_t::set_result_t set_global_##name(const type_t & val) { std::lock_guard<std::recursive_mutex> guard(ready_mutex); if(rdy_vals.val_for_global_##name != val) { std::lock_guard<std::recursive_mutex> guard(scanner_mutex); rdy_vals.val_for_global_##name = val; old_vals.val_for_global_##name = val; config_storage->store("global_" #name ##_u, val); return config_provider_base_t::set_result_t::success; } return config_provider_base_t::set_result_t::no_change; };
#define ENGINE_CONFIG_LOCAL_DEF(type_t, app, name) config_provider_base_t::set_result_t set_app_##app##_##name(const type_t & val) { std::lock_guard<std::recursive_mutex> guard(ready_mutex); if(rdy_vals.val_for_app_##app##_##name != val) { std::lock_guard<std::recursive_mutex> guard(scanner_mutex); rdy_vals.val_for_app_##app##_##name = val; old_vals.val_for_app_##app##_##name = val; config_storage->store("local_" #app "_" #name ##_u, val); return config_provider_base_t::set_result_t::success; } return config_provider_base_t::set_result_t::no_change; };
#define ENGINE_CONFIG_DEF(type_t, name) config_provider_base_t::set_result_t set_cfg_##name(manifest_app_t::app_t app, const type_t & val) { std::lock_guard<std::recursive_mutex> guard(ready_mutex); if(rdy_vals.val_for_cfg_##name[value_of(app)] != val) { std::lock_guard<std::recursive_mutex> guard(scanner_mutex); rdy_vals.val_for_cfg_##name[value_of(app)] = val; old_vals.val_for_cfg_##name[value_of(app)] = val; config_storage->store(format_string("cfg_#1#_" #name ##_u, manifest_app_t::get_app_name(app)), val); return config_provider_base_t::set_result_t::success; } return config_provider_base_t::set_result_t::no_change; };
#include "def/config.def"

	private:

		struct values_t
		{
#define ENGINE_CONFIG_GLOBAL_DEF(type_t, name) type_t val_for_global_##name;
#define ENGINE_CONFIG_LOCAL_DEF(type_t, app, name) type_t val_for_app_##app##_##name;
#define ENGINE_CONFIG_DEF(type_t, name) std::array<type_t, value_of(manifest_app_t::app_t::count)> val_for_cfg_##name;
#include "def/config.def"
		};

		values_t rdy_vals;
		values_t vals;
		values_t old_vals;
		mutable std::recursive_mutex scanner_mutex;
		mutable std::recursive_mutex ready_mutex;

		std::shared_ptr<messenger_t> messenger;
		std::shared_ptr<platform::config_storage_t> config_storage;

		void rescan();
		void notify_on_differences();
		void swap_vals()
		{
			std::swap(vals, old_vals);
		}

		std::unique_ptr<config_provider_base_t::provider_default_t> defaults;

	};


	class config_provider_storage_service_t : public service_base_t
	{

	public:

		config_provider_storage_service_t(std::shared_ptr<config_provider_storage_actual_t> actual) : actual(actual)
		{

		}

		ustring_t get_name() const final
		{
			return "Config Storage"_u;
		}

		task_base_t::result_t run() final
		{
			actual->service();
			return task_base_t::result_t::running;
		}

		std::chrono::duration<double> get_sleep_after() const final
		{
			return std::chrono::milliseconds(100);
		}

	private:

		std::shared_ptr<config_provider_storage_actual_t> actual;

	};

	class config_provider_config_storage_t : public config_provider_base_t
	{


	public:

		config_provider_config_storage_t(std::unique_ptr<service_t<config_provider_storage_service_t>> service, std::shared_ptr<config_provider_storage_actual_t> actual) : actual(actual), service(std::move(service))
		{
			this->service->start();
		}

		~config_provider_config_storage_t()
		{
			this->service->end();
		}

#define ENGINE_CONFIG_GLOBAL_DEF(type_t, name) type_t get_global_##name() const final { return actual->get_global_##name(); };
#define ENGINE_CONFIG_LOCAL_DEF(type_t, app, name) type_t get_app_##app##_##name() const final { return actual->get_app_##app##_##name(); };
#define ENGINE_CONFIG_DEF(type_t, name) type_t get_cfg_##name(manifest_app_t::app_t app) const final { return actual->get_cfg_##name(app); };
#include "def/config.def"
#define ENGINE_CONFIG_GLOBAL_DEF(type_t, name) config_provider_base_t::set_result_t set_global_##name(const type_t & val) final { return actual->set_global_##name(val); };
#define ENGINE_CONFIG_LOCAL_DEF(type_t, app, name) config_provider_base_t::set_result_t set_app_##app##_##name(const type_t & val) final { return actual->set_app_##app##_##name(val); };
#define ENGINE_CONFIG_DEF(type_t, name) config_provider_base_t::set_result_t set_cfg_##name(manifest_app_t::app_t app, const type_t & val) final { return actual->set_cfg_##name(app, val); };
#include "def/config.def"

	private:


		std::shared_ptr<messenger_t> messenger;
		std::shared_ptr<config_provider_storage_actual_t> actual;
		std::unique_ptr<service_t<config_provider_storage_service_t>> service;

	};

}

#endif