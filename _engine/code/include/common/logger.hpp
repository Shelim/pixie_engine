#ifndef ENGINE_COMMON_LOGGER_HPP
#define ENGINE_COMMON_LOGGER_HPP
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
#include "common/utility/text/ustring.hpp"
#include "common/platform.hpp"
#include "common/environment_info.hpp"
#include "common/utility/container/sync_queue.hpp"
#include <cereal/cereal.hpp>
#include <cereal/access.hpp>
#include <vlc/vlc.h>
#include <bitset>

namespace engine
{

	class logger_frame_notifier_t final
	{
	public:
		uint64_t get_frame() const;

		logger_frame_notifier_t() { }
		~logger_frame_notifier_t() { }

		logger_frame_notifier_t(logger_frame_notifier_t const&) = delete;
		logger_frame_notifier_t(logger_frame_notifier_t &&) = delete;
		logger_frame_notifier_t& operator=(logger_frame_notifier_t const&) = delete;
		logger_frame_notifier_t& operator=(logger_frame_notifier_t &&) = delete;
	};

	class logger_t final
	{
	public:

		class item_t final
		{
		public:
			enum class level_t
			{
				finished,
				task_started,
				task_failed,
				task_done,
				message,
				warning,
				error
			};

			enum class flag_t
			{
				raport,					// Will be raported in case of cease execution
				cease_execution,		// Cease execution of game
				count
			};

			friend class cereal::access;
			
			struct finished_t
			{

			};
			
			item_t(finished_t) : id(-1), level(level_t::finished), link(-1)
			{

			}

			item_t(std::size_t id, level_t level, const ustring_t & message, const ustring_t & function, bool raport, bool cease_execution, const ustring_t & file, uint32_t line, uint64_t frame, std::chrono::seconds time, std::thread::id thread, std::size_t link = -1) :
				id(id), level(level), message(message), function(function), file_raw(file), line(line), frame(frame), time(time), thread(thread), link(link)
			{
				parse_file();
				set_flag(flag_t::raport, raport);
				set_flag(flag_t::cease_execution, cease_execution);
			}
			std::size_t get_id() const
			{
				return id;
			}
			level_t get_level() const
			{
				return level;
			}
			const ustring_t & get_message() const
			{
				return message;
			}
			const ustring_t & get_function() const
			{
				return function;
			}
			ustring_t get_file() const
			{
				return file;
			}
			ustring_t get_file_raw() const
			{
				return file_raw;
			}
			uint32_t get_line() const
			{
				return line;
			}
			uint64_t get_frame() const
			{
				return frame;
			}
			std::chrono::seconds get_time() const
			{
				return time;
			}
			std::thread::id get_thread() const
			{
				return thread;
			}

			void set_flag(flag_t flag, bool value)
			{
				flags.set(static_cast<std::size_t>(flag), value);
			}

			bool is_flag(flag_t flag) const
			{
				return flags.test(static_cast<std::size_t>(flag));
			}

			std::size_t get_link() const
			{
				return link;
			}
			
			bool is_linked() const
			{
				return link != -1;
			}

		private:

			void parse_file();

			std::size_t id;
			level_t level;
			ustring_t message;
			ustring_t function;
			std::bitset<static_cast<std::size_t>(flag_t::count)> flags;
			ustring_t file;
			ustring_t file_raw;
			uint32_t line;
			uint64_t frame;
			std::chrono::seconds time;
			std::thread::id thread;
			std::size_t link;

		};

		void reset_raports()
		{
			std::lock_guard<std::recursive_mutex> guard(mutex);
			
			for (auto & item : items)
			{
				item.set_flag(item_t::flag_t::raport, false);
			}
		}

		typedef size_t item_id_t;
		typedef std::vector<item_t> items_t;
		
