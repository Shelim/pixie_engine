#include "provider/save_location.hpp"
#include "platform/path.hpp"

engine::ustring_t engine::save_location_provider_t::get_save_path(const virtual_path_t & path, bool ensure_it_exists)
{
	ustring_t ret = platform::get_self_path();

#define ENGINE_VIRTUAL_PATH_STD(name) if(path.get_type() == virtual_path_t::type_t::name) { ret = path_resolver->get()->save_path_for_##name(); }
#include "std/virtual_path_std.hpp"

	ret.append('/').append(path.get_path());
	
	ret = virtual_path_t::canonize_path(virtual_path_t::resolve_path(ret));

	if (ensure_it_exists)
	{ 
		std::filesystem::path path = ret.get_cstring();
		if (path.has_filename())
			path = path.remove_filename();
		platform::ensure_directory_exits(path);
	}

	return ret;

}