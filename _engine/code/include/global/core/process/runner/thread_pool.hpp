#ifndef ENGINE_COMMON_UTILITY_PROCESS_RUNNER_THREAD_POOL_HPP
#define ENGINE_COMMON_UTILITY_PROCESS_RUNNER_THREAD_POOL_HPP

#pragma once

#include "utility/container/concurrent_queue.hpp"
#include "global/core/process/thread_pool.hpp"
#include "global/core/process/task.hpp"
#include "global/core/app.hpp"
#include <mutex>
#include <memory>

namespace engine
{
	namespace process
	{

		class runner_thread_pool_t : public runner_base_t
		{

		public:

			runner_thread_pool_t(std::size_t count, const ustring_t & name, std::shared_ptr<thread_factory_t> thread_factory) : pool(std::make_unique<thread_pool_t>(count, name, thread_factory)), runner_base_t()
			{
				
			}

			runner_thread_pool_t(std::size_t count, app_t::kind_t app, app_t::instance_id_t app_instance_id, const ustring_t & name, std::shared_ptr<thread_factory_t> thread_factory) : pool(std::make_unique<thread_pool_t>(count, app, app_instance_id, name, thread_factory)), runner_base_t()
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
					this->name = task->get_task()->get_name();
				}

			private:

				std::unique_ptr<task_t> task;
				runner_base_t * owner;

				void run(thread_pool_token_t * token) final
				{
					if (task && task->run(owner) == task_base_t::result_t::running)
					{
						static_cast<runner_thread_pool_t*>(owner)->pool->enqueue_job(std::make_unique<job_t>(std::move(task), owner));
						task.reset();
					}
				}

				ustring_t name;

				ustring_t get_name() const
				{
					return name;
				}
			};

			std::unique_ptr<thread_pool_t> pool;

			void add_task_local(std::unique_ptr<task_t> task) final
			{
				pool->enqueue_job(std::make_unique<job_t>(std::move(task), this));
			}

		};

	}
}

#endif