#include <SDL.h>
#include <cstdio>
#include <cstdlib>
#include <thread>
#include <chrono>
#include "bootstrapper.hpp"
#include "utility/vfs/filesystem.hpp"
#include "utility/text/ustring.hpp"
#include "utility/vfs/virtual_path.hpp"
#include "settings/default_archive.hpp"
#include "settings/default_paths.hpp"
#include "settings/default_terminal.hpp"
#include "settings/default_logger.hpp"
#include "settings/default_config.hpp"
#include "settings/default_environment_info.hpp"
#include "utility/text/parser.hpp"
#include "utility/concurrention/thread_pool.hpp"
#include "manifest_app.hpp"

class sample_job_t final : public engine::thread_pool_job_t
{

public:

	sample_job_t(std::shared_ptr<engine::logger_t> logger) : logger(logger)
	{
		id++;
		cur_id = id;
	}

	engine::ustring_t get_name() const final
	{
		return engine::format_string("Sample job #1#"_u, cur_id);
	}

	void run(engine::thread_pool_token_t * token) final
	{
		for (int i = 0; i < 5; i++)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(750));
			token->get_logger()->log_msg(core, "Simulating execution #1#: #2#..."_u, cur_id, i);

			if (token->is_shutdown_requested())
			{
				token->get_logger()->log_msg(core, "Requested #1# termination before completion!"_u, cur_id);
				return;
			}
		}

	}

private:

	uint32_t cur_id;
	static uint32_t id;
	std::shared_ptr<engine::logger_t> logger;
};

uint32_t sample_job_t::id;

int main(int arg, char * argv[])
{

	engine::bootstrapper_t< 

		ENABLE_COMPONENT(logger),
		ENABLE_COMPONENT(terminal_writer),
		ENABLE_COMPONENT(config),
		ENABLE_COMPONENT(environment_info),
		ENABLE_COMPONENT(log_file_writer),
		ENABLE_COMPONENT(data_source),
		ENABLE_COMPONENT(data_archives),

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
		USE_SETTINGS(data_archives_t, normal),
		USE_SETTINGS(common_filenames_t, normal),
		USE_SETTINGS(terminal_writer_colors_t, normal),
		USE_SETTINGS(logger_output_t, normal),
		USE_SETTINGS(environment_info_output_t, normal),

		USE_PROVIDER_FOR(config, storage),
		
		USE_PROVIDERS_FOR(environment_info_output, terminal, file),
		USE_PROVIDERS_FOR(logger_output, terminal, file)
	
	> bootstrapper;

	std::shared_ptr<engine::terminal_writer_t> terminal = bootstrapper.construct_component<engine::terminal_writer_t>();
	std::shared_ptr<engine::logger_t> logger = bootstrapper.construct_component<engine::logger_t>();
	std::shared_ptr<engine::config_t> config = bootstrapper.construct_component<engine::config_t>();
	std::shared_ptr<engine::environment_info_t> environment_info = bootstrapper.construct_component<engine::environment_info_t>();
	std::shared_ptr<engine::data_source_t> data_source = bootstrapper.construct_component<engine::data_source_t>();
	std::shared_ptr<engine::thread_pool_t<4>> thread_pool = bootstrapper.construct_component<engine::thread_pool_t<4>>();
	std::shared_ptr<engine::process::runner_engine_thread_t<engine::process::runner_thread_t::engine_update> > runner_update = bootstrapper.construct_component<engine::process::runner_engine_thread_t<engine::process::runner_thread_t::engine_update> >();

	thread_pool->enqueue_job(std::make_unique<sample_job_t>(logger));
	thread_pool->enqueue_job(std::make_unique<sample_job_t>(logger));
	thread_pool->enqueue_job(std::make_unique<sample_job_t>(logger));
	thread_pool->enqueue_job(std::make_unique<sample_job_t>(logger));
	thread_pool->enqueue_job(std::make_unique<sample_job_t>(logger));
	thread_pool->enqueue_job(std::make_unique<sample_job_t>(logger));
	thread_pool->enqueue_job(std::make_unique<sample_job_t>(logger));
	thread_pool->enqueue_job(std::make_unique<sample_job_t>(logger));
	
	int i = 0;

	auto task = logger->log_task_start(core, "Calculating"_u);
	logger->log_task_done(task);

	task = logger->log_task_start(core, "Calculating no 2"_u);
	logger->log_task_failed(task);
	task = logger->log_msg(core, "Sample message"_u);
	task = logger->log_warn(core, "Sample warning"_u);
	task = logger->log_err(core, "Sample error"_u);
	
	std::this_thread::sleep_for(std::chrono::seconds(1));
	config->set_cfg_has_terminal(false);
	std::this_thread::sleep_for(std::chrono::seconds(1));
	config->set_cfg_has_terminal(true);
	std::this_thread::sleep_for(std::chrono::seconds(1));
	
	for (;;)
	{
		runner_update->run_all_available_once();
		
		/*
		if (i == 250000) config->set_game_has_console(false);
		if (i == 400000) config->set_game_has_console(true);
		if (i == 500000) break;
		*/

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		i++;
//		if (i == 10000) break;
		

		/*
		logger->log_msg(core, "Hello world: #1#!"_u, ++i);
		if ((i % 100) == 66)
			logger->log_err(core, "Critical failure!"_u);
			*/
	}

	runner_update->run_on_shutdown();

	return 0;
}