#ifndef ENGINE_COMMON_CONFIG_HPP
#define ENGINE_COMMON_CONFIG_HPP
#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <mutex>
#include <thread>
#include <array>
#include "common/ustring.hpp"
#include "common/logger.hpp"
#include "common/platform.hpp"
#include "common/manifest_app.hpp"
#include <cereal/cereal.hpp>

#define CONFIG_CURRENT_APP2(x, output) output ##_## x
#define CONFIG_CURRENT_APP3(x, output) CONFIG_CURRENT_APP2(x, output)
#define CONFIG_CURRENT_APP(x) CONFIG_CURRENT_APP3(x, PIXIE_OUTPUT_UNIX_NAME)

namespace engine
{
	namespace data
	{
		class database_t;
	}

	namespace config_io
	{
		class provider_base_t;
	}

	class config_t final
	{
	public:

		config_t(std::shared_ptr<engine::logger_t> logger, std::shared_ptr<engine::platform_t> platform, std::shared_ptr<engine::manifest_app_t> manifest_app);
		~config_t();
		
		config_t(config_t const&) = delete;
		config_t(config_t &&) = delete;
		config_t& operator=(config_t const&) = delete;
		config_t& operator=(config_t &&) = delete;

		enum class item_t
		{
#define GAME_CONFIG_STD(name, valr, vald) name,
#include "common/std/config_std.hpp"
			count
		};


		template<class T> T get(item_t key)
		{
			std::lock_guard<std::recursive_mutex> guard(mutex);

			if (data[static_cast<std::size_t>(key)].is_empty())
			{
				return from_string<T>(def(key));
			}
			return from_string<T>(data[static_cast<std::size_t>(key)]);
		}

		template<class T> T get_prev(item_t key) const
		{
			std::lock_guard<std::recursive_mutex> guard(mutex);

			if (data_prev[static_cast<std::size_t>(key)].is_empty())
				return from_string<T>(def(key));

			return from_string<T>(data_prev[static_cast<std::size_t>(key)]);
		}

		bool is_updated(item_t key)
		{
			return data_prev[static_cast<std::size_t>(key)] != data[static_cast<std::size_t>(key)];
		}
		
		template<class T> void set(item_t key, const T & newval)
		{
			std::lock_guard<std::recursive_mutex> guard(mutex);
			resave();
		}

		template<class T> void get_def(item_t key) const
		{
			return from_string<T>(def(key));
		}
		
		void revert_to_default()
		{
			auto task = logger->p_task_start(_U("Reverting config to default"));
			set_initial();
			resave();
			logger->p_task_done(task);
		}
		typedef std::array<ustring_t, static_cast<std::size_t>(item_t::count)> data_t;

		void query_data(data_t * output);
		void set_data(const data_t & input);

		void set_provider(std::weak_ptr<engine::config_io::provider_base_t> value)
		{
			provider = value;
		}

	private:

		void resave();
		
		void set_initial()
		{
			for (std::size_t i = 0; i < static_cast<std::size_t>(item_t::count); i++)
			{
				data[i] = def(static_cast<item_t>(i));
			}
		}

		ustring_t def(item_t key);

		data_t data;
		data_t data_prev;
		std::shared_ptr<engine::logger_t> logger;
		std::shared_ptr<engine::platform_t> platform;
		std::shared_ptr<engine::manifest_app_t> manifest_app;
		std::weak_ptr<engine::config_io::provider_base_t> provider;
		std::recursive_mutex mutex;
		
	};

}

#endif