#ifndef ENGINE_COMPONENT_PLATFORM_INTERNAL_DUMMY_HPP
#define ENGINE_COMPONENT_PLATFORM_INTERNAL_DUMMY_HPP
#pragma once

#include "component/platform_internal.hpp"

namespace engine
{

	class platform_internal_dummy_t : public platform_internal_t
	{

	public:
		
		void console_write(const richtext_t & richtext) final
		{
			// Do nothing, it is dummy.
		}

	private:

		void open_console() final
		{
			// Do nothing, it is dummy.
		}

		void close_console() final
		{
			// Do nothing, it is dummy.
		}

	};
}

#endif