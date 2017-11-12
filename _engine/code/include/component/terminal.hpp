#ifndef ENGINE_COMPONENT_TERMINAL_HPP
#define ENGINE_COMPONENT_TERMINAL_HPP
#pragma once

#include <memory>
#include "utility/text/ustring.hpp"

namespace engine
{

	class terminal_t
	{

	public:

		terminal_t()
		{

		}

		virtual ~terminal_t()
		{

		}

		enum class terminal_color_t
		{
#define ENGINE_TERMINAL_COLOR_STD(color) color,
#include "std/terminal_std.hpp"
			count
		};


		class instance_t
		{
			
		public:

			virtual ~instance_t()
			{

			}

			virtual bool is_closed() 
			{
				return false;
			}
			virtual void write(const ustring_t & text, terminal_t::terminal_color_t foreground, terminal_t::terminal_color_t background) = 0;
			virtual void write_new_line() = 0;

		private:

		};

		typedef std::function<void(instance_t*)> closing_callback_t;

		virtual std::shared_ptr<instance_t> open(terminal_t::terminal_color_t foreground, terminal_t::terminal_color_t background, closing_callback_t on_closing = [](instance_t*){}) = 0;

	private:

	};

}

#include "component/terminal/dummy.hpp"
#include "component/terminal/real.hpp"

#endif