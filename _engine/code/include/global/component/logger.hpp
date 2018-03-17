#ifndef ENGINE_COMPONENT_LOGGER_HPP
#define ENGINE_COMPONENT_LOGGER_HPP
#pragma once

#include <memory>
#include "utility/text/ustring.hpp"
#include "global/core/console/msg/logger.hpp"

namespace engine
{

	class logger_t
	{

	public:

		typedef std::size_t item_id_t;

		template<class ...Args> item_id_t log(engine::console::logger_item_t::kind_t kind, engine::app_t::kind_t app, engine::app_t::instance_id_t instance_id, engine::console::logger_item_t::source_t source, const ustring_t & file, uint32_t line, const ustring_t & function, std::size_t link, const ustring_t & message, Args... args)
		{
			return log_local(kind, app, instance_id, source, file, line, function, link, format_string(message, args...));
		}

		logger_t();
		virtual ~logger_t();
		virtual const engine::console::logger_items_t & get_cache() const = 0;

	private:
		
		virtual item_id_t log_local(engine::console::logger_item_t::kind_t kind, engine::app_t::kind_t app, engine::app_t::instance_id_t instance_id, engine::console::logger_item_t::source_t source, const ustring_t & file, uint32_t line, const ustring_t & function, std::size_t link, const ustring_t & message) = 0;
		
	};
}

#define log_global_msg(source, ...) log(engine::console::logger_item_t::kind_t::msg, engine::app_t::kind_t::_engine, 0, engine::console::logger_item_t::source_t::source, engine::ustring_t::from_ascii(__FILE__), __LINE__, engine::ustring_t::from_ascii(__FUNCTION__), -1, __VA_ARGS__)
#define log_global_task_start(source, ...) log(engine::console::logger_item_t::kind_t::pending, engine::app_t::kind_t::_engine, 0, engine::console::logger_item_t::source_t::source, engine::ustring_t::from_ascii(__FILE__), __LINE__, engine::ustring_t::from_ascii(__FUNCTION__), -1, __VA_ARGS__)
#define log_global_task_done(task_id) log(engine::console::logger_item_t::kind_t::success, engine::app_t::kind_t::_engine, 0, engine::console::logger_item_t::source_t::unknown, engine::ustring_t::from_ascii(__FILE__), __LINE__, engine::ustring_t::from_ascii(__FUNCTION__), task_id, ""_u)
#define log_global_task_failed(task_id) log(engine::console::logger_item_t::kind_t::failure, engine::app_t::kind_t::_engine, 0, engine::console::logger_item_t::source_t::unknown, engine::ustring_t::from_ascii(__FILE__), __LINE__, engine::ustring_t::from_ascii(__FUNCTION__), task_id, ""_u)
#define log_global_warn(source, ...) log(engine::console::logger_item_t::kind_t::warning, engine::app_t::kind_t::_engine, 0, engine::console::logger_item_t::source_t::source, engine::ustring_t::from_ascii(__FILE__), __LINE__, engine::ustring_t::from_ascii(__FUNCTION__), -1, __VA_ARGS__)
#define log_global_err(source, ...) log(engine::console::logger_item_t::kind_t::error, engine::app_t::kind_t::_engine, 0, engine::console::logger_item_t::source_t::source, engine::ustring_t::from_ascii(__FILE__), __LINE__, engine::ustring_t::from_ascii(__FUNCTION__), -1, __VA_ARGS__)

#define log_msg(app, instance, source, ...) log(engine::console::logger_item_t::kind_t::msg, app, instance, engine::console::logger_item_t::source_t::source, engine::ustring_t::from_ascii(__FILE__), __LINE__, engine::ustring_t::from_ascii(__FUNCTION__), -1, __VA_ARGS__)
#define log_task_start(app, instance, source, ...) log(engine::console::logger_item_t::kind_t::pending, app, instance, engine::console::logger_item_t::source_t::source, engine::ustring_t::from_ascii(__FILE__), __LINE__, engine::ustring_t::from_ascii(__FUNCTION__), -1, __VA_ARGS__)
#define log_task_done(task_id) log(engine::console::logger_item_t::kind_t::success, engine::app_t::kind_t::_engine, 0, engine::console::logger_item_t::source_t::unknown, engine::ustring_t::from_ascii(__FILE__), __LINE__, engine::ustring_t::from_ascii(__FUNCTION__), task_id, ""_u)
#define log_task_failed(task_id) log(engine::console::logger_item_t::kind_t::failure, engine::app_t::kind_t::_engine, 0, engine::console::logger_item_t::source_t::unknown, engine::ustring_t::from_ascii(__FILE__), __LINE__, engine::ustring_t::from_ascii(__FUNCTION__), task_id, ""_u)
#define log_warn(app, instance, source, ...) log(engine::console::logger_item_t::kind_t::warning, app, instance, engine::console::logger_item_t::source_t::source, engine::ustring_t::from_ascii(__FILE__), __LINE__, engine::ustring_t::from_ascii(__FUNCTION__), -1, __VA_ARGS__)
#define log_err(app, instance, source, ...) log(engine::console::logger_item_t::kind_t::error, app, instance, engine::console::logger_item_t::source_t::source, engine::ustring_t::from_ascii(__FILE__), __LINE__, engine::ustring_t::from_ascii(__FUNCTION__), -1, __VA_ARGS__)

#include "global/component/logger/dummy.hpp"
#include "global/component/logger/mockup.hpp"
#include "global/component/logger/real.hpp"

#endif