#include "utility/process/process.hpp"
#include "utility/process/runner.hpp"
#include "utility/process/runner/engine_thread.hpp"
#include "utility/process/runner/spawn.hpp"
#include "utility/process/runner/sync.hpp"
#include "utility/process/runner/thread_pool.hpp"
#include "utility/process/task.hpp"
#include "utility/process/token.hpp"
#include "utility/process/service.hpp"

bool engine::process::token_t::is_requested_shutdown()
{
	return runner->is_requested_shutdown();
}

engine::process::runner_base_t::result_t engine::process::runner_base_t::add_looped_task(std::unique_ptr<task_base_t> task, std::size_t loop_count, std::chrono::duration<double> sleep_after)
{
	auto task_id = logger->log_task_start(process, "Adding task #1# to runner"_u, task->get_name());
	std::lock_guard<std::recursive_mutex> guard(flag_mutex);
	if (!is_requested_shutdown())
	{
		add_task_local(std::make_unique<task_t>(std::move(task), loop_count, sleep_after));
		logger->log_task_done(task_id);
		return result_t::ok;
	}
	else
	{
		logger->log_warn(process, "Failed to add task #1# to runner -> this runner is shutting down right now!"_u, task->get_name());
		logger->log_task_failed(task_id);
		return result_t::error;
	}
}

engine::process::runner_sync_t::runner_sync_t(std::shared_ptr<logger_t> logger) : runner_base_t(logger)
{
	get_logger()->log_msg(process, "Starting up thread runner for sync tasks"_u);
}

engine::process::runner_sync_t::~runner_sync_t()
{
	request_shutdown();
	get_logger()->log_msg(process, "Sync runner is destroyed"_u);
}

void engine::process::runner_spawn_t::thread_func()
{
	get_logger()->log_msg(process, "Starting thread function for spawned thread"_u);

	for (;;)
	{
		std::unique_ptr<task_t> task = std::move(tasks.pop());
		if (!task) break;

		if (task->run(this) == task_base_t::result_t::running)
			tasks.push(std::move(task));
	}
	get_logger()->log_msg(process, "Thread function for spawned thread completed!"_u);
}

engine::process::runner_spawn_t::runner_spawn_t(std::shared_ptr<logger_t> logger) : runner_base_t(logger), thread([this]() {thread_func(); })
{
	get_logger()->log_msg(process, "Starting up spawn thread runner (this message may appear multiple times)"_u);
}

engine::process::runner_spawn_t::~runner_spawn_t()
{
	auto task_id = get_logger()->log_task_start(process, "Spawned runner is terminating itself"_u);
	{
		request_shutdown();
		tasks.push(nullptr);
	}

	try
	{
		thread.join();
	}
	catch (const std::system_error& e) {}

	get_logger()->log_task_done(task_id);
}