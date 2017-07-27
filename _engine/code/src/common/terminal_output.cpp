#include "component/terminal_writer.hpp"
#include "platform/terminal.hpp"


void engine::terminal_writer_real_t::write(const terminal_writer_string_t & terminal_string)
{
	std::lock_guard<std::recursive_mutex> guard(write_mutex);

	buffer.push_back(terminal_string);
	write_local(terminal_string);
}

void engine::terminal_writer_real_t::on_config_update(msg_base_t * msg)
{
	if (msg->get_type() == msg_config_updated_t::type)
	{
		msg_config_updated_t* cfg_updated_msg = static_cast<msg_config_updated_t*>(msg);
		if (cfg_updated_msg->get_item() == config_t::item_t::game_has_console)
		{
			std::shared_ptr<config_t> config = cfg_updated_msg->get_config().lock();
			if (config)
			{
				if (config->get_game_has_console())
					update_window(window_state_t::open);
				else
					update_window(window_state_t::close);
			}
		}
	}
}

void engine::terminal_writer_real_t::write_local(const terminal_writer_string_t & terminal_string)
{
	std::lock_guard<std::recursive_mutex> guard(write_mutex);

	engine::parser::resolver_output_t terminal_writer;

	resolve(terminal_string.get(), &terminal_writer, parser::resolver_terminal_t([this](const ustring_t & str, terminal_writer_tag_t tag) {

		platform::terminal_color_t fore = terminal_writer_colors->get()->foreground_default();
		platform::terminal_color_t back = terminal_writer_colors->get()->background_default();
#define ENGINE_TERMINAL_OUTPUT_TAG_STD(tag_type) if (tag == terminal_writer_tag_t::tag_type) { fore = terminal_writer_colors->get()->foreground_##tag_type(); back = terminal_writer_colors->get()->background_##tag_type(); }
#include "std/terminal_writer_std.hpp"

		platform::output_terminal_text(str, fore, back);

	}));

	platform::output_terminal_new_line();
}

void engine::terminal_writer_real_t::update_window(window_state_t next_window_state)
{
	std::lock_guard<std::recursive_mutex> guard(window_state_mutex);

	if (next_window_state != current_window_state)
	{
		switch (next_window_state)
		{
		case window_state_t::open:
			platform::open_terminal(terminal_writer_colors->get()->foreground_default(), terminal_writer_colors->get()->background_default());
			{
				std::lock_guard<std::recursive_mutex> guard(write_mutex);
				for (const auto & item : buffer)
					write_local(item);

				fflush(stdout);
			}
			break;
		case window_state_t::close: platform::close_terminal(); break;
		}

		current_window_state = next_window_state;
	}
}