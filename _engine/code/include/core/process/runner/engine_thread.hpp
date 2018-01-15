#ifndef ENGINE_COMMON_UTILITY_PROCESS_RUNNER_ENGINE_THREAD_HPP
#define ENGINE_COMMON_UTILITY_PROCESS_RUNNER_ENGINE_THREAD_HPP

#pragma once

#include "core/process/runner.hpp"
#include "utility/container/sync_queue.hpp"

namespace engine
{
	namespace process
	{

#include "def/enum/process_runner.def"

		template<runner_thread_t thread> class runner_engine_thread_t : public runner_base_t
		{

		public:

			runner_engine_thread_t() : runner_base_t()
			{
				
			}


			~runner_engine_thread_t()
			{
				
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
				for (std::size_t i = 0; i < count; i++)
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
				request_shutdown();
				run_all_available();
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

#define ENGINE_ENUM_HEADER_TO_USE "def/enum/process_runner.def"
#include "core/utility/enum_to_string.hpp"

#endif