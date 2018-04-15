#include "global/core/data/input.hpp"
#include "global/core/data/scanner.hpp"
#include "global/core/data/output.hpp"
#include "global/core/data/provider.hpp"
#include "global/core/data/provider_actual.hpp"
#include <algorithm>

std::unique_ptr<engine::data::input_t> engine::data::input_t::spawn_partial(int64_t size)
{
	buffer_t buff;
	buff.resize(size);
	read(&buff[0], size);

	return std::move(std::make_unique<input_partial_t>(path, std::move(buff)));
}

void engine::data::scanner_directory_t::scan_local(callback_t on_found) 
{
	std::size_t len = base_physical_path.native().size();
	if (base_physical_path.native()[len - 1] != '/' && base_physical_path.native()[len - 1] != '\\')
		len++;

	std::error_code ec;

	for (auto & file : filesystem->iterate_files_in_subdirectories(base_physical_path))
	{
		virtual_path_t virtual_path;
		virtual_path.set_type(base_virtual_path.get_type());

		virtual_path_t::path_t path = base_virtual_path.get_path();
		if (!path.is_empty() && path.last_ascii() != '/')
			path.append_utf8(u8"/");

		std::string path_regular = file.u8string().substr(len);
		std::replace(path_regular.begin(), path_regular.end(), '\\', '/');
		path.append_utf8(path_regular.c_str());

		virtual_path.set_path(path);

		on_found(std::make_unique<provider_actual_file_t>(filesystem, virtual_path, file, read_only, use_safe_output));
	}
}


bool engine::data::provider_t::add_provider_actual(std::unique_ptr<provider_actual_t> provider)
{
	std::lock_guard<std::recursive_mutex> guard(mutex);
	if (!provider) return false;

	if (providers.empty())
		virtual_path = provider->get_virtual_path();

	if (virtual_path == provider->get_virtual_path())
	{
		provider_actual_t * search_for = provider.get();
		if(std::find_if(providers.begin(), providers.end(), [search_for](const std::unique_ptr<provider_actual_t> & ptr){
			return ptr->get_kind() == search_for->get_kind() && ptr->get_time_last_mod() == search_for->get_time_last_mod();			
		}) != providers.end())
			return true;

		providers.insert(std::move(provider));
		return true;
	}
	return false;
}