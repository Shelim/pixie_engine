#ifndef ENGINE_CORE_CONSOLE_MSG_LOGGER_HPP
#define ENGINE_CORE_CONSOLE_MSG_LOGGER_HPP
#pragma once

#include <chrono>
#include "utility/text/ustring.hpp"
#include "utility/pattern/flags.hpp"
#include "utility/debug/callstack.hpp"
#include "utility/platform/debug.hpp"
#include "core/messenger/msg/console.hpp"

namespace engine
{
    namespace console
    {

		class logger_item_t final : public messenger::msg_console_t::actual_t
		{
		public:

			logger_item_t(std::size_t id, kind_t kind, source_t source, const ustring_t & message, const ustring_t & function, const ustring_t & file, uint32_t line, uint64_t frame, std::chrono::seconds time, std::thread::id thread, std::size_t link = -1) :
				actual_t(type_t::logger, source, kind), id(id), message(message), function(function), file_raw(file), line(line), frame(frame), time(time), thread(thread), link(link)
			{
				this->file = platform::canonize_debug_filename(file_raw);
			}
			std::size_t get_id() const
			{
				return id;
			}
			ustring_t get_source_as_string() const
			{
				return to_string(get_source());
			}
			const ustring_t & get_message() const
			{
				return message;
			}
			const ustring_t & get_function() const
			{
				return function;
			}
			const ustring_t & get_file() const
			{
				return file;
			}
			const ustring_t & get_file_raw() const
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

			std::size_t get_link() const
			{
				return link;
			}

			bool is_linked() const
			{
				return link != -1;
			}

		private:

			std::size_t id; // 1 for formatting
			// Kind is 2 for formatting
			ustring_t message; // 3 for formatting
			ustring_t function; // 4 for formatting
			ustring_t file; // 5 for formatting
			ustring_t file_raw;
			uint32_t line; // 6 for formatting
			uint64_t frame; // 7 for formatting
			std::chrono::seconds time; // 8 for formatting
			std::thread::id thread; // 9 for formatting
			std::size_t link; // 10 for formatting
		};

		typedef std::vector<logger_item_t> logger_items_t;
	}

}

#endif