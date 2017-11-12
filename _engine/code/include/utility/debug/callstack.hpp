#ifndef ENGINE_COMMON_UTILITY_DEBUG_CALLSTACK_HPP
#define ENGINE_COMMON_UTILITY_DEBUG_CALLSTACK_HPP

#include "utility/text/ustring.hpp"
#include "utility/platform/debug.hpp"

#pragma once

namespace engine
{

	class callstack_t
	{

	public:

		class item_t
		{

		public:

			item_t(ustring_t file, std::size_t line, ustring_t function) : file(file), line(line), function(function)
			{

			}

			const ustring_t & get_file() const
			{
				return file;
			}

			std::size_t get_line() const
			{
				return line;
			}

			const ustring_t & get_function() const
			{
				return function;
			}

		private:

			ustring_t file;
			std::size_t line;
			ustring_t function;

		};

		typedef std::vector<item_t> items_t;

		callstack_t(items_t && items) : items(std::move(items))
		{
			
		}
		
		const items_t & get_items() const
		{
			return items;
		}
		
		items_t::const_iterator begin() const
		{
			return items.begin();
		}

		items_t::const_iterator end() const
		{
			return items.end();
		}

	private:
		
		friend callstack_t engine::platform::dump_callstack(std::size_t skip_top);

		callstack_t()
		{

		}

		items_t items;

	};

}

#endif