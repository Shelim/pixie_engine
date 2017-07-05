#ifndef ENGINE_MODEL_LOGGER_ITEM_HPP
#define ENGINE_MODEL_LOGGER_ITEM_HPP
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
#include <cereal/cereal.hpp>
#include <cereal/access.hpp>
#include <vlc/vlc.h>
#include <bitset>

namespace engine
{
	class logger_item_t final
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
			raport,					// Will be marked for raport to end user
			count
		};

		enum class module_t
		{
			unknown,
#define ENGINE_LOGGER_MODULE_STD(module) module,
#include "std/logger_module_std.hpp"
			count
		};

		friend class cereal::access;

		struct finished_t
		{

		};

		logger_item_t(finished_t) : id(-1), level(level_t::finished), link(-1)
		{

		}

		logger_item_t(std::size_t id, level_t level, module_t module, const ustring_t & message, const ustring_t & function, bool raport, const ustring_t & file, uint32_t line, uint64_t frame, std::chrono::seconds time, std::thread::id thread, std::size_t link = -1) :
			id(id), level(level), module(module), message(message), function(function), file_raw(file), line(line), frame(frame), time(time), thread(thread), link(link)
		{
			parse_file();
			flags.set_flag(flag_t::raport, raport);
		}
		std::size_t get_id() const
		{
			return id;
		}
		level_t get_level() const
		{
			return level;
		}
		module_t get_module() const
		{
			return module;
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

		bool is_flag(flag_t flag) const
		{
			return flags.is_flag(flag);
		}

		std::size_t get_link() const
		{
			return link;
		}

		bool is_linked() const
		{
			return link != -1;
		}

		void clear_raport_flag()
		{
			flags.set_flag(flag_t::raport, false);
		}

	private:

		void parse_file();

		std::size_t id;
		level_t level;
		module_t module;
		ustring_t message;
		ustring_t function;
		flags_t<flag_t> flags;
		ustring_t file;
		ustring_t file_raw;
		uint32_t line;
		uint64_t frame;
		std::chrono::seconds time;
		std::thread::id thread;
		std::size_t link;

	};

	typedef std::vector<logger_item_t> logger_items_t;
}

#endif