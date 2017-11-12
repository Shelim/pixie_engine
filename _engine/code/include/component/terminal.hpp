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

		enum class provider_t
		{
#define ENGINE_TERMINAL_PROVIDER_STD(provider) provider,
#include "std/terminal_std.hpp"
			any,
			count = any
		};

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

			virtual void set_terminal_colors(terminal_t::terminal_color_t foreground, terminal_t::terminal_color_t background) = 0;
			virtual void output_terminal_text(const ustring_t & text, terminal_t::terminal_color_t foreground, terminal_t::terminal_color_t background) = 0;
			virtual void output_terminal_new_line() = 0;

		};

		virtual bool is_provider_available(provider_t provider) = 0;
		virtual std::shared_ptr<instance_t> open(provider_t provider, bool fallback_to_any_if_not_available = false) = 0;

	private:

	};

}

//#include "component/terminal/dummy.hpp"
//#include "component/terminal/real.hpp"

#endif