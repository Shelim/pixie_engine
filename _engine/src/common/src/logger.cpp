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
#include "common/environment_info.hpp"
#include "common/logger_output/providers.hpp"


engine::logger_output::provider_data_t::provider_data_t(std::shared_ptr<engine::data::database_t> database, std::shared_ptr<logger_t> logger, std::shared_ptr<environment_info_t> environment_info) : provider_base_t(logger), save_items_completed(false)
{

#define GAME_LOGGER_VIRTUAL_PATH_STD(path) output = database->get_output(virtual_path_t(path, virtual_path_t::type_t::log));
#include "common/std/virtual_path_std.hpp"

	output->write_ustring(_U("########################\r\n"));
	output->write_ustring(_U("### ENVIRONMENT INFO ###\r\n"));
	output->write_ustring(_U("########################\r\n"));
	
#define GAME_ENVIRONMENT_INFO_STD(key, name) { ustring_t info_str = _U("> "); const environment_info_t::item_t & info = environment_info->get(environment_info_t::key_t::key); info_str.append(name); info_str.append(_U(":")); padd(info_str, 32); info_str.append(info.get_value()); info_str.append(_U("\r\n")); output->write_ustring(info_str); }
#include "common/std/environment_info_std.hpp"

	output->write_ustring(_U("########################\r\n"));
	output->write_ustring(_U("###     FULL LOG     ###\r\n"));
	output->write_ustring(_U("########################\r\n"));
	output->force_flush();
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