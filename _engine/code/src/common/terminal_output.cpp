#include "component/terminal_writer.hpp"
#include "platform/terminal.hpp"

const engine::id_t engine::parser::token_terminal_escape_t::id = engine::make_id_t<'t', 'e', 's', 'c'>::value;
const engine::id_t engine::parser::token_terminal_eof_t::id = engine::make_id_t<'t', 'e', 'o', 'f'>::value;
const engine::id_t engine::parser::token_terminal_format_t::id = engine::make_id_t<'t', 'f', 'r', 'm'>::value;

engine::terminal_writer_real_t::terminal_writer_real_t(std::shared_ptr<messenger_t> messenger, std::unique_ptr<settings_t<terminal_writer_colors_t>> terminal_writer_colors) : current_window_state(window_state_t::close), terminal_writer_colors(std::move(terminal_writer_colors)), messenger(messenger), callbacks_container(messenger, this)
{
	update_window(window_state_t::close);

	callbacks_container.attach(msg_config_updated_t::type, [this](msg_base_t * msg) { on_config_update(msg); });
}

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
		if (cfg_updated_msg->get_item() == config_t::item_t::cfg_has_terminal)
		{
			config_t * config = cfg_updated_msg->get_config();
			if (config->get_cfg_has_terminal(manifest_app_t::get_local_app()))
				update_window(window_state_t::open);
			else
				update_window(window_state_t::close);
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