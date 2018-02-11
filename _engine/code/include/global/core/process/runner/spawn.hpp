#ifndef ENGINE_COMMON_UTILITY_PROCESS_RUNNER_SPAWN_HPP
#define ENGINE_COMMON_UTILITY_PROCESS_RUNNER_SPAWN_HPP

#pragma once

#include "utility/container/concurrent_queue.hpp"
#include "global/core/app.hpp"
#include <mutex>

namespace engine
{
	class thread_factory_t;
	class thread_t;
	
	namespace process
	{

		class runner_spawn_t : public runner_base_t
		{

		public:

			runner_spawn_t(const ustring_t & name, std::shared_ptr<thread_factory_t> thread_factory);
			runner_spawn_t(app_t::kind_t app, app_t::instance_id_t app_instance_id, const ustring_t & name, std::shared_ptr<thread_factory_t> thread_factory);

			~runner_spawn_t();

		private:

			void add_task_local(std::unique_ptr<task_t> task) final
			{
				tasks.push(std::move(task));
			}

			concurrent_queue_t<std::unique_ptr<task_t> > tasks;
			std::unique_ptr<thread_t> thread;
			std::mutex add_task_mutex;

			void thread_func();

		};

	}
}

#endif