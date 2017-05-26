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
#include "common/xml_pixie_archive.hpp"
#include "common/platform.hpp"
#include "common/config.hpp"
#include "common/data/output.hpp"
#include "common/data/database.hpp"
#include "common/data/database_items.hpp"
#include "common/execution_info.hpp"

uint64_t engine::logger_frame_notifier_t::get_frame() const
{
	return 0;
//	return engine::renderer::get_frame();
}

void engine::logger_t::item_changed(item_id_t item_id)
{
	auto output_providers_shard = output_providers.lock();
	if (output_providers_shard)
		output_providers_shard->on_item_changed(item_id);
}

engine::logger_t::logger_t(std::shared_ptr<engine::platform_t> platform, std::shared_ptr<engine::execution_info_t> execution_info, std::unique_ptr<engine::logger_frame_notifier_t> frame_notifier) :
	platform(platform), execution_info(execution_info), frame_notifier(std::move(frame_notifier)), time_start(std::chrono::system_clock::now())
{
	platform->set_logger(this);
}

engine::logger_t::~logger_t()
{

}

void engine::logger_t::completed_execution()
{
	p_msg(_U("Completed!"));
	set_execution_info(engine::execution_info_t::key_t::execution_completed, _U("True"), engine::execution_info_t::item_t::status_t::good);

	auto output_providers_shard = output_providers.lock();
	if (output_providers_shard)
		output_providers_shard->on_item_changed(-1);
}

void engine::logger_container_t::update()
{
	providers->update();
}