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

		engine::register_providers_for<engine::logger_output_t>
	
	> bootstrapper;

	std::shared_ptr<engine::platform_internal_t> component = bootstrapper.construct_component<engine::platform_internal_t>();

	component->console_update_window(engine::platform_internal_t::console_window_state_t::open);

	std::cout << u8"za¿ó³æ gêœl¹ jaŸñ";

	return 0;
}