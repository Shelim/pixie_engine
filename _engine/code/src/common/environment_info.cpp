#include "component/environment_info.hpp"
#include "platform/info.hpp"
#include "manifest_app.hpp"
#include "utility/text/parser.hpp"

engine::environment_info_real_t::environment_info_real_t(std::unique_ptr<holder_t<environment_info_output_t> > environment_info_output_providers) : environment_info_output_providers(std::move(environment_info_output_providers))
{
	gather_info();
	output_info();
}

void engine::environment_info_real_t::gather_info()
{
	set_info(key_t::full_name, manifest_app_t::get_manifest_full_name());
	set_info(key_t::app_name, manifest_app_t::get_local_manifest_name());
	set_info(key_t::app_type, manifest_app_t::get_local_manifest_output_type());
	set_info(key_t::app_version, manifest_app_t::get_version_text());
	set_info(key_t::launched_time, to_string(std::chrono::system_clock::now()));
	set_info(key_t::os_name, platform::get_os());
	set_info(key_t::os_ver, platform::get_os_version());
	set_info(key_t::cpu_cores, to_string(std::thread::hardware_concurrency()));
	set_info(key_t::cpu_cache_line, to_string(platform::get_cache_line_size()));
	set_info(key_t::cpu_features, platform::get_cpu_features());
}

void engine::environment_info_real_t::output_info()
{
	for (int provider = 0; provider < environment_info_output_providers->get_providers_count(); provider++)
	{
		environment_info_output_providers->get_provider(provider)->output_start();
		for (std::underlying_type<environment_info_real_t::key_t>::type key = 0; key < static_cast<std::underlying_type<environment_info_real_t::key_t>::type>(environment_info_real_t::key_t::count); key++)
		{
			environment_info_output_providers->get_provider(provider)->output(static_cast<environment_info_real_t::key_t>(key), items[key].get_status(), items[key].get_value());
		}
		environment_info_output_providers->get_provider(provider)->output_end();
	}
}

engine::environment_info_output_provider_file_t::environment_info_output_provider_file_t(std::shared_ptr<log_file_writer_t> log_file_writer, std::unique_ptr<settings_t<environment_info_output_t>> settings)  : log_file_writer(log_file_writer), settings(std::move(settings))
{
	start_text = this->settings->get()->file_start();
	end_text = this->settings->get()->file_end();

#define ENGINE_ENVIRONMENT_INFO_STATUS_STD(status_id) formattable_string[static_cast<std::underlying_type<environment_info_t::status_t>::type>(environment_info_t::status_t::status_id)] = this->settings->get()->format_file_##status_id ();
#include "std/environment_info_std.hpp"
}
engine::environment_info_output_provider_file_t::~environment_info_output_provider_file_t()
{

}

void engine::environment_info_output_provider_file_t::output_start() const
{
	log_file_writer->write(start_text);
}

void engine::environment_info_output_provider_file_t::output(environment_info_t::key_t key, environment_info_t::status_t status, const ustring_t & value) const
{
	log_file_writer->write(format_string(formattable_string[static_cast<std::underlying_type<environment_info_t::status_t>::type>(status)], environment_info_t::key_to_name(key), value));
}

void engine::environment_info_output_provider_file_t::output_end() const
{
	log_file_writer->write(end_text);
}

engine::environment_info_output_provider_terminal_t::environment_info_output_provider_terminal_t(std::shared_ptr<terminal_writer_t> terminal_writer, std::unique_ptr<settings_t<environment_info_output_t>> settings) : terminal_writer(terminal_writer), settings(std::move(settings))
{
	start_text = this->settings->get()->terminal_start();
	end_text = this->settings->get()->terminal_end();

#define ENGINE_ENVIRONMENT_INFO_STATUS_STD(status_id) formattable_string[static_cast<std::underlying_type<environment_info_t::status_t>::type>(environment_info_t::status_t::status_id)] = this->settings->get()->format_terminal_##status_id ();
#include "std/environment_info_std.hpp"
}

void engine::environment_info_output_provider_terminal_t::output_start() const
{
	terminal_writer->write(start_text);
}

void engine::environment_info_output_provider_terminal_t::output(environment_info_t::key_t key, environment_info_t::status_t status, const ustring_t & value) const
{
	terminal_writer->write(format_string(formattable_string[static_cast<std::underlying_type<environment_info_t::status_t>::type>(status)], environment_info_t::key_to_name(key), value));
}

void engine::environment_info_output_provider_terminal_t::output_end() const
{
	terminal_writer->write(end_text);
}