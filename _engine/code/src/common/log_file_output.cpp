#include "component/log_file_writer.hpp"

engine::log_file_writer_real_t::log_file_writer_real_t(std::shared_ptr<save_location_provider_t> save_location_provider, std::shared_ptr<common_filenames_provider_t> common_filenames_provider) : save_location_provider(save_location_provider), common_filenames_provider(common_filenames_provider)
{
	std::lock_guard<std::recursive_mutex> guard(fp_mutex);
	ustring_t path = save_location_provider->get_save_path(virtual_path_t(common_filenames_provider->get()->logger(), virtual_path_t::type_t::log));
	fp = fopen(path.get_cstring(), "w");
}

engine::log_file_writer_real_t::~log_file_writer_real_t()
{
	std::lock_guard<std::recursive_mutex> guard(fp_mutex);

	if (fp)
		fclose(fp);

	fp = nullptr;
}

void engine::log_file_writer_real_t::write(const ustring_t & line)
{
	std::lock_guard<std::recursive_mutex> guard(fp_mutex);
	if (fp)
	{
		fputs(line.get_cstring(), fp);
		fputs("\n", fp);
		fflush(fp);
	}
}