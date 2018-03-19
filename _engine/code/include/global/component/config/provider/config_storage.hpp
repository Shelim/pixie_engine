#ifndef ENGINE_COMPONENT_CONFIG_PROVIDER_STORAGE_HPP
#define ENGINE_COMPONENT_CONFIG_PROVIDER_STORAGE_HPP
#pragma once

#include "global/component/config/real.hpp"
#include "global/component/config_storage.hpp"
#include "utility/text/format/string.hpp"

namespace engine
{
	class config_provider_config_storage_t : public config_provider_base_t
	{


	public:

		config_provider_config_storage_t(std::shared_ptr<config_storage_t> config_storage, std::unique_ptr<settings_t<config_t>> settings, std::unique_ptr<settings_t<config_storage_t>> storage_settings)  : config_storage(config_storage), settings(std::move(settings)), storage_settings(std::move(storage_settings))
		{
			
		}

		~config_provider_config_storage_t()
		{
			
		}

#define ENGINE_CONFIG_GLOBAL_IMPL(name, type) \
		type get_global_##name() const final { std::lock_guard<std::recursive_mutex> guard(mutex); return config_storage->retrieve(storage_settings->get()->key_for_global_##name(), settings->get()->global_##name()); } \
		set_result_t set_global_##name(type val) final { std::lock_guard<std::recursive_mutex> guard(mutex); if(get_global_##name() != val) { config_storage->store(storage_settings->get()->key_for_global_##name(), val); return set_result_t::success; } else return set_result_t::no_change; }

#define ENGINE_CONFIG_GLOBAL(...) DEFINE_TYPE_PASS(ENGINE_CONFIG_GLOBAL_IMPL, __VA_ARGS__)

#define ENGINE_CONFIG_ONLY_FOR_APP_IMPL(name, type, app) \
		type get_app_##app##_##name() const final { std::lock_guard<std::recursive_mutex> guard(mutex); return config_storage->retrieve(storage_settings->get()->key_for_app_##app##_##name(), settings->get()->app_##app##_##name()); } \
		set_result_t set_app_##app##_##name(type val) final { std::lock_guard<std::recursive_mutex> guard(mutex); if(get_app_##app##_##name() != val) { config_storage->store(storage_settings->get()->key_for_app_##app##_##name(), val); return set_result_t::success; } else return set_result_t::no_change; }

#define ENGINE_CONFIG_ONLY_FOR_APP(...) DEFINE_TYPE_PASS(ENGINE_CONFIG_ONLY_FOR_APP_IMPL, __VA_ARGS__)

#define ENGINE_CONFIG_LOCAL_IMPL(name, type) \
		type get_local_##name(app_t::kind_t app) const final { std::lock_guard<std::recursive_mutex> guard(mutex); return config_storage->retrieve(format_string(storage_settings->get()->key_for_local_##name(), app), settings->get()->local_##name()); } \
		set_result_t set_local_##name(app_t::kind_t app, type val) final { std::lock_guard<std::recursive_mutex> guard(mutex); if(get_local_##name(app) != val) { config_storage->store(format_string(storage_settings->get()->key_for_local_##name(), app), val); return set_result_t::success; } else return set_result_t::no_change; }

#define ENGINE_CONFIG_LOCAL(...) DEFINE_TYPE_PASS(ENGINE_CONFIG_LOCAL_IMPL, __VA_ARGS__)

#include "def/config.def"

	private:

		std::shared_ptr<config_storage_t> config_storage;
		std::unique_ptr<settings_t<config_t>> settings;
		std::unique_ptr<settings_t<config_storage_t>> storage_settings;
		mutable std::recursive_mutex mutex;

	};

}

#endif