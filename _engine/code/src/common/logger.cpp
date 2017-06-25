#include <SDL_version.h>
#include <SDL_revision.h>
#include <SDL_cpuinfo.h>
#include <ctime>
#include <cstdio>
#include <sstream>
#include <fstream>
#include "pugixml.hpp"
#include "common/logger_container.hpp"
#include "common/platform.hpp"
#include "common/platform.hpp"
#include "common/config.hpp"
#include "common/data/output.hpp"
#include "common/data/database_providers.hpp"
#include "common/data/database_items.hpp"
#include "common/environment_info.hpp"
#include "common/logger_output/providers.hpp"

void engine::logger_t::item_t::parse_file()
{
	static std::string engine_tag = "_engine/src/";
	static std::string game_tag = XSTR(PIXIE_APP_UNIX_NAME) "/src/";

	std::string file_tmp = file_raw.get_cstring();
	std::replace(file_tmp.begin(), file_tmp.end(), '\\', '/');

	std::size_t index = file_tmp.find(engine_tag);
	if (index != std::string::npos)
		file_tmp = file_tmp.substr(index);

	index = file_tmp.find(game_tag);
	if (index != std::string::npos)
		file_tmp = file_tmp.substr(index);
	
	file = ustring_t::from_utf8(file_tmp.c_str());
}

void engine::logger_output::provider_text_base_t::line_constructor_t::query_richtext_flags(logger_t::item_t::level_t level, richtext_t::flag_t & normal, richtext_t::flag_t & meta)
{
	switch (level)
	{
	case logger_t::item_t::level_t::error: normal = richtext_t::flag_t::error; meta = richtext_t::flag_t::error_meta; break;
	case logger_t::item_t::level_t::finished: normal = richtext_t::flag_t::success; meta = richtext_t::flag_t::success_meta; break;
	case logger_t::item_t::level_t::message: normal = richtext_t::flag_t::note; meta = richtext_t::flag_t::note_meta; break;
	case logger_t::item_t::level_t::task_done: normal = richtext_t::flag_t::success; meta = richtext_t::flag_t::success_meta; break;
	case logger_t::item_t::level_t::task_failed: normal = richtext_t::flag_t::error; meta = richtext_t::flag_t::error_meta; break;
	case logger_t::item_t::level_t::task_started: normal = richtext_t::flag_t::pending; meta = richtext_t::flag_t::pending_meta; break;
	case logger_t::item_t::level_t::warning: normal = richtext_t::flag_t::warning; meta = richtext_t::flag_t::warning_meta; break;
	}
}

void engine::logger_output::provider_text_base_t::line_constructor_t::populate(const item_output_elements_t & item_output_elements)
{
	for (auto & iter : item_output_elements)
	{
		switch (iter)
		{
		case item_output_element_t::output_sep_column: column += column_width; line.append(engine::logger_output::provider_text_base_t::generate_padding(line.get_len(), column)); break;
		case item_output_element_t::output_sep_arrow: line.clear_flags(); line.set_flag(meta); line.append(_U(" <- ")); break;
		case item_output_element_t::output_sep_comma: line.clear_flags(); line.set_flag(meta); line.append(_U(", ")); break;

		case item_output_element_t::output_id: line.clear_flags(); line.set_flag(meta); line.append(_U("# ")); line.append(to_string(item->get_id(), 5)); line.append(_U(" ")); break;
		case item_output_element_t::output_prompt: line.clear_flags(); line.set_flag(meta); line.append(level_to_prompt(item->get_level())); break;
		case item_output_element_t::output_message:
			line.clear_flags();
			line.set_flag(normal);
			if (item->get_link() != -1)
			{
				auto & items = owner->get_logger()->get_items();
				line.append(items[item->get_link()].get_message());
			}
			else
				line.append(item->get_message());

			line.clear_flags();
			line.set_flag(meta);

			if (item->get_level() == logger_t::item_t::level_t::task_started)
				line.append(_U("..."));
			else if (item->get_level() == logger_t::item_t::level_t::task_done)
				line.append(_U("... "));
			else if (item->get_level() == logger_t::item_t::level_t::task_failed)
				line.append(_U("... "));

			line.clear_flags();
			line.set_flag(normal);

			if (item->get_level() == logger_t::item_t::level_t::task_done)
				line.append(_U("DONE!"));
			else if (item->get_level() == logger_t::item_t::level_t::task_failed)
				line.append(_U("FAILED!!!"));

			break;
		case item_output_element_t::output_frame: line.clear_flags(); line.set_flag(meta); line.append(_U("frame: ")); line.clear_flags(); line.set_flag(normal); line.append(to_string(item->get_frame())); break;
		case item_output_element_t::output_time: line.clear_flags(); line.set_flag(meta); line.append(_U("time: ")); line.clear_flags(); line.set_flag(normal); line.append(to_string(item->get_time())); break;
		case item_output_element_t::output_thread: line.clear_flags(); line.set_flag(meta); line.append(_U("thread: ")); line.clear_flags(); line.set_flag(normal); line.append(to_string(item->get_thread())); break;
		case item_output_element_t::output_func: line.clear_flags(); line.set_flag(meta); line.append(_U("func: ")); line.clear_flags(); line.set_flag(normal); line.append(item->get_function()); line.append(_U("()")); break;
		case item_output_element_t::output_file: line.clear_flags(); line.set_flag(meta); line.append(_U("at: '")); line.clear_flags(); line.set_flag(normal); line.append(item->get_file()); line.clear_flags(); line.set_flag(meta); line.append(_U("'")); break;
		case item_output_element_t::output_line: line.clear_flags(); line.set_flag(meta); line.append(_U("on line: ")); line.clear_flags(); line.set_flag(normal); line.append(to_string(item->get_line())); break;
		}
	}
}

