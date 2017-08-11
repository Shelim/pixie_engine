#include <SDL.h>
#include <cstdio>
#include <cstdlib>
#include <thread>
#include <chrono>
#include "bootstrapper.hpp"
#include "utility/vfs/filesystem.hpp"
#include "utility/text/ustring.hpp"
#include "utility/vfs/virtual_path.hpp"
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

class sample_task_t : public engine::task_base_t
{

public:


private:



};

uint32_t sample_job_t::id;

int main(int arg, char * argv[])
{

	engine::bootstrapper_t< 

		engine::register_as<engine::logger_real_t, engine::logger_t>,
		engine::register_as<engine::terminal_writer_real_t, engine::terminal_writer_t>,
		engine::register_as<engine::config_real_t, engine::config_t>,
		engine::register_as<engine::environment_info_real_t, engine::environment_info_t>,
		engine::register_as<engine::log_file_writer_real_t, engine::log_file_writer_t>,
		engine::register_as<engine::data_provider_real_t, engine::data_provider_t>,
		engine::register_as<engine::tasks_real_t, engine::tasks_t>,

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
	std::shared_ptr<engine::thread_pool_t<4>> thread_pool = bootstrapper.construct_component<engine::thread_pool_t<4>>();

	thread_pool->enqueue_job(std::make_unique<sample_job_t>(logger));
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
		break;

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