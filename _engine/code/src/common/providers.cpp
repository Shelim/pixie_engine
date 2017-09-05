#include "provider/save_location.hpp"
#include "provider/scanners.hpp"
#include "utility/data/provider_actual/file.hpp"
#include "platform/path.hpp"

engine::ustring_t engine::save_location_provider_t::get_save_path(const virtual_path_t & path, ensure_path_exists_t ensure_it_exists)
{
	ustring_t ret = platform::get_self_path();

#define ENGINE_VIRTUAL_PATH_STD(name) if(path.get_type() == virtual_path_t::type_t::name) { ret = path_resolver->get()->save_path_for_##name(); }
#include "std/virtual_path_std.hpp"

	ret.append('/').append(path.get_path());
	
	ret = virtual_path_t::canonize_path(virtual_path_t::resolve_path(ret));

	if (ensure_it_exists == ensure_path_exists_t::yes)
	{ 
		std::filesystem::path path = ret.get_cstring();
		if (path.has_filename())
			path = path.remove_filename();
		platform::ensure_directory_exits(path);
	}

	return ret;

}

std::unique_ptr<engine::data::scanners_t::collection_t > engine::scanners_provider_t::construct_scanners_collection()
{
	std::unique_ptr<engine::data::scanners_t::collection_t > ret = std::make_unique<engine::data::scanners_t::collection_t >();

#define ENGINE_VIRTUAL_PATH_STD(name) { ustring_collection_t collection = scanners->get()->path_fullaccess_for_##name(); for(auto & iter : collection) { ret->push_back(std::make_unique<engine::data::scanner_directory_t>(engine::virtual_path_t(""_u, engine::virtual_path_t::type_t::name), virtual_path_t::canonize_path(virtual_path_t::resolve_path(iter)), data::provider_actual_file_t::read_only_t::no)); } }
#include "std/virtual_path_std.hpp"

#define ENGINE_VIRTUAL_PATH_STD(name) { ustring_collection_t collection = scanners->get()->path_readonly_for_##name(); for(auto & iter : collection) { ret->push_back(std::make_unique<engine::data::scanner_directory_t>(engine::virtual_path_t(""_u, engine::virtual_path_t::type_t::name), virtual_path_t::canonize_path(virtual_path_t::resolve_path(iter)), data::provider_actual_file_t::read_only_t::yes)); } }
#include "std/virtual_path_std.hpp"

	return std::move(ret);
}