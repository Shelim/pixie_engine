/*

#include "component/logger_file_writer.hpp"

engine::logger_file_writer_real_t::logger_file_writer_real_t(std::shared_ptr<platform::filesystem_t> platform_filesystem)
{
	std::lock_guard<std::recursive_mutex> guard(fp_mutex);
	output = platform_filesystem->output('todo');
}

engine::logger_file_writer_real_t::~logger_file_writer_real_t()
{
	std::lock_guard<std::recursive_mutex> guard(fp_mutex);
}

void engine::logger_file_writer_real_t::write(const ustring_t & line)
{
	std::lock_guard<std::recursive_mutex> guard(fp_mutex);

	output->write(reinterpret_cast<const uint8_t*>(line.get_cstring()), line.len());
	output->write("\n", 1);
	output->flush();
}

*/