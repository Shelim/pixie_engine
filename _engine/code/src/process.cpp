#include "global/core/process/process.hpp"
#include "global/core/process/runner.hpp"
#include "global/core/process/runner/program_thread.hpp"
#include "global/core/process/runner/spawn.hpp"
#include "global/core/process/runner/sync.hpp"
#include "global/core/process/runner/thread_pool.hpp"
#include "global/core/process/task.hpp"
#include "global/core/process/token.hpp"
#include "global/core/process/service.hpp"
#include "global/core/process/thread_pool.hpp"
#include "global/core/thread/thread.hpp"

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

engine::process::runner_spawn_t::runner_spawn_t(const ustring_t & name, std::shared_ptr<thread_factory_t> thread_factory) : runner_base_t()
{
	thread = thread_factory->create(name, [this]{thread_func();});
}

engine::process::runner_spawn_t::runner_spawn_t(app_t::kind_t app, app_t::instance_id_t app_instance_id, const ustring_t & name, std::shared_ptr<thread_factory_t> thread_factory) : runner_base_t()
{
	thread = thread_factory->create(app, app_instance_id, name, [this]{thread_func();});
}


engine::process::runner_spawn_t::~runner_spawn_t()
{
	{
		request_shutdown();
		tasks.push(nullptr);
	}
}

bool engine::thread_pool_token_t::is_shutdown_requested()
{
	return owner->is_shutdown_requested();
}

engine::thread_pool_t::thread_pool_t(std::size_t count, const ustring_t & name, std::shared_ptr<thread_factory_t> thread_factory) : count(count), token(this)
{
	worker_threads.resize(count);
	for (uint32_t i = 0; i < count; i++)
	{
		ustring_t instance_name = format_string("#1# (#2# of #3#)"_u, name, (i+1), count);
		worker_threads[i] = thread_factory->create(instance_name, [this]() { thread_func(); });
	}
}

engine::thread_pool_t::thread_pool_t(std::size_t count, app_t::kind_t app, app_t::instance_id_t app_instance_id, const ustring_t & name, std::shared_ptr<thread_factory_t> thread_factory) : count(count), token(this)
{
	worker_threads.resize(count);
	for (uint32_t i = 0; i < count; i++)
	{
		ustring_t instance_name = format_string("#1# (#2# of #3#)"_u, name, (i+1), count);
		worker_threads[i] = thread_factory->create(app, app_instance_id, instance_name, [this]() { thread_func(); });
	}
}

engine::thread_pool_t::~thread_pool_t()
{
	flags.set_flag(flag_t::request_shutdown, true);
	for (uint32_t i = 0; i < count; i++)
	{
		jobs.push(nullptr);
	}
}