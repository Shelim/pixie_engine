#include "component/terminal.hpp"

engine::terminal_t::terminal_t()
{

}

engine::terminal_t::~terminal_t()
{
	
}

engine::terminal_t::instance_t::~instance_t()
{

}

bool engine::terminal_t::instance_t::is_closed() 
{
	return false;
}

bool engine::terminal_dummy_t::instance_dummy_t::is_closed()
{
	return false;
}
void engine::terminal_dummy_t::instance_dummy_t::write(const ustring_t & text, terminal_t::color_t foreground)
{

}
void engine::terminal_dummy_t::instance_dummy_t::write(const ustring_t & text, terminal_t::color_t foreground, terminal_t::color_t background)
{

}
void engine::terminal_dummy_t::instance_dummy_t::write_new_line()
{
	
}

std::shared_ptr<engine::terminal_t::instance_t> engine::terminal_dummy_t::open(const ustring_t & name, terminal_t::color_t background, closing_callback_t on_closing)
{
	return std::make_shared<instance_dummy_t>();
}

engine::terminal_provider_base_t::~terminal_provider_base_t()
{

}

engine::terminal_real_t::terminal_real_t(std::unique_ptr<holder_t<terminal_t> > terminal_provider) : terminal_provider(std::move(terminal_provider))
{
	
}

std::shared_ptr<engine::terminal_t::instance_t> engine::terminal_real_t::open(const ustring_t & name, terminal_t::color_t background, closing_callback_t on_closing)
{
	return terminal_provider->get_provider()->open(name, background, on_closing);
}

/*

#include "component/terminal.hpp"
#include "wrapper/terminal_writer.hpp"
#include "utility/platform/terminal.hpp"
#include "component/config/msg_config_updated.hpp"

const engine::id_t engine::parser::token_terminal_escape_t::id = engine::make_id("tesc");
const engine::id_t engine::parser::token_terminal_eof_t::id = engine::make_id("teof");
const engine::id_t engine::parser::token_terminal_format_t::id = engine::make_id("tfrm");

engine::terminal_writer_t::terminal_writer_t(std::shared_ptr<terminal_t> terminal, std::unique_ptr<settings_t<terminal_writer_colors_t>> terminal_writer_colors, std::unique_ptr<settings_t<terminal_writer_output_t>> terminal_writer_output) : terminal(terminal), terminal_writer_colors(std::move(terminal_writer_colors)), terminal_writer_output(std::move(terminal_writer_output)), messenger(messenger)
{
}

void engine::terminal_writer_t::write(writer_t writer, const terminal_writer_string_t & terminal_string)
{
	std::lock_guard<std::recursive_mutex> guard(write_mutex);

	buffer.push_back(terminal_string);
	write_local(writer, terminal_string);
}

engine::terminal_real_t::terminal_real_t(std::shared_ptr<platform::terminal_t> terminal, std::shared_ptr<messenger_t> messenger, std::unique_ptr<settings_t<terminal_colors_t>> terminal_colors) : terminal(terminal), callbacks_container(messenger, this), terminal_colors(std::move(terminal_colors))
{
	for (auto i = 0; i < value_of(instance_t::count); i++)
	{
		terminals_metadata[i].set_colors(terminal_colors->get()->foreground_default(), terminal_colors->get()->background_default());
	}

	callbacks_container.attach(msg_config_updated_t::type, [this](msg_base_t * msg) { on_config_update(msg); });
}

engine::terminal_real_t::~terminal_real_t()
{

}

void engine::terminal_real_t::update_window(instance_t instance, window_state_t next_window_state)
{
	if (next_window_state == window_state_t::open && !terminals[value_of(instance)])
		terminals[value_of(instance)] = terminal->open_terminal(terminals_metadata[value_of(instance)].get_foreground(), terminals_metadata[value_of(instance)].get_background());

	else if (next_window_state == window_state_t::close && terminals[value_of(instance)])
		terminals[value_of(instance)].reset();
}
void engine::terminal_real_t::set_terminal_colors(instance_t instance, platform::terminal_t::color_t foreground, platform::terminal_t::color_t background)
{
	if (terminals[value_of(instance)])
		terminals[value_of(instance)]->set_terminal_colors(foreground, background);

	terminals_metadata[value_of(instance)].set_colors(foreground, background);
}
void engine::terminal_real_t::output_terminal_text(instance_t instance, const ustring_t & text, platform::terminal_t::color_t foreground, platform::terminal_t::color_t background)
{
	terminals[value_of(instance)]->output_terminal_text(text, foreground, background);
}
void engine::terminal_real_t::output_terminal_new_line(instance_t instance)
{
	terminals[value_of(instance)]->output_terminal_new_line();
}

void engine::terminal_real_t::on_config_update(msg_base_t * msg)
{
	if (msg->get_type() == msg_config_updated_t::type)
	{
		msg_config_updated_t* cfg_updated_msg = static_cast<msg_config_updated_t*>(msg);
		config_t * config = cfg_updated_msg->get_config();
		
#define ENGINE_TERMINAL_STD(terminal) if (cfg_updated_msg->get_item() == config_t::item_t::cfg_has_##terminal##_terminal) { if (config->get_cfg_has_##terminal##_terminal()) update_window(instance_t::terminal, window_state_t::open); else update_window(instance_t::terminal, window_state_t::close); }
#include "def/terminal.def"
	}
}

void engine::terminal_writer_t::write_local(writer_t writer, const terminal_writer_string_t & terminal_string)
{
	std::lock_guard<std::recursive_mutex> guard(write_mutex);

	engine::parser::resolver_output_t terminal_writer;

	resolve(terminal_string.get(), &terminal_writer, parser::resolver_terminal_t([this](const ustring_t & str, terminal_writer_tag_t tag) {

		platform::terminal_t::color_t fore = terminal_writer_colors->get()->foreground_default();
		platform::terminal_t::color_t back = terminal_writer_colors->get()->background_default();
#define ENGINE_CONSOLE_OUTPUT_TAG_STD(tag_type) if (tag == terminal_writer_tag_t::tag_type) { fore = terminal_writer_colors->get()->foreground_##tag_type(); back = terminal_writer_colors->get()->background_##tag_type(); }
#include "def/terminal_writer.def"


#define ENGINE_CONSOLE_SOURCE_STD(writer) SETTINGS_TABLE_ENTRY(engine::terminal_t::instance_t, instance_for_#writer)
#include "def/terminal_writer.def"

		terminal_t::instance_t instance = static_cast<terminal_t::instance_t>(0);


#define ENGINE_CONSOLE_SOURCE_STD(writer_real) if(writer == writer_t::writer_real) instance = terminal_writer_output->get()->instance_for_##writer_real();
#include "def/terminal_writer.def"

		terminal->output_terminal_text(instance, str, fore, back);

	}));

	terminal->output_terminal_new_line();
}

*/