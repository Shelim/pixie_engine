#ifndef ENGINE_COMMON_LOGGER_HPP
#define ENGINE_COMMON_LOGGER_HPP
#pragma once

#include <pugixml.hpp>
#include <enum_flags.h>
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
#include "common/ustring.hpp"
#include "common/platform.hpp"
#include "common/execution_info.hpp"
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
	
	namespace logger_output
	{
		class providers_t;
	}

	class logger_t final
	{
	public:

		class item_t final
		{
		public:
			enum class level_t
			{
				task,
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

			item_t(level_t level, const ustring_t & message, const ustring_t & function, bool raport, bool cease_execution, const ustring_t & file, uint32_t line, uint64_t frame, std::chrono::seconds time, std::thread::id thread) :
				level(level), message(message), function(function), file(file), line(line), frame(frame), time(time), thread(thread)
			{
				set_flag(flag_t::raport, raport);
				set_flag(flag_t::cease_execution, cease_execution);
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

			void set_level(level_t value)
			{
				level = value;
			}

		private:

			level_t level;
			ustring_t message;
			ustring_t function;
			std::bitset<static_cast<std::size_t>(flag_t::count)> flags;
			ustring_t file;
			uint32_t line;
			uint64_t frame;
			std::chrono::seconds time;
			std::thread::id thread;

			template<class archive_t> void serialize(archive_t & ar)
			{
				ar(CEREAL_NVP(level), CEREAL_NVP(message), CEREAL_NVP(function), CEREAL_NVP(file), CEREAL_NVP(line), CEREAL_NVP(frame), CEREAL_NVP(time), CEREAL_NVP(thread));
			}
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

		item_id_t task_end(item_id_t item, bool success = true, bool raport = false)
		{
			std::lock_guard<std::recursive_mutex> guard(mutex);

			if (item >= 0 || item < items.size())
			{
				if (items[item].get_level() == item_t::level_t::task)
				{
					items[item].set_level(success ? item_t::level_t::task_done : item_t::level_t::task_failed);
					if (raport)
						items[item].set_flag(item_t::flag_t::raport, true);

					item_changed(item);
				}
			}

			return item;
		}

		template<class ...Args> item_id_t log(item_t::level_t level, bool raport, bool cease_execution, const ustring_t & file, uint32_t line, const ustring_t & function, const ustring_t & message, Args... args)
		{
			size_t item_id;
			{
				std::lock_guard<std::recursive_mutex> guard(mutex);

				items.emplace_back(item_t(level, format_string(message, args...), function, raport, cease_execution, file, line, frame_notifier->get_frame(), std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - time_start), std::this_thread::get_id()));

				item_id = items.size() - 1;
				item_changed(item_id);
			}

			if(items[item_id].is_flag(item_t::flag_t::cease_execution))
			{
				platform_fatal();
			}
			return item_id;
		}

		logger_t(std::shared_ptr<engine::platform_t> platform, std::shared_ptr<engine::execution_info_t> execution_info, std::unique_ptr<engine::logger_frame_notifier_t> frame_notifier);
		~logger_t();

		logger_t(logger_t const&) = delete;
		logger_t(logger_t &&) = delete;
		logger_t& operator=(logger_t const&) = delete;

		friend class platform_t;

		friend class cereal::access;

		void set_execution_info(engine::execution_info_t::key_t key, ustring_t value, engine::execution_info_t::item_t::status_t status)
		{
			std::lock_guard<std::recursive_mutex> guard(mutex);
			execution_info->set_info(key, value, status);
			item_changed(-1);
		}

		void set_execution_info(engine::execution_info_t::key_t key, ustring_t value)
		{
			std::lock_guard<std::recursive_mutex> guard(mutex);
			execution_info->set_info(key, value);
			item_changed(-1);
		}

		void completed_execution();

		class snapshot_t
		{

		public:

			template<class archive_t> void serialize(archive_t & ar)
			{
				ar(cereal::make_nvp("execution_info", info), CEREAL_NVP(items));
			}

		private:

			friend class logger_t;

			execution_info_t::vals_t info;
			logger_t::items_t items;
		};

		void query_snapshot(snapshot_t * output)
		{
			std::lock_guard<std::recursive_mutex> guard(mutex);

			output->info = execution_info->get_val_for_save();
			output->items = items;
		}

		const items_t & get_items()
		{
			return items;
		}

		void set_output_providers(std::weak_ptr<engine::logger_output::providers_t> output_providers)
		{
			this->output_providers = output_providers;
		}

	private:
		
		void item_changed(item_id_t item_id);

		std::weak_ptr<engine::logger_output::providers_t> output_providers;
		
		std::shared_ptr<engine::platform_t> platform;
		std::unique_ptr<engine::logger_frame_notifier_t> frame_notifier;
		items_t items;

		std::shared_ptr<engine::execution_info_t> execution_info;

		void platform_fatal();

		std::chrono::system_clock::time_point time_start;

		mutable std::recursive_mutex mutex;

	};

	template <class archive_t> std::string save_minimal(archive_t const &, logger_t::item_t::level_t const & obj)
	{
		switch (obj)
		{
		case logger_t::item_t::level_t::task: return "task_interrupted";
		case logger_t::item_t::level_t::task_done: return "task_done";
		case logger_t::item_t::level_t::task_failed: return "task_failed";
		case logger_t::item_t::level_t::message: return "message";
		case logger_t::item_t::level_t::warning: return "warning";
		case logger_t::item_t::level_t::error: return "error";
		}
		return "message";
	}

	template <class archive_t> void load_minimal(archive_t const &, logger_t::item_t::level_t & obj, std::string const & value)
	{
		if (value == "task_interrupted") obj = logger_t::item_t::level_t::task;
		else if (value == "task_done") obj = logger_t::item_t::level_t::task_done;
		else if (value == "task_failed") obj = logger_t::item_t::level_t::task_failed;
		else if (value == "message") obj = logger_t::item_t::level_t::message;
		else if (value == "warning") obj = logger_t::item_t::level_t::warning;
		else if (value == "error") obj = logger_t::item_t::level_t::error;
		else obj = logger_t::item_t::level_t::message;
	}

	template <class archive_t> std::string save_minimal(archive_t const &, std::thread::id const & obj)
	{
		std::stringstream ss;
		ss << obj;
		return ss.str();
	}

	template <class archive_t> void load_minimal(archive_t const &, std::thread::id & obj, std::string const & value)
	{
		// Deliberately do nothing. We can't reasonably load thread id
	}

	template <class archive_t> std::string save_minimal(archive_t const &, std::chrono::seconds const & obj)
	{
		return engine::to_string(obj).to_utf8();
	}

	template <class archive_t> void load_minimal(archive_t const &, std::chrono::seconds & obj, std::string const & value)
	{
		obj = engine::from_string<std::chrono::seconds>(engine::ustring_t::from_utf8(value.c_str()));
	}
}

#define p_msg(...) log(engine::logger_t::item_t::level_t::message, false, false, _U(__FILE__), __LINE__, engine::ustring_t::from_ascii(__FUNCTION__), __VA_ARGS__)
#define p_task_start(...) log(engine::logger_t::item_t::level_t::task, false, false, _U(__FILE__), __LINE__, engine::ustring_t::from_ascii(__FUNCTION__), __VA_ARGS__)
#define p_task_done(task_id) task_end(task_id, true, false)
#define p_task_failed(task_id) task_end(task_id, false, false)
#define p_task_failed_raport(task_id) task_end(task_id, false, true)
#define p_warn(...) log(engine::logger_t::item_t::level_t::warning, false, false, _U(__FILE__), __LINE__, engine::ustring_t::from_ascii(__FUNCTION__), __VA_ARGS__)
#define p_warn_raport(...) log(engine::logger_t::item_t::level_t::warning, true, false, _U(__FILE__), __LINE__, engine::ustring_t::from_ascii(__FUNCTION__), __VA_ARGS__)
#define p_err(...) log(engine::logger_t::item_t::level_t::error, false, false, _U(__FILE__), __LINE__, engine::ustring_t::from_ascii(__FUNCTION__), __VA_ARGS__)
#define p_err_raport(...) log(engine::logger_t::item_t::level_t::error, true, false, _U(__FILE__), __LINE__, engine::ustring_t::from_ascii(__FUNCTION__), __VA_ARGS__)
#define p_fatal_raport(...) log(engine::logger_t::item_t::level_t::error, true, true, _U(__FILE__), __LINE__, engine::ustring_t::from_ascii(__FUNCTION__), __VA_ARGS__)

#endif