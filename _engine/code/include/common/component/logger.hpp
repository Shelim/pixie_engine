#ifndef ENGINE_COMPONENT_LOGGER_HPP
#define ENGINE_COMPONENT_LOGGER_HPP
#pragma once

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
#include "utility/text/ustring.hpp"
#include "component/frame_notifier.hpp"
#include "utility/pattern/flags.hpp"
#include "component/logger/item.hpp"
#include "utility/text/parser.hpp"
#include <cereal/cereal.hpp>
#include <cereal/access.hpp>
#include <vlc/vlc.h>
#include <bitset>

namespace engine
{

	class logger_t
	{
	public:
		
		virtual void reset_raports(logger_item_t::module_t module = logger_item_t::module_t::unknown) = 0;

		typedef size_t item_id_t;

		template<class ...Args> item_id_t log(logger_item_t::level_t level, logger_item_t::module_t module, bool raport, const ustring_t & file, uint32_t line, const ustring_t & function, std::size_t link, const ustring_t & message, Args... args)
		{
			return log_local(level, module, raport, file, line, function, link, format_string(message, args...));
		}

		virtual logger_items_t get_raported_items(logger_item_t::module_t module = logger_item_t::module_t::unknown) const = 0;
		virtual logger_items_t get_all_items(logger_item_t::module_t module = logger_item_t::module_t::unknown) const = 0;

		logger_t()
		{

		}

		virtual ~logger_t()
		{

		}

	private:
		
		virtual item_id_t log_local(logger_item_t::level_t level, logger_item_t::module_t module, bool raport, const ustring_t & file, uint32_t line, const ustring_t & function, std::size_t link, const ustring_t & message) = 0;
		
	};
}

#include "component/logger/dummy.hpp"
#include "component/logger/real.hpp"

#define log_msg(module, ...) log(engine::logger_item_t::level_t::message, engine::logger_item_t::module_t::module, false, engine::ustring_t::from_ascii(__FILE__), __LINE__, engine::ustring_t::from_ascii(__FUNCTION__), -1, __VA_ARGS__)
#define log_task_start(module, ...) log(engine::logger_item_t::level_t::task_started, engine::logger_item_t::module_t::module, false, engine::ustring_t::from_ascii(__FILE__), __LINE__, engine::ustring_t::from_ascii(__FUNCTION__), -1, __VA_ARGS__)
#define log_task_done(task_id) log(engine::logger_item_t::level_t::task_done, engine::logger_item_t::module_t::unknown, false, engine::ustring_t::from_ascii(__FILE__), __LINE__, engine::ustring_t::from_ascii(__FUNCTION__), task_id, ""_u)
#define log_task_failed(task_id) log(engine::logger_item_t::level_t::task_failed, engine::logger_item_t::module_t::unknown, false, engine::ustring_t::from_ascii(__FILE__), __LINE__, engine::ustring_t::from_ascii(__FUNCTION__), task_id, ""_u)
#define log_task_failed_raport(task_id) log(engine::logger_item_t::level_t::task_failed, engine::logger_item_t::module_t::unknown, true, engine::ustring_t::from_ascii(__FILE__), __LINE__, engine::ustring_t::from_ascii(__FUNCTION__), task_id, ""_u)
#define log_warn(module, ...) log(engine::logger_item_t::level_t::warning, engine::logger_item_t::module_t::module, false, engine::ustring_t::from_ascii(__FILE__), __LINE__, engine::ustring_t::from_ascii(__FUNCTION__), -1, __VA_ARGS__)
#define log_warn_raport(module, ...) log(engine::logger_item_t::level_t::warning, engine::logger_item_t::module_t::module, true, engine::ustring_t::from_ascii(__FILE__), __LINE__, engine::ustring_t::from_ascii(__FUNCTION__), -1, __VA_ARGS__)
#define log_err(module, ...) log(engine::logger_item_t::level_t::error, engine::logger_item_t::module_t::module, false, engine::ustring_t::from_ascii(__FILE__), __LINE__, engine::ustring_t::from_ascii(__FUNCTION__), -1, __VA_ARGS__)
#define log_err_raport(module, ...) log(engine::logger_item_t::level_t::error, engine::logger_item_t::module_t::module, true, engine::ustring_t::from_ascii(__FILE__), __LINE__, engine::ustring_t::from_ascii(__FUNCTION__), -1, __VA_ARGS__)

#endif