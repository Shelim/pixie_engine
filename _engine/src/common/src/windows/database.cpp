#include "common/platform.hpp"

#if PIXIE_WINDOWS

#include "common/data/database.hpp"
#include "common/data/scanner_file.hpp"
#include "common/data/scanner_directory.hpp"
#include "platform_pimpl.hpp"

#include <clocale>
#include <string>
#include <stack>

#include <SDL.h>
#include <SDL_syswm.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shlwapi.h>
#include <shlobj.h>
#include <shellapi.h>
#include <dbghelp.h>

#include <cstdio>
#include <cstdlib>
#include <io.h>
#include <fcntl.h>

static void add_file(engine::data::scanners_t::collection_t * ret, engine::platform_t * platform, const engine::ustring_t & filename, engine::virtual_path_t::type_t type)
{
	std::filesystem::path path_full = platform->get_pimpl()->get_exe_path();
	path_full /= filename.get_cstring();
	ret->push_back(std::make_unique<engine::data::scanner_file_t>(engine::virtual_path_t(filename, type), path_full, true));

	std::filesystem::path path_local = platform->get_pimpl()->get_save_path(type);
	path_local /= filename.get_cstring();
	ret->push_back(std::make_unique<engine::data::scanner_file_t>(engine::virtual_path_t(filename, type), path_local, false));
}

static void add_path(engine::data::scanners_t::collection_t * ret, engine::platform_t * platform, const engine::ustring_t & filename, engine::virtual_path_t::type_t type)
{
	std::filesystem::path path_full = platform->get_pimpl()->get_exe_path();
	path_full /= filename.get_cstring();
	std::error_code ec;
	if(std::filesystem::exists(path_full, ec))
		ret->push_back(std::make_unique<engine::data::scanner_directory_t>(engine::virtual_path_t(_U(""), type), path_full, true));

	std::filesystem::path path_local = platform->get_pimpl()->get_save_path(type);
	ret->push_back(std::make_unique<engine::data::scanner_directory_t>(engine::virtual_path_t(_U(""), type), path_local, false));
}

std::unique_ptr<engine::data::scanners_t::collection_t > engine::data::database_t::platform_get_scanners_collection()
{
	std::unique_ptr<engine::data::scanners_t::collection_t > ret = std::make_unique<engine::data::scanners_t::collection_t >();

#define GAME_CONFIG_VIRTUAL_PATH_STD(config_path) add_file(ret.get(), platform.get(), _U(config_path), engine::virtual_path_t::type_t::config);
#define GAME_KEYBINDING_VIRTUAL_PATH_STD(keybinding_path) add_file(ret.get(), platform.get(), _U(keybinding_path), engine::virtual_path_t::type_t::keybinding);
#include "common/std/virtual_path_std.hpp"

	add_path(ret.get(), platform.get(), _U("common"), engine::virtual_path_t::type_t::common);
	add_path(ret.get(), platform.get(), _U("modules"), engine::virtual_path_t::type_t::modules);
	add_path(ret.get(), platform.get(), _U("submodules"), engine::virtual_path_t::type_t::submodules);
	add_path(ret.get(), platform.get(), _U("saves"), engine::virtual_path_t::type_t::saves);
	add_path(ret.get(), platform.get(), _U("crash_dumps"), engine::virtual_path_t::type_t::crash_dumps);

	return std::move(ret);
}

std::unique_ptr<engine::data::output_t> engine::data::provider_t::platform_create_new(virtual_path_t path)
{
	std::filesystem::path path_output = engine::platform_t::implementation_t::get_static()->get_save_path(path.get_type());
	path_output /= path.get_path().get_cstring();

	if(path.get_type() == engine::virtual_path_t::type_t::log)
		return std::make_unique<engine::data::output_file_t>(path, path_output);
	else
		return std::make_unique<engine::data::output_file_safe_t>(path, path_output);
}

std::unique_ptr<engine::data::output_t> engine::data::provider_actual_file_t::platform_construct_output_local()
{
	return std::make_unique<output_file_safe_t>(get_virtual_path(), physical_path);
}


#endif