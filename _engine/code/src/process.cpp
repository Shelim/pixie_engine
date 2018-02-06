#include "global/core/process/process.hpp"
#include "global/core/process/runner.hpp"
#include "global/core/process/runner/program_thread.hpp"
#include "global/core/process/runner/spawn.hpp"
#include "global/core/process/runner/sync.hpp"
#include "global/core/process/runner/thread_pool.hpp"
#include "global/core/process/task.hpp"
#include "global/core/process/token.hpp"
#include "global/core/process/service.hpp"

bool engine::process::token_t::is_requested_shutdown()
{
	return runner->is_requested_shutdown();
}

engine::process::runner_base_t::result_t engine::process::runner_base_t::add_looped_task(std::unique_ptr<task_base_t> task, callback_t on_completion, std::size_t loop_count, std::chrono::duration<double> sleep_after)
{
	std::lock_guard<std::recursive_mutex> guard(flag_mutex);
	if (!is_requested_shutdown())
	{
		add_task_local(std::make_unique<task_t>(std::move(task), on_completion, loop_count, sleep_after));
		return result_t::ok;
	}
	else
	{
		return result_t::error;
	}
}

engine::process::runner_sync_t::runner_sync_t() : runner_base_t()
{
}

engine::process::runner_sync_t::~runner_sync_t()
{
	request_shutdown();
}

void engine::process::runner_spawn_t::thread_func()
{
	for (;;)
	{
		std::unique_ptr<task_t> task = std::move(tasks.pop());
		if (!task) break;

		if (task->run(this) == task_base_t::result_t::running)
			tasks.push(std::move(task));
	}
}

engine::process::runner_spawn_t::runner_spawn_t() : runner_base_t(), thread([this]() {thread_func(); })
{
}

engine::process::runner_spawn_t::~runner_spawn_t()
{
	{
		request_shutdown();
		tasks.push(nullptr);
	}

	try
	{
		thread.join();
	}
	catch (const std::system_error& e) {}
}