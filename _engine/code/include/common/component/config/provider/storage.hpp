#ifndef ENGINE_COMPONENT_CONFIG_PROVIDER_STORAGE_HPP
#define ENGINE_COMPONENT_CONFIG_PROVIDER_STORAGE_HPP
#pragma once

#include "component/config/real.hpp"
#include <memory>
#include <chrono>
#include <cstdio>
#include "platform/config_storage.hpp"
#include <array>

namespace engine
{

	class config_provider_storage_t : public config_provider_base_t
	{


	public:

		config_provider_storage_t(std::shared_ptr<messenger_t> messenger, std::unique_ptr<settings_t<config_t>> configuration) : config_provider_base_t(std::move(configuration)), messenger(messenger)
		{
			rescan();
			rdy_vals = vals;
			swap_vals();
			scanner_thread = std::thread([this] { scanner_thread_func(); });
			
		}

		~config_provider_storage_t()
		{
			flags.set_flag(flag_t::complete_scanning, true);
			scanner_thread.join();
		}

#define GAME_CONFIG_GLOBAL_STD(type_t, name) type_t get_global_##name() const final { std::lock_guard<std::recursive_mutex> guard(ready_mutex); return rdy_vals.val_for_global_##name; };
#define GAME_CONFIG_LOCAL_STD(type_t, app, name) type_t get_app_##app##_##name() const final { std::lock_guard<std::recursive_mutex> guard(ready_mutex); return rdy_vals.val_for_app_##app##_##name; };
#define GAME_CONFIG_STD(type_t, name) type_t get_cfg_##name(manifest_app_t::app_t app) const final { std::lock_guard<std::recursive_mutex> guard(ready_mutex); return rdy_vals.val_for_cfg_##name[static_cast<std::underlying_type<manifest_app_t::app_t>::type>(app)]; };
#include "std/config_std.hpp"
#define GAME_CONFIG_GLOBAL_STD(type_t, name) bool set_global_##name(const type_t & val) final { std::lock_guard<std::recursive_mutex> guard(ready_mutex); if(rdy_vals.val_for_global_##name != val) { std::lock_guard<std::recursive_mutex> guard(scanner_mutex); rdy_vals.val_for_global_##name = val; old_vals.val_for_global_##name = val; platform::store("global_" #name ##_u, val); return true; } return false; };
#define GAME_CONFIG_LOCAL_STD(type_t, app, name) bool set_app_##app##_##name(const type_t & val) final { std::lock_guard<std::recursive_mutex> guard(ready_mutex); if(rdy_vals.val_for_app_##app##_##name != val) { std::lock_guard<std::recursive_mutex> guard(scanner_mutex); rdy_vals.val_for_app_##app##_##name = val; old_vals.val_for_app_##app##_##name = val; platform::store("local_" #app "_" #name ##_u, val); return true; } return false; };
#define GAME_CONFIG_STD(type_t, name) bool set_cfg_##name(manifest_app_t::app_t app, const type_t & val) final { std::lock_guard<std::recursive_mutex> guard(ready_mutex); if(rdy_vals.val_for_cfg_##name[static_cast<std::underlying_type<manifest_app_t::app_t>::type>(app)] != val) { std::lock_guard<std::recursive_mutex> guard(scanner_mutex); rdy_vals.val_for_cfg_##name[static_cast<std::underlying_type<manifest_app_t::app_t>::type>(app)] = val; old_vals.val_for_cfg_##name[static_cast<std::underlying_type<manifest_app_t::app_t>::type>(app)] = val; platform::store(format_string("cfg_#1#_" #name ##_u, manifest_app_t::get_app_name(app)), val); return true; } return false; };
#include "std/config_std.hpp"

	private:

		struct values_t
		{
#define GAME_CONFIG_GLOBAL_STD(type_t, name) type_t val_for_global_##name;
#define GAME_CONFIG_LOCAL_STD(type_t, app, name) type_t val_for_app_##app##_##name;
#define GAME_CONFIG_STD(type_t, name) std::array<type_t, static_cast<std::underlying_type<manifest_app_t::app_t>::type>(manifest_app_t::app_t::count)> val_for_cfg_##name;
#include "std/config_std.hpp"
		};

		values_t rdy_vals;
		values_t vals;
		values_t old_vals;
		mutable std::recursive_mutex scanner_mutex;
		mutable std::recursive_mutex ready_mutex;

		std::shared_ptr<messenger_t> messenger;

		enum class flag_t
		{
			complete_scanning,
			count
		};

		flags_t<flag_t> flags;

		std::thread scanner_thread;
		void rescan();
		void notify_on_differences();
		void swap_vals()
		{
			std::swap(vals, old_vals);
		}
		void scanner_thread_func()
		{
			while (!flags.is_flag(flag_t::complete_scanning))
			{
				if (scanner_mutex.try_lock())
				{
					rescan();
					notify_on_differences();
					swap_vals();

					scanner_mutex.unlock();
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
			}
		}

	};

}

#endif