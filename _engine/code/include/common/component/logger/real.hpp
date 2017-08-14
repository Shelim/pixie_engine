#ifndef ENGINE_COMPONENT_LOGGER_REAL_HPP
#define ENGINE_COMPONENT_LOGGER_REAL_HPP
#pragma once

#include "component/logger.hpp"
#include "component/frame_notifier.hpp"
#include <pugixml.hpp>
#include <string>
#include <vector>
#include <bitset>
#include <array>
#include <mutex>
#include <map>
#include <thread>
#include <chrono>
#include <sstream>
#include <SDL.h>
#include <pugixml.hpp>
#include "component/environment_info.hpp"
#include "utility/pattern/provider.hpp"
#include "utility/text/ustring.hpp"
#include "utility/container/concurrent_queue.hpp"
#include "utility/pattern/class_settings.hpp"
#include <cereal/cereal.hpp>
#include <cereal/access.hpp>
#include <vlc/vlc.h>
#include <bitset>

namespace engine
{
	class logger_output_provider_base_t
	{

	public:

		virtual ~logger_output_provider_base_t()
		{

		}

		virtual void output_start() const = 0;
		virtual void output(const logger_item_t & item) const = 0;
		virtual void output_end() const = 0;

	private:
	};

	class logger_output_t
	{

	};

	class logger_real_t : public logger_t
	{

	public:

		logger_real_t(std::shared_ptr<engine::frame_notifier_t> frame_notifier, std::shared_ptr<engine::environment_info_t> environment_info, std::unique_ptr<holder_t<logger_output_t> > logger_output_providers);

		~logger_real_t();

		void reset_raports(logger_item_t::module_t module = logger_item_t::module_t::unknown) final;

		logger_items_t get_raported_items(logger_item_t::module_t module = logger_item_t::module_t::unknown) const final;

		logger_items_t get_all_items(logger_item_t::module_t module = logger_item_t::module_t::unknown) const final;

	private:
		
		std::array<logger_items_t, value_of(logger_item_t::module_t::count)> items_cache;

		mutable std::recursive_mutex items_cache_mutex;

		concurrent_queue_t<logger_item_t> items_queue;

		std::shared_ptr<engine::frame_notifier_t> frame_notifier;

		void output_start();
		void output_end();
		std::unique_ptr<holder_t<logger_output_t> > logger_output_providers;

		std::thread output_thread;

		std::chrono::system_clock::time_point time_start;

		void output_thread_func();

		const logger_items_t & get_cache(logger_item_t::module_t module) const
		{
			return items_cache[value_of(module)];
		}

		logger_items_t & get_cache(logger_item_t::module_t module)
		{
			return items_cache[value_of(module)];
		}

		item_id_t log_local(logger_item_t::level_t level, logger_item_t::module_t module, bool raport, const ustring_t & file, uint32_t line, const ustring_t & function, std::size_t link, const ustring_t & message) final;
	};


	REGISTER_PROVIDER_BASE_TYPE(logger_output_t, logger_output_provider_base_t)

	SETTINGS_TABLE_START(logger_output_t)

		SETTINGS_TABLE_ENTRY(ustring_t, file_start)
		SETTINGS_TABLE_ENTRY(ustring_t, file_end)
		SETTINGS_TABLE_ENTRY(ustring_t, terminal_start)
		SETTINGS_TABLE_ENTRY(ustring_t, terminal_end)

#define ENGINE_LOGGER_LEVEL_STD(level) SETTINGS_TABLE_ENTRY(ustring_t, format_file_##level) SETTINGS_TABLE_ENTRY(ustring_t, format_terminal_##level)
#include "std/logger_std.hpp"

	SETTINGS_TABLE_END()
}

#include "component/logger/output_provider/terminal.hpp"
#include "component/logger/output_provider/file.hpp"

#endif