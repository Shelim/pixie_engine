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
#include "utility/pattern/provider.hpp"
#include "utility/text/ustring.hpp"
#include "utility/container/concurrent_queue.hpp"
#include "utility/pattern/class_settings.hpp"
#include "utility/text/richtext.hpp"
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

		void output(const logger_item_t & item)
		{
			output(format(format_provider(item), item));
		}

	private:

		virtual void output(const richtext_t & text) = 0;
		virtual ustring_t format_provider(const logger_item_t & item) = 0;
		richtext_t format(ustring_t format, const logger_item_t & item);
	};

	class logger_output_t
	{

	};

	class logger_real_t : public logger_t
	{

	public:

		logger_real_t(std::shared_ptr<engine::frame_notifier_t> frame_notifier, std::unique_ptr<holder_t<logger_output_t> > logger_output_providers);

		~logger_real_t();

		void reset_raports(logger_item_t::module_t module = logger_item_t::module_t::unknown) final;

		logger_items_t get_raported_items(logger_item_t::module_t module = logger_item_t::module_t::unknown) const final;

		logger_items_t get_all_items(logger_item_t::module_t module = logger_item_t::module_t::unknown) const final;

	private:
		
		std::array<logger_items_t, static_cast<std::underlying_type<logger_item_t::module_t>::type>(logger_item_t::module_t::count)> items_cache;

		mutable std::recursive_mutex items_cache_mutex;

		concurrent_queue_t<logger_item_t> items_queue;

		std::shared_ptr<engine::frame_notifier_t> frame_notifier;

		std::unique_ptr<holder_t<logger_output_t> > logger_output_providers;

		std::thread output_thread;

		std::chrono::system_clock::time_point time_start;

		void output_thread_func();

		const logger_items_t & get_cache(logger_item_t::module_t module) const
		{
			return items_cache[static_cast<std::underlying_type<logger_item_t::module_t>::type>(module)];
		}

		logger_items_t & get_cache(logger_item_t::module_t module)
		{
			return items_cache[static_cast<std::underlying_type<logger_item_t::module_t>::type>(module)];
		}

		item_id_t log_local(logger_item_t::level_t level, logger_item_t::module_t module, bool raport, const ustring_t & file, uint32_t line, const ustring_t & function, std::size_t link, const ustring_t & message) final;
	};


	REGISTER_PROVIDER_BASE_TYPE(logger_output_t, logger_output_provider_base_t)

	SETTINGS_TABLE_START(logger_output_t)

		SETTINGS_TABLE_ENTRY(bool, is_file_richtext_format, true)

#define ENGINE_LOGGER_LEVEL_STD(level, file_pattern, terminal_pattern) SETTINGS_TABLE_ENTRY(ustring_t, format_file_##level, file_pattern) SETTINGS_TABLE_ENTRY(ustring_t, format_terminal_##level, terminal_pattern)
#include "std/logger_std.hpp"

	SETTINGS_TABLE_END()
}

#include "component/logger/output_provider/terminal.hpp"
#include "component/logger/output_provider/file.hpp"

#endif