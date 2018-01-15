#ifndef ENGINE_COMPONENT_LOGGER_HPP
#define ENGINE_COMPONENT_LOGGER_HPP
#pragma once

#include <memory>
#include "utility/text/ustring.hpp"
#include "core/console/msg/logger.hpp"

namespace engine
{

	class logger_t
	{

	public:

		typedef std::size_t item_id_t;

		template<class ...Args> item_id_t log(engine::console::logger_item_t::kind_t kind, engine::console::logger_item_t::source_t source, const ustring_t & file, uint32_t line, const ustring_t & function, std::size_t link, const ustring_t & message, Args... args)
		{
			return log_local(kind, source, file, line, function, link, format_string(message, args...));
		}

		logger_t();
		virtual ~logger_t();
		virtual const engine::console::logger_items_t & get_cache() const = 0;

	private:
		
		virtual item_id_t log_local(engine::console::logger_item_t::kind_t kind, engine::console::logger_item_t::source_t source, const ustring_t & file, uint32_t line, const ustring_t & function, std::size_t link, const ustring_t & message) = 0;
		
	};
}

#define log_msg(source, ...) log(engine::console::logger_item_t::kind_t::msg, engine::console::logger_item_t::source_t::source, engine::ustring_t::from_ascii(__FILE__), __LINE__, engine::ustring_t::from_ascii(__FUNCTION__), -1, __VA_ARGS__)
#define log_task_start(source, ...) log(engine::console::logger_item_t::kind_t::pending, engine::console::logger_item_t::source_t::source, engine::ustring_t::from_ascii(__FILE__), __LINE__, engine::ustring_t::from_ascii(__FUNCTION__), -1, __VA_ARGS__)
#define log_task_done(task_id) log(engine::console::logger_item_t::kind_t::success, engine::console::logger_item_t::source_t::unknown, engine::ustring_t::from_ascii(__FILE__), __LINE__, engine::ustring_t::from_ascii(__FUNCTION__), task_id, ""_u)
#define log_task_failed(task_id) log(engine::console::logger_item_t::kind_t::failure, engine::console::logger_item_t::source_t::unknown, engine::ustring_t::from_ascii(__FILE__), __LINE__, engine::ustring_t::from_ascii(__FUNCTION__), task_id, ""_u)
#define log_warn(source, ...) log(engine::console::logger_item_t::kind_t::warning, engine::console::logger_item_t::source_t::source, engine::ustring_t::from_ascii(__FILE__), __LINE__, engine::ustring_t::from_ascii(__FUNCTION__), -1, __VA_ARGS__)
#define log_err(source, ...) log(engine::console::logger_item_t::kind_t::error, engine::console::logger_item_t::source_t::source, engine::ustring_t::from_ascii(__FILE__), __LINE__, engine::ustring_t::from_ascii(__FUNCTION__), -1, __VA_ARGS__)

#include "component/logger/dummy.hpp"
#include "component/logger/mockup.hpp"
#include "component/logger/real.hpp"

#endif