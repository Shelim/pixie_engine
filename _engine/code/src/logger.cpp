#include "component/logger.hpp"
#include "core/console/writer.hpp"
#include "core/vfs/virtual_path.hpp"
#include "core/console/msg/meta.hpp"

engine::logger_real_t::logger_real_t(std::shared_ptr<engine::renderer_status_t> renderer_status, std::shared_ptr<engine::environment_info_t> environment_info, std::unique_ptr<holder_t<logger_t> > logger_providers)
 : renderer_status(renderer_status), logger_providers(std::move(logger_providers))
{
	time_start = std::chrono::system_clock::now();
	output_start();
}

engine::logger_real_t::~logger_real_t()
{
	output_end();
}

void engine::logger_real_t::output_start()
{
	for (int provider = 0; provider < logger_providers->get_providers_count(); provider++)
	{
		logger_providers->get_provider(provider)->output_start();
	}
}

void engine::logger_real_t::output_end()
{
	for (int provider = 0; provider < logger_providers->get_providers_count(); provider++)
	{
		logger_providers->get_provider(provider)->output_end();
	}
}

engine::logger_real_t::item_id_t engine::logger_real_t::log_local(engine::console::logger_item_t::kind_t kind, engine::console::logger_item_t::source_t source, const ustring_t & file, uint32_t line, const ustring_t & function, std::size_t link, const ustring_t & message)
{
	size_t item_id;
	{
		if (source == engine::console::logger_item_t::source_t::unknown && link < cache.size())
			source = cache[link].get_source();

		ustring_t msg = message;
		if (link < cache.size() && msg.is_empty())
			msg = cache[link].get_message();

		engine::console::logger_item_t item(cache.size(), kind, source, msg, function, file, line, renderer_status->get_frame(), std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - time_start), std::this_thread::get_id(), link);

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

void engine::logger_provider_console_t::output_start() const
{
	console_writer->write(std::make_unique<engine::console::meta_item_t>(engine::console::meta_item_t::meta_type_t::output_start));
}
void engine::logger_provider_console_t::output(const engine::console::logger_item_t & item) const
{
	console_writer->write(std::make_unique<engine::console::logger_item_t>(item));
}
void engine::logger_provider_console_t::output_end() const
{
	console_writer->write(std::make_unique<engine::console::meta_item_t>(engine::console::meta_item_t::meta_type_t::output_end));
}