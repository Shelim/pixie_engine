#include "component/environment_info.hpp"
#include "platform/info.hpp"
#include "manifest_app.hpp"
#include "utility/text/parser.hpp"

engine::environment_info_real_t::environment_info_real_t()
{
	set_info(key_t::full_name, manifest_app_t::get_manifest_full_name());
	set_info(key_t::app_name, manifest_app_t::get_local_manifest_name());
	set_info(key_t::app_version, manifest_app_t::get_version_text());
	set_info(key_t::launched_time, to_string(std::chrono::system_clock::now()));
	set_info(key_t::os_name, platform::get_os());
	set_info(key_t::os_ver, platform::get_os_version());
	set_info(key_t::cpu_cores, to_string(std::thread::hardware_concurrency()));
}