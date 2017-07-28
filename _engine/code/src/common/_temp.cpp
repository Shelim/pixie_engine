#include <SDL.h>
#include <cstdio>
#include <cstdlib>
#include <thread>
#include <chrono>
#include "bootstrapper.hpp"
#include "component/logger.hpp"
#include "component/frame_notifier.hpp"
#include "component/terminal_writer.hpp"
#include "component/config.hpp"
#include "component/log_file_writer.hpp"
#include "component/data_provider.hpp"
#include "component/environment_info.hpp"
#include "utility/vfs/filesystem.hpp"
#include "utility/text/ustring.hpp"
#include "utility/vfs/virtual_path.hpp"
#include "settings/default_paths.hpp"
#include "settings/default_terminal.hpp"
#include "settings/default_logger.hpp"
#include "settings/default_config.hpp"
#include "settings/default_environment_info.hpp"
#include "utility/text/parser.hpp"

int main(int arg, char * argv[])
{

	engine::bootstrapper_t< 

		engine::register_as<engine::frame_notifier_dummy_t, engine::frame_notifier_t>,
		engine::register_as<engine::logger_real_t, engine::logger_t>,
		engine::register_as<engine::terminal_writer_real_t, engine::terminal_writer_t>,
		engine::register_as<engine::config_real_t, engine::config_t>,
		engine::register_as<engine::environment_info_real_t, engine::environment_info_t>,
		engine::register_as<engine::log_file_writer_real_t, engine::log_file_writer_t>,
		engine::register_as<engine::data_provider_real_t, engine::data_provider_t>,

#if PIXIE_IS_PORTABLE_BUILD
		USE_SETTINGS(save_location_resolver_t, windows_portable),
		USE_SETTINGS(scanners_provider_settings_t, windows_portable),
#else
		USE_SETTINGS(save_location_resolver_t, windows_normal),
		USE_SETTINGS(scanners_provider_settings_t, windows_normal),
#endif
#if PIXIE_IS_DEBUG_BUILD
		USE_SETTINGS(config_t, debug),
#else
		USE_SETTINGS(config_t, release),
#endif
		USE_SETTINGS(common_filenames_t, normal),
		USE_SETTINGS(terminal_writer_colors_t, normal),
		USE_SETTINGS(logger_output_t, normal),
		USE_SETTINGS(environment_info_output_t, normal),

		engine::register_providers_for<engine::environment_info_output_t, engine::environment_info_output_provider_terminal_t, engine::environment_info_output_provider_file_t>,
		engine::register_providers_for<engine::logger_output_t, engine::logger_output_provider_terminal_t, engine::logger_output_provider_file_t>,
		engine::register_provider_for<engine::config_t, engine::config_provider_storage_t>
	
	> bootstrapper;

	std::shared_ptr<engine::terminal_writer_t> terminal = bootstrapper.construct_component<engine::terminal_writer_t>();
	std::shared_ptr<engine::logger_t> logger = bootstrapper.construct_component<engine::logger_t>();
	std::shared_ptr<engine::config_t> config = bootstrapper.construct_component<engine::config_t>();
	std::shared_ptr<engine::environment_info_t> environment_info = bootstrapper.construct_component<engine::environment_info_t>();
	std::shared_ptr<engine::data_provider_t> data_provider = bootstrapper.construct_component<engine::data_provider_t>();

	int i = 0;

	auto task = logger->log_task_start(core, "Calculating"_u);
	logger->log_task_done(task);

	task = logger->log_task_start(core, "Calculating no 2"_u);
	logger->log_task_failed(task);
	task = logger->log_msg(core, "Sample message"_u);
	task = logger->log_warn(core, "Sample warning"_u);
	task = logger->log_err(core, "Sample error"_u);

	
	std::this_thread::sleep_for(std::chrono::seconds(1));
	config->config_set_local(has_terminal, false);
	std::this_thread::sleep_for(std::chrono::seconds(1));
	config->config_set_local(has_terminal, true);
	std::this_thread::sleep_for(std::chrono::seconds(1));
	
	for (;;)
	{
		data_provider->update();
		
		/*
		if (i == 250000) config->set_game_has_console(false);
		if (i == 400000) config->set_game_has_console(true);
		if (i == 500000) break;
		*/

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		i++;
		if (i == 10000) break;
		

		/*
		logger->log_msg(core, "Hello world: #1#!"_u, ++i);
		if ((i % 100) == 66)
			logger->log_err(core, "Critical failure!"_u);
			*/
	}

	return 0;
}