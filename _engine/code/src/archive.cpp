/*

#include "component/data_archives.hpp"
#include "core/messenger/msg_scanner_found_item.hpp"
#include "core/data/provider_actual.hpp"
#include "core/data/provider_actual/archive.hpp"
#include "utility/archive/zip.hpp"
#include "core/data/results.hpp"

engine::data_archives_real_t::data_archives_real_t(std::shared_ptr<messenger_t> messenger, std::unique_ptr<settings_t<data_archives_t>> settings) : messenger(messenger), settings(std::move(settings)), callbacks_container(messenger, this)
{
	callbacks_container.attach(msg_scanner_found_item_t::type, [this](msg_base_t * msg) { on_scanner_found_item(msg); });
}


void engine::data_archives_real_t::on_scanner_found_item(msg_base_t * msg)
{
	if (msg->get_type() == msg_scanner_found_item_t::type)
	{
		msg_scanner_found_item_t* found_item_message = static_cast<msg_scanner_found_item_t*>(msg);
		
		ustring_t ext = found_item_message->get_provider()->get_virtual_path().get_ext_lower();
		
#define ENGINE_ARCHIVE_READABLE_DEF(archive_t) { bool is_found = false; for(auto & iter : settings->get()->allowed_extensions_for_zip()) { if(iter == ext) is_found = true; break; } if(is_found) { std::shared_ptr<engine::archive::archive_t##_input_t> item = std::make_shared<engine::archive::archive_t##_input_t>(found_item_message->get_provider()); item->iterate_files_to_scanning_results(found_item_message->get_results()); found_item_message->reject(); }  }
#include "def/archive.def"
	}
}

void engine::archive::zip_input_t::init()
{
	std::lock_guard<std::recursive_mutex> guard(zip_access);
	zlib_filefunc64_def_s filefunc_actual;
	fill_filefunc(&filefunc_actual, this);

	file = unzOpen2_64(get_path().get_path().get_cstring(), &filefunc_actual);

	if (!file) return;
	int status;

	unz_global_info64 globalinfo;
	status = unzGetGlobalInfo64(file, &globalinfo);
	if (status != UNZ_OK) return;

	unz_file_info64 file_info;
	status = unzGoToFirstFile2(file, &file_info, nullptr, 0, nullptr, 0, nullptr, 0);
	while (status == UNZ_OK)
	{
		if (file_info.size_filename > 0)
		{
			std::vector<char> filename;
			filename.resize(file_info.size_filename + 1, 0);
			status = unzGetCurrentFileInfo64(file, nullptr, &filename[0], file_info.size_filename, nullptr, 0, nullptr, 0);
			if (status != UNZ_OK) return;

			if (filename[filename.size() - 2] != '/') // This is not a directory
			{
				add_file(construct_virtual_path(&filename[0]), ustring_t::from_utf8(&filename[0]));
			}
		}

		status = unzGoToNextFile2(file, &file_info, nullptr, 0, nullptr, 0, nullptr, 0);
	}
}

void engine::archive::input_base_t::iterate_files_to_scanning_results(data::results_t * results)
{
	providers_t providers;
	construct_all_providers(&providers);
	for (auto & provider : providers)
	{
		results->add_result(std::move(provider));
	}
}

void engine::archive::zip_input_t::construct_all_providers(providers_t * providers)
{
	for (auto & file : get_files())
	{
		providers->push_back(std::make_unique<engine::data::provider_actual_archive_t>(file.get_path(), file.get_mod_time(), shared_from_this(), file.get_archive_path()));
	}
}

engine::data::input_t::buffer_t engine::archive::zip_input_t::unpack_file(const ustring_t & archive_path)
{
	std::lock_guard<std::recursive_mutex> guard(zip_access);
	int status = UNZ_OK;
	engine::data::input_t::buffer_t ret;

	status = unzLocateFile(file, archive_path.get_cstring(), zip_comparision);
	if (status != UNZ_OK) return ret;

	status = unzOpenCurrentFile(file);
	if (status != UNZ_OK) return ret;

	unz_file_info64 file_info;

	status = unzGetCurrentFileInfo64(file, &file_info, nullptr, 0, nullptr, 0, nullptr, 0);
	if (status != UNZ_OK) return ret;

	std::vector<char> filename;
	filename.resize(file_info.size_filename + 1, 0);

	status = unzGetCurrentFileInfo64(file, nullptr, &filename[0], file_info.size_filename, nullptr, 0, nullptr, 0);
	if (status != UNZ_OK) return ret;

	if (filename[filename.size() - 2] != '/') // This is not a directory
	{
		ret.resize(file_info.uncompressed_size, 0);
		status = unzReadCurrentFile(file, &ret[0], file_info.uncompressed_size);
		if (status != UNZ_OK) return ret;
	}

	status = unzCloseCurrentFile(file);
	if (status != UNZ_OK) return ret;

	return ret;
}

*/