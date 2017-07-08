#include <SDL.h>
#include <cstdio>
#include <cstdlib>
#include "bootstrapper.hpp"
#include "component/logger.hpp"
#include "component/frame_notifier.hpp"
#include "component/terminal_output.hpp"
#include "utility/vfs/filesystem.hpp"
#include "utility/text/ustring.hpp"
#include "utility/vfs/virtual_path.hpp"
#include "settings/default_paths.hpp"
#include "settings/default_terminal.hpp"
#include "settings/default_logger.hpp"

int main(int arg, char * argv[])
{

	engine::bootstrapper_t< 

		engine::register_as<engine::frame_notifier_dummy_t, engine::frame_notifier_t>,
		engine::register_as<engine::logger_real_t, engine::logger_t>,
		engine::register_as<engine::terminal_output_real_t, engine::terminal_output_t>,

#if PIXIE_IS_PORTABLE_BUILD
		USE_SETTINGS(save_locatin_resolver_t, windows_portable),
#else
		USE_SETTINGS(save_locatin_resolver_t, windows_normal),
#endif
		USE_SETTINGS(common_filenames_t, normal),
		USE_SETTINGS(terminal_output_colors_t, normal),
		USE_SETTINGS(logger_output_t, normal),

		engine::register_providers_for<engine::logger_output_t, engine::logger_output_provider_terminal_t, engine::logger_output_provider_file_t>
	
	> bootstrapper;

	std::shared_ptr<engine::terminal_output_t> terminal = bootstrapper.construct_component<engine::terminal_output_t>();
	std::shared_ptr<engine::logger_t> logger = bootstrapper.construct_component<engine::logger_t>();

	terminal->update_window(engine::terminal_output_t::window_state_t::open);

	int i = 0;
	for (;;)
	{
		if (i == 50000) break;

		logger->log_msg(core, "Hello world: #1#!"_u, ++i);
	}

	return 0;
}