		template<class ...Args> item_id_t log(item_t::level_t level, bool raport, bool cease_execution, const ustring_t & file, uint32_t line, const ustring_t & function, std::size_t link, const ustring_t & message, Args... args)
		{
			size_t item_id;
			{
				std::lock_guard<std::recursive_mutex> guard(mutex);

				items.emplace_back(item_t(items.size(), level, format_string(message, args...), function, raport, cease_execution, file, line, frame_notifier->get_frame(), std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - time_start), std::this_thread::get_id(), link));

				item_id = items.size() - 1;
				items_queue.push(items[item_id]);
			}

			if(items[item_id].is_flag(item_t::flag_t::cease_execution))
			{
				platform_fatal();
			}
			return item_id;
		}

		logger_t(std::shared_ptr<engine::platform_t> platform, std::shared_ptr<environment_info_t> environment_info, std::unique_ptr<engine::logger_frame_notifier_t> frame_notifier);
		~logger_t();

		logger_t(logger_t const&) = delete;
		logger_t(logger_t &&) = delete;
		logger_t& operator=(logger_t const&) = delete;

		friend class platform_t;

		friend class cereal::access;

		items_t get_items()
		{
			std::lock_guard<std::recursive_mutex> guard(mutex);
			return items;
		}

		item_t query_next_item()
		{
			return items_queue.pop();
		}

		void kill_item_queue()
		{
			items_queue.push(item_t(item_t::finished_t()));
		}

	private:
				
		std::shared_ptr<environment_info_t> environment_info;

		std::shared_ptr<engine::platform_t> platform;
		std::unique_ptr<engine::logger_frame_notifier_t> frame_notifier;
		items_t items;
		
		sync_queue_t<item_t> items_queue;

		void platform_fatal();

		std::chrono::system_clock::time_point time_start;

		mutable std::recursive_mutex mutex;

	};
}

#define p_msg(...) log(engine::logger_t::item_t::level_t::message, false, false, _U(__FILE__), __LINE__, engine::ustring_t::from_ascii(__FUNCTION__), -1, __VA_ARGS__)
#define p_task_start(...) log(engine::logger_t::item_t::level_t::task_started, false, false, _U(__FILE__), __LINE__, engine::ustring_t::from_ascii(__FUNCTION__), -1, __VA_ARGS__)
#define p_task_done(task_id) log(engine::logger_t::item_t::level_t::task_done, false, false, _U(__FILE__), __LINE__, engine::ustring_t::from_ascii(__FUNCTION__), task_id, _U(""))
#define p_task_failed(task_id) log(engine::logger_t::item_t::level_t::task_failed, false, false, _U(__FILE__), __LINE__, engine::ustring_t::from_ascii(__FUNCTION__), task_id, _U(""))
#define p_task_failed_raport(task_id) log(engine::logger_t::item_t::level_t::task_failed, true, false, _U(__FILE__), __LINE__, engine::ustring_t::from_ascii(__FUNCTION__), task_id, _U(""))
#define p_warn(...) log(engine::logger_t::item_t::level_t::warning, false, false, _U(__FILE__), __LINE__, engine::ustring_t::from_ascii(__FUNCTION__), -1, __VA_ARGS__)
#define p_warn_raport(...) log(engine::logger_t::item_t::level_t::warning, true, false, _U(__FILE__), __LINE__, engine::ustring_t::from_ascii(__FUNCTION__), -1, __VA_ARGS__)
#define p_err(...) log(engine::logger_t::item_t::level_t::error, false, false, _U(__FILE__), __LINE__, engine::ustring_t::from_ascii(__FUNCTION__), -1, __VA_ARGS__)
#define p_err_raport(...) log(engine::logger_t::item_t::level_t::error, true, false, _U(__FILE__), __LINE__, engine::ustring_t::from_ascii(__FUNCTION__), -1, __VA_ARGS__)
#define p_fatal_raport(...) log(engine::logger_t::item_t::level_t::error, true, true, _U(__FILE__), __LINE__, engine::ustring_t::from_ascii(__FUNCTION__), -1, __VA_ARGS__)

#endif