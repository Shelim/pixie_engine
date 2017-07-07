#include "component/terminal_output.hpp"
#include "platform/terminal.hpp"

void engine::terminal_output_real_t::write(const richtext_t & richtext)
{
	std::lock_guard<std::recursive_mutex> guard(write_mutex);
	
	for (const auto & iter : richtext.get())
	{
#define ENGINE_RICHTEXT_TAG_STD(richtext_type) if (iter.is_flag(richtext_t::flag_t::richtext_type)) { platform::output_terminal_text(iter.get_text(), terminal_output_colors->get()->foreground_##richtext_type(), terminal_output_colors->get()->background_##richtext_type()); continue; }
#include "std/richtext_std.hpp"

		platform::output_terminal_text(iter.get_text(), terminal_output_colors->get()->foreground_default(), terminal_output_colors->get()->background_default());
	}

	platform::output_terminal_new_line();
}

void engine::terminal_output_real_t::update_window(window_state_t next_window_state)
{
	std::lock_guard<std::recursive_mutex> guard(window_state_mutex);

	if (next_window_state != current_window_state)
	{
		switch (next_window_state)
		{
		case window_state_t::open: platform::open_terminal(terminal_output_colors->get()->foreground_default(), terminal_output_colors->get()->background_default()); break;
		case window_state_t::close: platform::close_terminal(); break;
		}

		current_window_state = next_window_state;
	}
}