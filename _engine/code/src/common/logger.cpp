#include <SDL_version.h>
#include <SDL_revision.h>
#include <SDL_cpuinfo.h>
#include <ctime>
#include <cstdio>
#include <sstream>
#include <fstream>
#include "pugixml.hpp"
#include "component/logger.hpp"
#include "utility/text/stringify.hpp"

void engine::logger_item_t::parse_file()
{

#if defined _MSC_VER

	static std::string engine_tag = "_engine/src/";
	static std::string app_tag = XSTR(PIXIE_APP_UNIX_NAME) "/src/";

	std::string file_tmp = file_raw.get_cstring();
	std::replace(file_tmp.begin(), file_tmp.end(), '\\', '/');

	std::size_t index = file_tmp.find(engine_tag);
	if (index != std::string::npos)
		file_tmp = file_tmp.substr(index);

	index = file_tmp.find(app_tag);
	if (index != std::string::npos)
		file_tmp = file_tmp.substr(index);

	file = ustring_t::from_utf8(file_tmp.c_str());

#else 

#error "Specify parsing function for logger item t filename!"

#endif

}

engine::logger_real_t::logger_real_t(std::shared_ptr<engine::frame_notifier_t> frame_notifier, std::unique_ptr<holder_t<logger_output_t> > logger_output_providers) : frame_notifier(frame_notifier), logger_output_providers(std::move(logger_output_providers))
{
	output_thread = std::thread([this] { output_thread_func(); });
}

engine::logger_real_t::~logger_real_t()
{
	items_queue.push(logger_item_t(logger_item_t::finished_t{}));
	output_thread.join();
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

engine::logger_real_t::item_id_t engine::logger_real_t::log_local(logger_item_t::level_t level, logger_item_t::module_t module, bool raport, const ustring_t & file, uint32_t line, const ustring_t & function, std::size_t link, const ustring_t & message)
{
	size_t item_id;
	{
		std::lock_guard<std::recursive_mutex> guard(items_cache_mutex);

		auto & cache = get_cache(logger_item_t::module_t::unknown);

		if (module == logger_item_t::module_t::unknown && link < cache.size())
			module = cache[0].get_module();

		logger_item_t item(items_cache.size(), level, module, message, function, raport, file, line, frame_notifier->get_frame(), std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - time_start), std::this_thread::get_id(), link);

		cache.push_back(item);
		if (module != logger_item_t::module_t::unknown)
			get_cache(module).push_back(item);

		item_id = cache.size() - 1;
		items_queue.push(item);
	}
	return item_id;
}