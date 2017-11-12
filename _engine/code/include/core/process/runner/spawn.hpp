#ifndef ENGINE_COMMON_UTILITY_PROCESS_RUNNER_SPAWN_HPP
#define ENGINE_COMMON_UTILITY_PROCESS_RUNNER_SPAWN_HPP

#pragma once

#include "core/process/runner.hpp"
#include "utility/container/concurrent_queue.hpp"
#include <mutex>

namespace engine
{
	namespace process
	{

		class runner_spawn_t : public runner_base_t
		{

		public:

			runner_spawn_t();

			~runner_spawn_t();

		private:

			void add_task_local(std::unique_ptr<task_t> task) final
			{
				tasks.push(std::move(task));
			}

			concurrent_queue_t<std::unique_ptr<task_t> > tasks;
			std::thread thread;
			std::mutex add_task_mutex;

			void thread_func();

		};

	}
}

#endif