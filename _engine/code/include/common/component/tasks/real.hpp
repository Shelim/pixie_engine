#ifndef ENGINE_COMPONENT_TASKS_REAL_HPP
#define ENGINE_COMPONENT_TASKS_REAL_HPP
#pragma once

#include "component/tasks.hpp"
#include "utility/container/sync_queue.hpp"
#include <thread>

namespace engine
{
	class tasks_real_t: public tasks_t, public std::enable_shared_from_this<tasks_real_t>
	{

	public:

		void start_task(std::unique_ptr<task_base_t> task) final
		{
		}

		friend class runner_real_t;

		class runner_real_t : public tasks_t::runner_t
		{

		public:

			runner_real_t(std::shared_ptr<tasks_real_t> owner, task_base_t::runner_t runner) : owner(owner), runner(runner)
			{

			}

			~runner_real_t()
			{

			}

			void update_top_task() final
			{

			}
			void update_tasks(std::chrono::duration<double> min_available_time) final
			{

			}

		private:

			sync_queue_t<std::unique_ptr<task_base_t> > tasks;
			std::shared_ptr<tasks_real_t> owner;
			task_base_t::runner_t runner;

		};

		std::unique_ptr<runner_t> construct_runner(task_base_t::runner_t runner) final
		{
			return std::make_unique<runner_real_t>(shared_from_this(), runner);
		}

		tasks_real_t(std::shared_ptr<messenger_t> messenger, std::shared_ptr<logger_t> logger) : messenger(messenger), logger(logger)
		{
			logger->log_msg(tasks, "Task component is ready to accept tasks"_u);
		}

		~tasks_real_t()
		{
			logger->log_msg(tasks, "Task component has been destroyed"_u);
		}

	private:

		std::vector<std::unique_ptr<task_base_t> > available_tasks;
		std::shared_ptr<messenger_t> messenger;
		std::shared_ptr<logger_t> logger;

	};

}

#endif