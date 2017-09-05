#ifndef ENGINE_COMMON_UTILITY_PROCESS_RUNNER_ENGINE_THREAD_HPP
#define ENGINE_COMMON_UTILITY_PROCESS_RUNNER_ENGINE_THREAD_HPP

#pragma once

#include "utility/process/runner.hpp"
#include "utility/container/sync_queue.hpp"

namespace engine
{
	namespace process
	{

		enum class runner_thread_t
		{
#define ENGINE_TASK_THREAD_CALLER_STD(name) engine_##name,
#define ENGINE_TASK_THREAD_CALLER_CUSTOM_STD(name) custom_##name,
#include "std/process_runner_std.hpp"
			count
		};

		template<runner_thread_t thread> class runner_engine_thread_t : public runner_base_t
		{

		public:

			static ustring_t to_string(runner_thread_t runner)
			{
#define ENGINE_TASK_THREAD_CALLER_STD(name) if(runner == runner_thread_t::engine_##name) return "Internal " #name##_u;
#define ENGINE_TASK_THREAD_CALLER_CUSTOM_STD(name) if(runner == runner_thread_t::custom_##name) return "Thread " #name##_u;
#include "std/process_runner_std.hpp"
				return "Unknown"_u;
			}

			runner_engine_thread_t(std::shared_ptr<logger_t> logger) : runner_base_t(logger)
			{
				get_logger()->log_msg(process, "Starting up thread runner for #1#"_u, to_string(thread));
			}


			~runner_engine_thread_t()
			{
				if (!is_requested_shutdown())
					get_logger()->log_warn(process, "Runner for #1# is dying before requesting its shutdown; Most likely call to run_on_shutdown() is missing!"_u, to_string(thread));

				get_logger()->log_msg(process, "Runner #1# is destroyed"_u, to_string(thread));
			}

			enum single_task_result_t
			{
				success,
				no_task_available
			};

			single_task_result_t run_single_task()
			{
				if (!tasks.is_empty())
				{
					std::unique_ptr<task_t> task = std::move(tasks.pop());
					
					if (task->run(this) == task_base_t::result_t::running)
						tasks.push(std::move(task));

					return single_task_result_t::success;
				}
				return single_task_result_t::no_task_available;
			}

			void run_multiple_tasks(std::size_t count = 1)
			{
				for (std : size_t i = 0; i < count; i++)
				{
					run_single_task();
				}
			}

			void run_tasks_for(std::chrono::duration<double> min_available_time)
			{
				std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
				std::chrono::steady_clock::time_point now = start;

				while (now - start <= min_available_time)
				{
					if (run_single_task() == single_task_result_t::no_task_available)
						return;
					now = std::chrono::steady_clock::now();
				}
			}

			void run_all_available()
			{
				while (!tasks.is_empty())
					run_single_task();
			}

			void run_all_available_once()
			{
				std::size_t count = tasks.size();
				for(std::size_t i = 0; i < count; i++)
				{
					std::unique_ptr<task_t> task = std::move(tasks.pop());
					
					if (task->run(this) == task_base_t::result_t::running)
						tasks.push(std::move(task));
				}
			}

			void run_on_shutdown()
			{
				if (is_requested_shutdown())
				{
					get_logger()->log_warn(process, "Requested shutdown for runner #1# for the second time!"_u, to_string(thread));
				}
				else
				{
					get_logger()->log_msg(process, "Requested shutdown for runner #1#"_u, to_string(thread));
					request_shutdown();
					run_all_available();
				}
			}

		private:

			void add_task_local(std::unique_ptr<task_t> task) final
			{
				tasks.push(std::move(task));
			}

			sync_queue_t<std::unique_ptr<task_t>> tasks;

		};

	}
}

#endif