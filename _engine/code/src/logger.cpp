#include "global/component/logger.hpp"
#include "global/core/console/writer.hpp"
#include "global/core/vfs/virtual_path.hpp"
#include "global/core/thread/thread.hpp"

engine::logger_t::logger_t()
{

}

engine::logger_t::~logger_t()
{

}

engine::logger_t::item_id_t engine::logger_dummy_t::log_local(engine::console::logger_item_t::kind_t kind, engine::app_t::kind_t app, engine::app_t::instance_id_t app_instance_id, engine::console::logger_item_t::source_t source, const ustring_t & file, uint32_t line, const ustring_t & function, std::size_t link, const ustring_t & message)
{
	return 0;
}

const engine::console::logger_items_t & engine::logger_dummy_t::get_cache() const
{
	return engine::console::logger_items_t{};
}

engine::logger_provider_base_t::~logger_provider_base_t()
{

}

engine::logger_real_t::logger_real_t(std::shared_ptr<engine::environment_info_t> environment_info, std::unique_ptr<holder_t<logger_t> > logger_providers)
 : logger_providers(std::move(logger_providers))
{
	time_start = std::chrono::system_clock::now();
}

engine::logger_real_t::~logger_real_t()
{
}

const engine::console::logger_items_t & engine::logger_real_t::get_cache() const
{
	return cache;
}

engine::logger_real_t::item_id_t engine::logger_real_t::log_local(engine::console::logger_item_t::kind_t kind, engine::app_t::kind_t app, engine::app_t::instance_id_t app_instance_id, engine::console::logger_item_t::source_t source, const ustring_t & file, uint32_t line, const ustring_t & function, std::size_t link, const ustring_t & message)
{
	size_t item_id;
	{
		if (source == engine::console::logger_item_t::source_t::unknown && link < cache.size())
		{
			source = cache[link].get_source();
			app = cache[link].get_app();
			app_instance_id = cache[link].get_app_instance_id();
		}

		ustring_t msg = message;
		if (link < cache.size() && msg.is_empty())
			msg = cache[link].get_message();

		engine::console::logger_item_t item(cache.size(), kind, app, app_instance_id, source, msg, function, file, line, std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - time_start), std::this_thread::get_id(), link);

		cache.push_back(item);

		item_id = cache.size() - 1;

		for (int provider = 0; provider < logger_providers->get_providers_count(); provider++)
		{
			logger_providers->get_provider(provider)->output(item);
		}
	}
	return item_id;
}

engine::logger_provider_console_t::logger_provider_console_t(std::shared_ptr<engine::console_writer_t> console_writer) : console_writer(console_writer)
{

}

void engine::logger_provider_console_t::output(const engine::console::logger_item_t & item) const
{
	console_writer->write(std::make_unique<engine::console::logger_item_t>(item));
}