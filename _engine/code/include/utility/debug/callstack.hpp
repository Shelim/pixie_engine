#ifndef ENGINE_COMMON_UTILITY_DEBUG_CALLSTACK_HPP
#define ENGINE_COMMON_UTILITY_DEBUG_CALLSTACK_HPP

#include "utility/text/ustring.hpp"
#include "utility/pattern/flags.hpp"
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
	 * @see engine::platform::dump_callstack(), callstack_t::item_t
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
			 * If you wish to add more data later on use @ref add_debug_info
			 * 
			 * @param[in] address Location in executable file for given stack item. Usually you pass content of @c ProgramCounter register
			 * @param[in] module Module to be used in given stack item. Usually you pass here @c argv[0]
			 * @param[in] file Filename to be used in given stack item. Usually you pass @c canonize_debug_filename(__FILE__)
			 * @param[in] line Line to be used in given stack item. Usually you pass @c __LINE__
			 * @param[in] function Function name to be used in given stack frame. Usually you pass @c __FUNCTION__
			 */
			item_t(intptr_t address, ustring_t module = ""_u, ustring_t file = ""_u, int line = 0, ustring_t function = ""_u) : address(address), module(module), file(file), line(line), function(function)
			{
				if(!module.is_empty())
					flags.set_flag(flag_t::is_module_available);
				if(!file.is_empty())
					flags.set_flag(flag_t::is_file_available);
				if(line >= 0)
					flags.set_flag(flag_t::is_line_available);
				if(!function.is_empty())
					flags.set_flag(flag_t::is_function_available);
			}

			/**
			 * @brief Adds debug info for given callstack item
			 * 
			 * @param[in] module Module to be used in given stack item. Usually you pass here @c argv[0]
			 * @param[in] file Filename to be used in given stack item. Usually you pass @c canonize_debug_filename(__FILE__)
			 * @param[in] line Line to be used in given stack item. Usually you pass @c __LINE__
			 * @param[in] function Function name to be used in given stack frame. Usually you pass @c __FUNCTION__
			*/
			void add_debug_info(ustring_t module = ""_u, ustring_t file = ""_u, int line = 0, ustring_t function = ""_u)
			{
				if(!module.is_empty())
				{
					this->module = module;
					flags.set_flag(flag_t::is_module_available);
				}
				if(!file.is_empty())
				{
					this->file = file;
					flags.set_flag(flag_t::is_file_available);
				}
				if(line >= 0)
				{
					this->line = line;
					flags.set_flag(flag_t::is_line_available);
				}
				if(!function.is_empty())
				{
					this->function = function;
					flags.set_flag(flag_t::is_function_available);
				}
			}

			/**
			 * @brief Returns address of given stack item.
			 * 
			 * @note You can look up more information about this stack item in linker map file, in case callstack dump is comming from build without debugging symbols
			 * 
			 * @return Address of given stack item
			 */
			intptr_t get_address() const
			{
				return address;
			}

			/**
			 * @brief Returns true if module information is available
			 * 
			 * The information may be unavailable due to compiler not producing complete debugging symbols
			 * 
			 * @return true if module information is available
			 */
			bool is_module_available() const
			{
				return flags.is_flag(flag_t::is_module_available);
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
			 * @brief Returns true if file information is available
			 * 
			 * The information may be unavailable due to compiler not producing complete debugging symbols
			 * 
			 * @todo Currently LLVM is unable to emit file information :-(
			 * 
			 * @return true if file information is available
			 */
			bool is_file_available() const
			{
				return flags.is_flag(flag_t::is_file_available);
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
			 * @brief Returns true if line information is available
			 * 
			 * The information may be unavailable due to compiler not producing complete debugging symbols
			 * 
			 * @todo Currently LLVM is unable to emit line information :-(
			 * 
			 * @return true if line information is available
			 */
			bool is_line_available() const
			{
				return flags.is_flag(flag_t::is_line_available);
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
			 * @brief Returns true if function information is available
			 * 
			 * The information may be unavailable due to compiler not producing complete debugging symbols
			 * 
			 * @return true if function information is available
			 */
			bool is_function_available() const
			{
				return flags.is_flag(flag_t::is_function_available);
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

			enum class flag_t
			{
				is_module_available,
				is_file_available,
				is_line_available,
				is_function_available,

				count
			};

			flags_t<flag_t> flags;

		};

		typedef std::vector<item_t> items_t;

		/**
		 * @brief Create new callstack instance by moving items_t collection
		 * 
		 * @param[in] items to populate callstack
		 * @see callstack_t::item_t
		 */
		callstack_t(items_t && items) : items(std::move(items))
		{
			
		}
		
		/**
		 * @brief Gets the collection of callstack items
		 * 
		 * @return collection of callstack items
		 * @see callstack_t::item_t
		 */
		const items_t & get_items() const
		{
			return items;
		}
		
		/**
		 * @brief Returns begin of collection of callstack items
		 * 
		 * This function is provided, so you can write: C++ 11 `for(auto item : callstack)`
		 * 
		 * @return begin of collection of callstack item
		 * @see callstack_t::item_t
		 */
		items_t::const_iterator begin() const
		{
			return items.begin();
		}

		/**
		 * @brief Returns end of collection of callstack items
		 * 
		 * This function is provided, so you can write: C++ 11 `for(auto item : callstack)`
		 * 
		 * @return end of collection of callstack item
		 * @see callstack_t::item_t
		 */
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