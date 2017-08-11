#ifndef ENGINE_COMPONENT_TASKS_DUMMY_HPP
#define ENGINE_COMPONENT_TASKS_DUMMY_HPP
#pragma once

#include "component/tasks.hpp"
#include <thread>

namespace engine
{
	class tasks_dummy_t : public tasks_t
	{

	public:

		void start_task(std::unique_ptr<task_base_t> task) final
		{
			// Do nothing, it is dummy.
		}

		class runner_dummy_t : public tasks_t::runner_t
		{

		public:

			void update_top_task() final
			{
				// Do nothing, it is dummy.
			}
			void update_tasks(std::chrono::duration<double> min_available_time) final
			{
				std::this_thread::sleep_for(min_available_time);
			}
		};

		std::unique_ptr<runner_t> construct_runner(task_base_t::runner_t runner) final
		{
			return std::make_unique<runner_dummy_t>();
		}

	private:



	};

}

#endif