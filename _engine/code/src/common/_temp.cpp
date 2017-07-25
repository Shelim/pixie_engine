#include <SDL.h>
#include <cstdio>
#include <cstdlib>
#include <thread>
#include <chrono>
#include "bootstrapper.hpp"
#include "component/logger.hpp"
#include "component/frame_notifier.hpp"
#include "component/terminal_output.hpp"
#include "component/config.hpp"
#include "utility/vfs/filesystem.hpp"
#include "utility/text/ustring.hpp"
#include "utility/vfs/virtual_path.hpp"
#include "settings/default_paths.hpp"
#include "settings/default_terminal.hpp"
#include "settings/default_logger.hpp"
#include "settings/default_config.hpp"
#include "utility/text/parser.hpp"

int main(int arg, char * argv[])
{

	engine::bootstrapper_t< 

		engine::register_as<engine::frame_notifier_dummy_t, engine::frame_notifier_t>,
		engine::register_as<engine::logger_real_t, engine::logger_t>,
		engine::register_as<engine::terminal_output_real_t, engine::terminal_output_t>,
		engine::register_as<engine::config_real_t, engine::config_t>,

#if PIXIE_IS_PORTABLE_BUILD
		USE_SETTINGS(save_location_resolver_t, windows_portable),
#else
		USE_SETTINGS(save_location_resolver_t, windows_normal),
#endif
#if PIXIE_IS_DEBUG_BUILD
		USE_SETTINGS(config_t, debug),
#else
		USE_SETTINGS(config_t, release),
#endif
		USE_SETTINGS(common_filenames_t, normal),
		USE_SETTINGS(terminal_output_colors_t, normal),
		USE_SETTINGS(logger_output_t, normal),

		engine::register_providers_for<engine::logger_output_t, engine::logger_output_provider_terminal_t, engine::logger_output_provider_file_t>,
		engine::register_provider_for<engine::config_t, engine::config_provider_storage_t>
	
	> bootstrapper;

	std::shared_ptr<engine::terminal_output_t> terminal = bootstrapper.construct_component<engine::terminal_output_t>();
	std::shared_ptr<engine::logger_t> logger = bootstrapper.construct_component<engine::logger_t>();
	std::shared_ptr<engine::config_t> config = bootstrapper.construct_component<engine::config_t>();

	int i = 0;

	auto task = logger->log_task_start(core, "Calculating"_u);
	logger->log_task_done(task);

	task = logger->log_task_start(core, "Calculating no 2"_u);
	logger->log_task_failed(task);
	task = logger->log_msg(core, "Sample message"_u);
	task = logger->log_warn(core, "Sample warning"_u);
	task = logger->log_err(core, "Sample error"_u);

	/*
	std::this_thread::sleep_for(std::chrono::seconds(1));
	config->set_game_has_console(false);
	std::this_thread::sleep_for(std::chrono::seconds(1));
	config->set_game_has_console(true);
	std::this_thread::sleep_for(std::chrono::seconds(1));
	*/
	for (;;)
	{
		
		if (i == 25000) config->set_game_has_console(false);
		if (i == 40000) config->set_game_has_console(true);
		if (i == 50000) break;

		++i;

		engine::terminal_output_string_t str(engine::format_string("#1# sample"_u, i));
		logger->log_msg(core, "#1# log"_u, i);

		terminal->write(str);

		/*
		logger->log_msg(core, "Hello world: #1#!"_u, ++i);
		if ((i % 100) == 66)
			logger->log_err(core, "Critical failure!"_u);
			*/
	}

	return 0;
}