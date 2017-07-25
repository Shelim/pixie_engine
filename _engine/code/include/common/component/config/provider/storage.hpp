#ifndef ENGINE_COMPONENT_CONFIG_PROVIDER_STORAGE_HPP
#define ENGINE_COMPONENT_CONFIG_PROVIDER_STORAGE_HPP
#pragma once

#include "component/config/real.hpp"
#include <memory>
#include <chrono>
#include <cstdio>
#include "platform/config_storage.hpp"

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

#define GAME_CONFIG_STD(type, name) const type & get_##name() const final { std::lock_guard<std::recursive_mutex> guard(ready_mutex); return rdy_vals.val_for_##name; };
#include "std/config_std.hpp"
#define GAME_CONFIG_STD(type, name) bool set_##name(const type & val) final { std::lock_guard<std::recursive_mutex> guard(ready_mutex); if(rdy_vals.val_for_##name != val) { std::lock_guard<std::recursive_mutex> guard(scanner_mutex); rdy_vals.val_for_##name = val; old_vals.val_for_##name = val; platform::store(ustring_t::from_utf8(#name), val); return true; } return false; };
#include "std/config_std.hpp"

	private:

		struct values_t
		{
#define GAME_CONFIG_STD(type, name) type val_for_##name;
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