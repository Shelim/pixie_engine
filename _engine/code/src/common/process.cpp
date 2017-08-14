#include "utility/process/process.hpp"
#include "utility/process/processes.hpp"
#include "utility/process/runner.hpp"
#include "utility/pattern/enum.hpp"

engine::processes_internal_t::processes_internal_t(std::shared_ptr<logger_t> logger) : logger(logger)
{
	logger->log_msg(process, "Processes library ready for clients"_u);
}

void engine::processes_internal_t::start_process(std::unique_ptr<process_t> process)
{
	auto task = logger->log_task_start(process, "Starting new process '#1#'"_u, process->get_name());
	{
		std::lock_guard<std::recursive_mutex> guard(processes_mutex);
		if (has_requested_shutdown())
		{
			if (processes.empty()) return;
			process->request_shutdown();
		}
		processes.push_back(std::move(process));
	}
	run_sync();
	logger->log_task_done(task);
}

void engine::processes_internal_t::run_sync()
{
	while (run_single_if_available(make_id_t<'s','y','n','c'>::value))
	{

	}
}

bool engine::processes_internal_t::has_all_processes_completed()
{
	std::lock_guard<std::recursive_mutex> guard(processes_mutex);

	return processes.empty();
}

void engine::processes_internal_t::shutdown_all()
{
	flags.set_flag(flag_t::requested_shutdown, true);
	std::lock_guard<std::recursive_mutex> guard(processes_mutex);
	for (auto & iter : processes)
	{
		iter->request_shutdown();
	}
}

bool engine::processes_internal_t::is_process_available(process_runner_id_t runner_id)
{
	std::lock_guard<std::recursive_mutex> guard(processes_mutex);
	for (auto & iter : processes)
	{
		if (iter->get_next_runner() == runner_id)
			return true;
	}
	return false;
}

engine::process_t * engine::processes_internal_t::get_process_to_run(process_runner_id_t runner_id)
{
	std::lock_guard<std::recursive_mutex> guard(processes_mutex);
	for (auto & iter : processes)
	{
		if (iter->get_next_runner() == runner_id)
		{
			return iter.get();
		}
	}
	return nullptr;
}

void engine::processes_internal_t::run_process_internal(process_t * process, process_runner_id_t runner_id)
{
	bool process_completed = process->execute(runner_id);
	if (process_completed)
	{
		std::lock_guard<std::recursive_mutex> guard(processes_mutex);
		for (auto iter = processes.begin(); iter != processes.end(); iter++)
		{
			if (iter->get() == process)
			{
				processes.erase(iter);
				break;
			}
		}
	}
	if (runner_id != process_runner_id_t::force_sync)
		run_sync();
}

bool engine::processes_internal_t::run_single_if_available(process_runner_id_t runner_id)
{
	process_t * process_to_run = get_process_to_run();
	
	if(process_to_run == nullptr)
		return false;

	run_process_internal(process_to_run, runner_id);

	return true;
}