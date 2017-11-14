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

		terminal_t();
		virtual ~terminal_t();

		enum class color_t
		{
#define ENGINE_TERMINAL_COLOR_DEF(color) color,
#include "def/terminal.def"
			count
		};

		class instance_t
		{
			
		public:

			virtual ~instance_t();
			virtual bool is_closed() ;

			virtual void write(const ustring_t & text, terminal_t::color_t foreground) = 0;
			virtual void write(const ustring_t & text, terminal_t::color_t foreground, terminal_t::color_t background) = 0;
			virtual void write_new_line() = 0;

		private:

		};

		typedef std::function<void(instance_t*)> closing_callback_t;

		virtual std::shared_ptr<instance_t> open(const ustring_t & name, terminal_t::color_t background, closing_callback_t on_closing = [](instance_t*){}) = 0;

	private:

	};

#define STRINGIFY_ENUM_TYPE terminal_t::color_t
#define ENGINE_TERMINAL_COLOR_DEF STRINGIFY_DEF_NAME
#define STRINGIFY_DEF_INCLUDE "def/terminal.def"
#include "core/utility/stringify_def.hpp"

}

#include "component/terminal/dummy.hpp"
#include "component/terminal/real.hpp"

#endif