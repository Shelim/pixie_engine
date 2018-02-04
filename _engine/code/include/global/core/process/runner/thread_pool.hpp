#ifndef ENGINE_COMMON_UTILITY_PROCESS_RUNNER_THREAD_POOL_HPP
#define ENGINE_COMMON_UTILITY_PROCESS_RUNNER_THREAD_POOL_HPP

#pragma once

#include "global/core/process/runner.hpp"
#include "utility/container/concurrent_queue.hpp"
#include "utility/concurrention/thread_pool.hpp"
#include <mutex>

namespace engine
{
	namespace process
	{

		template<uint32_t count = 4> class runner_thread_pool_t : public runner_base_t
		{

		public:

			runner_thread_pool_t() : pool(std::make_unique<std::unique_ptr<thread_pool_t<count> >>()), runner_base_t()
			{
				
			}

			~runner_thread_pool_t()
			{
				request_shutdown();
			}

		private:

			friend class job_t;

			class job_t : public thread_pool_job_t
			{

			public:

				job_t(std::unique_ptr<task_t> task, runner_base_t * owner) : task(std::move(task)), owner(owner)
				{

				}

			private:

				std::unique_ptr<task_t> task;
				runner_base_t * owner;

				void run(thread_pool_token_t * token) final
				{
					if (task->run(owner) == task_base_t::result_t::running)
					{
						static_cast<runner_thread_pool_t<count>*>(owner)->pool->enqueue_job(std::make_unique<job_t>(task, owner));
					}
				}

				ustring_t get_name() const
				{
					return task->get_task()->get_name();
				}
			};

			std::unique_ptr<thread_pool_t<count> > pool;

			void add_task_local(std::unique_ptr<task_t> task) final
			{
				pool->enqueue_job(std::make_unique<job_t>(task, this));
			}

		};

	}
}

#endif