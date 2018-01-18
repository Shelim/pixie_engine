#ifndef ENGINE_COMMON_UTILITY_DEBUG_CALLSTACK_HPP
#define ENGINE_COMMON_UTILITY_DEBUG_CALLSTACK_HPP

#include "utility/text/ustring.hpp"
#include "utility/platform/debug.hpp"

#pragma once

namespace engine
{

	/**
	 * @brief Callstack information in platform-independent way
	 * 
	 * This class is usually a product of @ref engine::platform::dump_callstack() function execution.
	 * You can access each frame and recieve information on file, line and function involved.
	 * 
	 * You can access them either by using method @ref get_items()
	 * or by using C++ 11 `for(auto item : callstack)`
	 * 
	 * @note some function calls may be omitted from callstack due to compilator optimization
	 * @see engine::platform::dump_callstack()
	 */
	class callstack_t
	{

	public:

		/**
		 * @brief Single function call in callstack
		 * 
		 * @note some function calls may be omitted from callstack due to compilator optimization
		 * @see engine::platform::dump_callstack()
		 */
		class item_t
		{

		public:
			
			/**
			 * @brief Creates callstack item from given values
			 * 
			 * @param[in] address Location in executable file for given stack item. Usually you pass content of @c ProgramCounter register
			 * @param[in] module Module to be used in given stack item. Usually you pass here @c argv[0]
			 * @param[in] file Filename to be used in given stack item. Usually you pass @c canonize_debug_filename(__FILE__)
			 * @param[in] line Line to be used in given stack item. Usually you pass @c __LINE__
			 * @param[in] function Function name to be used in given stack frame. Usually you pass @c __FUNCTION__
			 */
			item_t(intptr_t address, ustring_t module, ustring_t file, std::size_t line, ustring_t function) : address(address), module(module), file(file), line(line), function(function)
			{

			}

			/**
			 * @brief Returns address of given stack item.
			 * 
			 * @note You can look up more information about this stack item in linker map file, in case callstack dump is comming from build without debugging symbols
			 * 
			 * @todo Currently LLD-LINK is unable to produce map file :-(
			 * 
			 * @return Address of given stack item
			 */
			intptr_t get_address() const
			{
				return address;
			}

			/**
			 * @brief Returns module name of given stack item.
			 * 
			 * Usually you will see here name of your executable or external shared library
			 * 
			 * @return Module name of given stack item
			 */
			const ustring_t & get_module() const
			{
				return module;
			}

			/**
			 * @brief Get source file for this stack item
			 * 
			 * Usually this will contain output of @c canonize_debug_filename(__FILE__)
			 * 
			 * @return filename
			 * @see get_line
			 */
			const ustring_t & get_file() const
			{
				return file;
			}

			/**
			 * @brief Get source line for this stack item
			 * 
			 * Usually this will contain output of @c __LINE__
			 * 
			 * @return line in file
			 * @see get_file
			 */
			std::size_t get_line() const
			{
				return line;
			}

			/**
			 * @brief Get function name for this stack item
			 * 
			 * Usually this will contain output of @c __FUNCTION__
			 * 
			 * @return function name
			 */
			const ustring_t & get_function() const
			{
				return function;
			}

		private:

			intptr_t address;
			ustring_t module;			
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