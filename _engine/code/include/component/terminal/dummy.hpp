#ifndef ENGINE_COMPONENT_TERMINAL_DUMMY_HPP
#define ENGINE_COMPONENT_TERMINAL_DUMMY_HPP
#pragma once

#include "component/terminal.hpp"


namespace engine
{

	class terminal_dummy_t : public terminal_t
	{

	public:


		class instance_dummy_t : public terminal_t::instance_t
		{
			
		public:

			bool is_closed()  final;
			void write(const ustring_t & text, terminal_t::color_t foreground) final;
			void write(const ustring_t & text, terminal_t::color_t foreground, terminal_t::color_t background) final;
			void write_new_line() final;

		private:

		};

		std::shared_ptr<instance_t> open(const ustring_t & name, terminal_t::color_t background, closing_callback_t on_closing = [](instance_t*){}) final;

	};
}


#endif