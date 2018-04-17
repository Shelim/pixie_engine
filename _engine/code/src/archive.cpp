#include "global/core/data/archive.hpp"
#include "global/core/data/input/buffer.hpp"

void engine::data::archive::zip_input_t::read_archive()
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
	status = unzGoToFirstFile(file);
	while (status == UNZ_OK)
	{
		status = unzGetCurrentFileInfo64(file, &file_info, nullptr, 0, nullptr, 0, nullptr, 0);
		if (status == UNZ_OK && file_info.size_filename > 0)
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

		status = unzGoToNextFile(file);
	}
}


std::unique_ptr<engine::data::input_t> engine::data::archive::zip_input_t::get_file(const ustring_t & archive_path)
{
	std::lock_guard<std::recursive_mutex> guard(zip_access);
	int status = UNZ_OK;
	auto return_path = construct_virtual_path(archive_path.get_cstring());
	auto ret = std::make_unique<engine::data::input_t::buffer_t>();

	status = unzLocateFile(file, archive_path.get_cstring(), 2);
	if (status != UNZ_OK) return std::make_unique<input_void_t>(return_path);

	status = unzOpenCurrentFile(file);
	if (status != UNZ_OK) return std::make_unique<input_void_t>(return_path);

	unz_file_info64 file_info;

	status = unzGetCurrentFileInfo64(file, &file_info, nullptr, 0, nullptr, 0, nullptr, 0);
	if (status != UNZ_OK) return std::make_unique<input_void_t>(return_path);

	std::vector<char> filename;
	filename.resize(file_info.size_filename + 1, 0);

	status = unzGetCurrentFileInfo64(file, nullptr, &filename[0], file_info.size_filename, nullptr, 0, nullptr, 0);
	if (status != UNZ_OK) return std::make_unique<input_void_t>(return_path);

	if (filename[filename.size() - 2] != '/') // This is not a directory
	{
		ret->resize(file_info.uncompressed_size, 0);
		status = unzReadCurrentFile(file, ret.get(), file_info.uncompressed_size);
		if (status != UNZ_OK) return std::make_unique<input_void_t>(return_path);
	}

	status = unzCloseCurrentFile(file);
	if (status != UNZ_OK) return std::make_unique<input_void_t>(return_path);

	return std::make_unique<input_buffer_t>(return_path, std::move(ret));
}