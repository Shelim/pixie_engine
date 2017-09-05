#ifndef ENGINE_COMMON_UTILITY_PROCESS_RUNNER_SYNC_HPP
#define ENGINE_COMMON_UTILITY_PROCESS_RUNNER_SYNC_HPP

#pragma once

#include "utility/process/runner.hpp"

namespace engine
{
	namespace process
	{

		class runner_sync_t : public runner_base_t
		{

		public:

			runner_sync_t(std::shared_ptr<logger_t> logger);
			~runner_sync_t();

		private:

			void add_task_local(std::unique_ptr<task_t> task) final
			{
				while (task->run(this) == task_base_t::result_t::running)
				{
					
				}
			}
		};

	}
}

#endif