#include <SDL_version.h>
#include <SDL_revision.h>
#include <SDL_cpuinfo.h>
#include <ctime>
#include <cstdio>
#include <sstream>
#include <fstream>
#include "pugixml.hpp"
#include "component/logger.hpp"
#include "utility/vfs/virtual_path.hpp"
#include "utility/platform/compilation.hpp"
#include "platform/debug.hpp"
#include "platform/terminal.hpp"

engine::logger_output_provider_file_t::logger_output_provider_file_t(std::shared_ptr<log_file_writer_t> log_file_writer, std::unique_ptr<settings_t<logger_output_t>> settings) : log_file_writer(log_file_writer), settings(std::move(settings))
{
	start_text = this->settings->get()->file_start();
	end_text = this->settings->get()->file_end();

#define ENGINE_LOGGER_LEVEL_STD(type_id) formattable_string[static_cast<std::underlying_type<logger_item_t::level_t>::type>(logger_item_t::level_t::type_id)] = this->settings->get()->format_file_##type_id ();
#include "std/logger_std.hpp"
}

engine::logger_output_provider_file_t::~logger_output_provider_file_t()
{

}

void engine::logger_output_provider_file_t::output_start() const
{
	log_file_writer->write(start_text);
}


void engine::logger_output_provider_file_t::output(const logger_item_t & item) const
{
	ustring_t raw = format_string(formattable_string[static_cast<std::underlying_type<logger_item_t::level_t>::type>(item.get_level())],
			item.get_id(),
			item.get_module_name(),
			item.get_message(),
			item.get_function(),
			item.get_file(),
			item.get_line(),
			item.get_frame(),
			item.get_time(),
			item.get_thread(),
		    item.get_link());

	log_file_writer->write(raw);
}

void engine::logger_output_provider_file_t::output_end() const
{
	log_file_writer->write(end_text);
}

engine::logger_output_provider_terminal_t::logger_output_provider_terminal_t(std::shared_ptr<terminal_writer_t> terminal_writer, std::unique_ptr<settings_t<logger_output_t>> settings) : terminal_writer(terminal_writer), settings(std::move(settings))
{
	start_text = this->settings->get()->terminal_start();
	end_text = this->settings->get()->terminal_end();

#define ENGINE_LOGGER_LEVEL_STD(type_id) formattable_string[static_cast<std::underlying_type<logger_item_t::level_t>::type>(logger_item_t::level_t::type_id)] = this->settings->get()->format_terminal_##type_id ();
#include "std/logger_std.hpp"
}

engine::logger_output_provider_terminal_t::~logger_output_provider_terminal_t()
{

}

void engine::logger_output_provider_terminal_t::output_start() const
{
	terminal_writer->write(start_text);
}

void engine::logger_output_provider_terminal_t::output(const logger_item_t & item) const
{
	ustring_t raw = format_string(formattable_string[static_cast<std::underlying_type<logger_item_t::level_t>::type>(item.get_level())],
		item.get_id(),
		item.get_module_name(),
		item.get_message(),
		item.get_function(),
		item.get_file(),
		item.get_line(),
		item.get_frame(),
		item.get_time(),
		item.get_thread(),
		item.get_link());

	terminal_writer->write(raw);
}

void engine::logger_output_provider_terminal_t::output_end() const
{
	terminal_writer->write(end_text);
}

engine::logger_real_t::logger_real_t(std::shared_ptr<engine::frame_notifier_t> frame_notifier, std::shared_ptr<engine::environment_info_t> environment_info, std::unique_ptr<holder_t<logger_output_t> > logger_output_providers) : frame_notifier(frame_notifier), logger_output_providers(std::move(logger_output_providers))
{
	time_start = std::chrono::system_clock::now();
	output_start();
	output_thread = std::thread([this] { output_thread_func(); });
}

engine::logger_real_t::~logger_real_t()
{
	items_queue.push(logger_item_t(logger_item_t::finished_t{}));
	try
	{
		output_thread.join();
	}
	catch (const std::system_error& e) {}
	output_end();
}

void engine::logger_real_t::reset_raports(logger_item_t::module_t module)
{
	std::lock_guard<std::recursive_mutex> guard(items_cache_mutex);

	for (auto & item : get_cache(module))
	{
		item.clear_raport_flag();
	}
	for (auto & item : get_cache(logger_item_t::module_t::unknown))
	{
		if (item.get_module() == module)
			item.clear_raport_flag();
	}
}

engine::logger_items_t engine::logger_real_t::get_raported_items(logger_item_t::module_t module) const
{
	std::lock_guard<std::recursive_mutex> guard(items_cache_mutex);

	logger_items_t ret;

	for (auto & item : get_cache(module))
	{
		if (item.get_module() == module)
			ret.push_back(item);
	}

	return ret;
}

engine::logger_items_t engine::logger_real_t::get_all_items(logger_item_t::module_t module) const
{
	std::lock_guard<std::recursive_mutex> guard(items_cache_mutex);

	return get_cache(module);
}

void engine::logger_real_t::output_start()
{
	for (int provider = 0; provider < logger_output_providers->get_providers_count(); provider++)
	{
		logger_output_providers->get_provider(provider)->output_start();
	}
}

void engine::logger_real_t::output_end()
{
	for (int provider = 0; provider < logger_output_providers->get_providers_count(); provider++)
	{
		logger_output_providers->get_provider(provider)->output_end();
	}
}

void engine::logger_real_t::output_thread_func()
{
	for (;;)
	{
		logger_item_t item = items_queue.pop();
		if (item.get_level() == logger_item_t::level_t::finished)
			return;

		for (int provider = 0; provider < logger_output_providers->get_providers_count(); provider++)
		{
			logger_output_providers->get_provider(provider)->output(item);
		}
	}
}

engine::logger_real_t::item_id_t engine::logger_real_t::log_local(logger_item_t::level_t level, logger_item_t::module_t module, logger_item_t::raport_t raport, const ustring_t & file, uint32_t line, const ustring_t & function, std::size_t link, const ustring_t & message)
{
	size_t item_id;
	{
		std::lock_guard<std::recursive_mutex> guard(items_cache_mutex);

		auto & cache = get_cache(logger_item_t::module_t::unknown);

		if (module == logger_item_t::module_t::unknown && link < cache.size())
			module = cache[link].get_module();

		ustring_t msg = message;
		if (link < cache.size() && msg.is_empty())
			msg = cache[link].get_message();

		logger_item_t item(cache.size(), level, module, msg, function, raport, file, line, frame_notifier->get_frame(), std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - time_start), std::this_thread::get_id(), link);

		cache.push_back(item);
		if (module != logger_item_t::module_t::unknown)
			get_cache(module).push_back(item);

		item_id = cache.size() - 1;
		items_queue.push(item);
	}
	return item_id;
}