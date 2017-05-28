#include <SDL_version.h>
#include <SDL_revision.h>
#include <SDL_cpuinfo.h>
#include <ctime>
#include <cstdio>
#include <sstream>
#include <fstream>
#include "pugixml.hpp"
#include "common/execution_info.hpp"
#include "common/platform.hpp"
#include "common/xml_pixie_archive.hpp"
#include "common/platform.hpp"
#include <cereal/types/array.hpp>
#include <cereal/types/vector.hpp>

engine::execution_info_t::vals_t & engine::execution_info_t::get_val_for_save()
{
	std::chrono::system_clock::time_point time_now = std::chrono::system_clock::now();

	std::chrono::seconds duration = std::chrono::duration_cast<std::chrono::seconds>(time_now - time_start);
	set_info(key_t::execution_time, engine::to_string(duration));
	
	return vals;
}

engine::execution_info_t::execution_info_t(std::shared_ptr<engine::platform_t> platform, std::shared_ptr<engine::manifest_app_t> manifest_app) : platform(platform), manifest_app(manifest_app)
{
#define GAME_EXECUTION_INFO_STD(name, lang) vals[static_cast<size_t>(key_t::name)].desc = lang;
#include "common/std/execution_info_std.hpp"

	time_start = std::chrono::system_clock::now();
	std::time_t t = std::chrono::system_clock::to_time_t(time_start);
	ustring_t ts = ustring_t::from_ascii(ctime(&t));
	ts = ts.substr(0, ts.len() - 1); // remove trailling newline
	set_info(key_t::launched_time, ts);

	std_platform();
}

engine::execution_info_t::~execution_info_t()
{

}

void engine::execution_info_t::std_platform()
{
	set_info(key_t::game_name, ustring_t::from_utf8(manifest_app->game_name()));
	ustring_t app_data_str = _U(PIXIE_OUTPUT_TYPE_STR);
	ustring_t app_data_canonized = app_data_str.substr(0, 1).to_upper();
	app_data_canonized.append(app_data_str.substr(1));
	set_info(key_t::app_type, app_data_canonized);

	#ifdef _DEBUG
	set_info(key_t::game_version, format_utf8(u8"#1#.#2#.#3# (Build #4#) DEBUG", manifest_app->version_major(), manifest_app->version_minor(), manifest_app->version_revision(), manifest_app->version_build()));
	#else
	set_info(key_t::game_version, format_utf8(u8"#1#.#2#.#3# (Build #4#)", manifest_app->version_major(), manifest_app->version_minor(), manifest_app->version_revision(), manifest_app->version_build()));
	#endif

#define GAME_LOGGER_VIRTUAL_PATH_STD(path) set_info(key_t::execution_completed, _U("False; Log may be incompleted! Check '" path "'..."), item_t::status_t::bad);
#include "common/std/virtual_path_std.hpp"	
	set_info(key_t::os_name, ustring_t::from_ascii(SDL_GetPlatform()));
	set_info(key_t::os_ver, platform->get_platform_version());
	set_info(key_t::cpu_cores, to_string(SDL_GetCPUCount()));
	set_info(key_t::cpu_cache, to_string(SDL_GetCPUCacheLineSize()));

	ustring_t features;
	if (SDL_Has3DNow()) features.append_ascii("3DNow ");
	if (SDL_HasAltiVec()) features.append_ascii("AltiVec ");
	if (SDL_HasMMX()) features.append_ascii("MMX ");
	if (SDL_HasRDTSC()) features.append_ascii("RDTSC ");
	if (SDL_HasSSE()) features.append_ascii("SSE ");
	if (SDL_HasSSE2()) features.append_ascii("SSE2 ");
	if (SDL_HasSSE3()) features.append_ascii("SSE3 ");
	if (SDL_HasSSE41()) features.append_ascii("SSE41 ");
	if (SDL_HasSSE42()) features.append_ascii("SSE42 ");

	if (features.len() > 0)
		features = features.substr(0, features.len() - 1);

	set_info(key_t::cpu_features, features);

	SDL_version compiled;
	SDL_version linked;

	SDL_VERSION(&compiled);
	SDL_GetVersion(&linked);

	set_info(key_t::sdl_version_compiled, format_utf8(u8"#1#.#2#.#3#", compiled.major, compiled.minor, compiled.patch));
	set_info(key_t::sdl_version_linked, format_utf8(u8"#1#.#2#.#3#", linked.major, linked.minor, linked.patch));
	set_info(key_t::sdl_revision_compiled, ustring_t::from_ascii(SDL_REVISION));
	set_info(key_t::sdl_revision_linked, ustring_t::from_ascii(SDL_GetRevision()));
	set_info(key_t::crash_dump, _U("None"), item_t::status_t::good);

	if (get(key_t::sdl_version_compiled) == get(key_t::sdl_version_linked))
	{
		set_status(key_t::sdl_version_compiled, item_t::status_t::good);
		set_status(key_t::sdl_version_linked, item_t::status_t::good);
	}
	else
	{
		set_status(key_t::sdl_version_compiled, item_t::status_t::bad);
		set_status(key_t::sdl_version_linked, item_t::status_t::bad);
	}

	if (get(key_t::sdl_revision_compiled) == get(key_t::sdl_revision_linked))
	{
		set_status(key_t::sdl_revision_compiled, item_t::status_t::good);
		set_status(key_t::sdl_revision_linked, item_t::status_t::good);
	}
	else
	{
		set_status(key_t::sdl_revision_compiled, item_t::status_t::bad);
		set_status(key_t::sdl_revision_linked, item_t::status_t::bad);
	}

}