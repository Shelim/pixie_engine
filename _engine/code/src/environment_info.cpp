/*

#include "component/environment_info.hpp"
#include "utility/platform/compilation.hpp"
#include "utility/platform/info.hpp"
#include "utility/text/ustring.hpp"

engine::environment_info_real_t::environment_info_real_t(std::shared_ptr<platform::info_t> platform_info, std::shared_ptr<manifest_version_t> manifest_version, std::unique_ptr<holder_t<environment_info_output_t> > environment_info_output_providers) : environment_info_output_providers(std::move(environment_info_output_providers)), manifest_version(manifest_version), platform_info(platform_info)
{
	gather_info();
	output_info();
}

void engine::environment_info_real_t::gather_info()
{
	set_info(key_t::engine_version, manifest_version->get_string());
	set_info(key_t::launched_time, to_string(std::chrono::system_clock::now()));
	set_info(key_t::os_name, compilation_t::get_os_name(compilation_t::get_os()));
	set_info(key_t::os_ver, platform_info->get_os_version());
	set_info(key_t::cpu_cores, to_string(std::thread::hardware_concurrency()));
	set_info(key_t::cpu_cache_line, to_string(platform_info->get_cache_line_size()));
	set_info(key_t::cpu_features, platform_info->get_cpu_features());
}

void engine::environment_info_real_t::output_info()
{
	for (int provider = 0; provider < environment_info_output_providers->get_providers_count(); provider++)
	{
		environment_info_output_providers->get_provider(provider)->output_start();
		for (std::underlying_type<environment_info_real_t::key_t>::type key = 0; key < value_of(environment_info_real_t::key_t::count); key++)
		{
			environment_info_output_providers->get_provider(provider)->output(static_cast<environment_info_real_t::key_t>(key), items[key].get_status(), items[key].get_value());
		}
		environment_info_output_providers->get_provider(provider)->output_end();
	}
}

engine::environment_info_output_provider_file_t::environment_info_output_provider_file_t(std::shared_ptr<logger_file_writer_t> logger_file_writer, std::unique_ptr<settings_t<environment_info_output_t>> settings)  : logger_file_writer(logger_file_writer), settings(std::move(settings))
{
	start_text = this->settings->get()->file_start();
	end_text = this->settings->get()->file_end();

#define ENGINE_ENVIRONMENT_INFO_STATUS_DEF(status_id) formattable_string[value_of(environment_info_t::status_t::status_id)] = this->settings->get()->format_file_##status_id ();
#include "def/environment_info.def"
}
engine::environment_info_output_provider_file_t::~environment_info_output_provider_file_t()
{

}

void engine::environment_info_output_provider_file_t::output_start() const
{
	logger_file_writer->write(start_text);
}

void engine::environment_info_output_provider_file_t::output(environment_info_t::key_t key, environment_info_t::status_t status, const ustring_t & value) const
{
	logger_file_writer->write(format_string(formattable_string[value_of(status)], environment_info_t::key_to_name(key), value));
}

void engine::environment_info_output_provider_file_t::output_end() const
{
	logger_file_writer->write(end_text);
}

engine::environment_info_output_provider_terminal_t::environment_info_output_provider_terminal_t(std::shared_ptr<terminal_writer_t> terminal_writer, std::unique_ptr<settings_t<environment_info_output_t>> settings) : terminal_writer(terminal_writer), settings(std::move(settings))
{
	start_text = this->settings->get()->terminal_start();
	end_text = this->settings->get()->terminal_end();

#define ENGINE_ENVIRONMENT_INFO_STATUS_DEF(status_id) formattable_string[value_of(environment_info_t::status_t::status_id)] = this->settings->get()->format_terminal_##status_id ();
#include "def/environment_info.def"
}

void engine::environment_info_output_provider_terminal_t::output_start() const
{
	terminal_writer->write(terminal_writer_t::writer_t::environment_info, start_text);
}

void engine::environment_info_output_provider_terminal_t::output(environment_info_t::key_t key, environment_info_t::status_t status, const ustring_t & value) const
{
	terminal_writer->write(terminal_writer_t::writer_t::environment_info, format_string(formattable_string[value_of(status)], environment_info_t::key_to_name(key), value));
}

void engine::environment_info_output_provider_terminal_t::output_end() const
{
	terminal_writer->write(terminal_writer_t::writer_t::environment_info, end_text);
}

*/