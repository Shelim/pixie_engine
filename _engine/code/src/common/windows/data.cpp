#include "platform/data.hpp"

#if PIXIE_WINDOWS

#include "utility/data/output.hpp"
#include "provider/save_location.hpp"

#include "windows/internal.hpp"
#include "utility/text/ustring.hpp"
#include "manifest_app.hpp"

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

std::unique_ptr<engine::data::output_t> engine::platform::create_new(const virtual_path_t & path, std::shared_ptr<save_location_provider_t> save_location_provider)
{
	ustring_t physical_path = save_location_provider->get_save_path(path, true);

	return std::move(std::make_unique<engine::data::output_file_safe_t>(path, physical_path.get_cstring()));
}


#endif