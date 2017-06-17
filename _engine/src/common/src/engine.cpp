#include "common/engine.hpp"
#include <memory>

std::function<void()> engine::arg0 = [] {};


namespace
{
	auto make_di = [](const std::string& program_path)
	{
		return di::make_injector(di::bind<std::string>().named(engine::arg0).to(program_path),
			di::bind<engine::config_io::provider_base_t>().to<engine::config_io::GAME_CONFIG_PROVIDER_STD>());
	};
}

void engine::engine_container_t::run()
{
	engine = make_di(platform_get_executable_filename()).create<std::unique_ptr<engine::engine_t> >();

	engine->run();

	is_completed = true;

	condition_variable.notify_one();
}