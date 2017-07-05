#include <SDL.h>
#include <cstdio>
#include <cstdlib>
#include "bootstrapper.hpp"
#include "component/logger.hpp"
#include "component/frame_notifier.hpp"
#include "component/platform_internal.hpp"
#include "utility/text/ustring.hpp"

int main(int arg, char * argv[])
{
	engine::bootstrapper_t< 

		engine::register_as<engine::frame_notifier_dummy_t, engine::frame_notifier_t>,
		engine::register_as<engine::logger_real_t, engine::logger_t>,
		engine::register_as<engine::platform_internal_windows_t, engine::platform_internal_t>,

		engine::register_providers_for<engine::logger_output_t, engine::logger_output_provider_console_t, engine::logger_output_provider_file_t>
	
	> bootstrapper;

	std::shared_ptr<engine::platform_internal_t> platform = bootstrapper.construct_component<engine::platform_internal_t>();
	std::shared_ptr<engine::logger_t> logger = bootstrapper.construct_component<engine::logger_t>();

	platform->console_update_window(engine::platform_internal_t::console_window_state_t::open);

	int i = 0;
	for (;;)
	{

		logger->log_msg(core, "Hello world: #1#!"_u, ++i);
	}

	return 0;
}