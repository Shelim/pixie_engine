#ifndef ENGINE_COMPONENT_PLATFORM_INTERNAL_HPP
#define ENGINE_COMPONENT_PLATFORM_INTERNAL_HPP
#pragma once

#include "utility/text/richtext.hpp"
#include "utility/vfs/filesystem.hpp"
#include "utility/vfs/virtual_path.hpp"
#include <mutex>

namespace engine
{
	

	class platform_internal_t
	{

	public:

		platform_internal_t() : current_console_window_state(console_window_state_t::close)
		{

		}

		enum class console_window_state_t
		{
			open,
			close
		};

		virtual ~platform_internal_t()
		{

		}
		
		void console_update_window(console_window_state_t next_console_window_state)
		{
			std::lock_guard<std::recursive_mutex> guard(console_window_state);

			if (next_console_window_state != current_console_window_state)
			{
				switch (next_console_window_state)
				{
				case console_window_state_t::open: open_console(); break;
				case console_window_state_t::close: close_console(); break;
				}

				current_console_window_state = next_console_window_state;
			}
		}

		virtual void console_write(const richtext_t & richtext) = 0;
		virtual std::filesystem::path get_self_path() = 0;
		virtual std::filesystem::path get_save_path(const virtual_path_t & path);

	private:

		mutable std::recursive_mutex console_window_state;

		console_window_state_t current_console_window_state;

		virtual void open_console() = 0;
		virtual void close_console() = 0;
	};
}

#include "component/platform_internal/dummy.hpp"
#include "component/platform_internal/windows.hpp"

#endif