void engine::logger_output::provider_text_base_t::process_item(const logger_t::item_t & item)
{
	change_section(section_t::item);

	line_constructor.start(item);

	line_constructor.populate(item_output_elements);
	
	output_line(line_constructor.get_line());
}

void engine::logger_output::provider_text_base_t::process_environment_info(std::shared_ptr<environment_info_t> environment_info)
{
	change_section(section_t::environment_info);

#define GAME_ENVIRONMENT_INFO_STD(key, name) { richtext_t line; const environment_info_t::item_t & info = environment_info->get(environment_info_t::key_t::key); line.clear_flags(); line.set_flag(richtext_t::flag_t::info_meta); line.append(_U("> ")); line.clear_flags(); line.set_flag(richtext_t::flag_t::info_meta); line.append(info.get_name()); line.clear_flags(); line.set_flag(richtext_t::flag_t::info_meta); line.append(_U(" ")); line.append(generate_padding(line.get_raw(), 30, '-')); line.append(_U("> ")); line.clear_flags(); line.set_flag(richtext_t::flag_t::info); line.append(info.get_value()); output_line(line); }
#include "common/std/environment_info_std.hpp"
}

engine::logger_output::provider_data_output_t::provider_data_output_t(std::shared_ptr<engine::data::database_providers_t> database_providers, std::shared_ptr<logger_t> logger, std::shared_ptr<environment_info_t> environment_info) : provider_text_base_t(logger, { item_output_element_t::output_id, item_output_element_t::output_prompt, item_output_element_t::output_message, item_output_element_t::output_sep_column, item_output_element_t::output_frame, item_output_element_t::output_sep_comma, item_output_element_t::output_time, item_output_element_t::output_sep_comma, item_output_element_t::output_thread, item_output_element_t::output_sep_comma, item_output_element_t::output_func, item_output_element_t::output_sep_column, item_output_element_t::output_file, item_output_element_t::output_sep_comma, item_output_element_t::output_line })
{

#define GAME_LOGGER_VIRTUAL_PATH_STD(path) output = database_providers->get_output(virtual_path_t(path, virtual_path_t::type_t::log));
#include "common/std/virtual_path_std.hpp"

}

uint64_t engine::logger_frame_notifier_t::get_frame() const
{
	return 0;
//	return engine::renderer::get_frame();
}

engine::logger_t::logger_t(std::shared_ptr<engine::platform_t> platform, std::shared_ptr<environment_info_t> environment_info, std::unique_ptr<engine::logger_frame_notifier_t> frame_notifier) :
	platform(platform), environment_info(environment_info), frame_notifier(std::move(frame_notifier)), time_start(std::chrono::system_clock::now())
{
	platform->set_logger(this);
}

engine::logger_t::~logger_t()
{

}

void engine::logger_container_t::update()
{
	providers->update();
}