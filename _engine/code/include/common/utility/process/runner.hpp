#ifndef ENGINE_COMMON_UTILITY_PROCESS_RUNNER_HPP
#define ENGINE_COMMON_UTILITY_PROCESS_RUNNER_HPP

#pragma once

#include <memory>
#include <chrono>
#include <mutex>
#include "utility/pattern/fourcc.hpp"
#include "utility/process/task.hpp"

namespace engine
{
	class logger_t;

	namespace process
	{

		class runner_base_t
		{

		public:

			runner_base_t(std::shared_ptr<logger_t> logger) : logger(logger)
			{

			}

			virtual ~runner_base_t()
			{

			}

			enum class result_t
			{
				ok,
				error
			};

			result_t add_task(std::unique_ptr<task_base_t> task)
			{
				return add_looped_task(std::move(task), 1);
			}
			
			result_t add_looped_task(std::unique_ptr<task_base_t> task, std::size_t loop_count = 0, std::chrono::duration<double> sleep_after = std::chrono::milliseconds(0));

			bool is_requested_shutdown()
			{
				std::lock_guard<std::recursive_mutex> guard(flag_mutex);
				return flags.is_flag(flag_t::is_requested_shutdown);
			}

		protected:

			class task_t
			{

			public:

				task_base_t * get_task()
				{
					return task.get();
				}

				std::size_t get_loop_count() const
				{
					return loop_count;
				}

				std::size_t get_loop_remains() const
				{
					return loop_remains;
				}

				std::chrono::duration<double> get_sleep_after() const
				{
					return sleep_after;
				}

				task_base_t::result_t run(runner_base_t * owner)
				{
					if (loop_count == 0 || loop_remains > 0)
					{
						token_t token(owner);
						if (task->run(&token) != task_base_t::result_t::running)
						{
							return task_base_t::result_t::completed;
						}
						if (loop_count > 0)
							loop_remains--;

						if (sleep_after > std::chrono::milliseconds(0))
						{
							std::this_thread::sleep_for(sleep_after);
						}
					}
					if (loop_count > 0 && loop_remains == 0)
						return task_base_t::result_t::completed;
					if (loop_count == 0 && owner->is_requested_shutdown())
						return task_base_t::result_t::completed;
					return task_base_t::result_t::running;
				}

				task_t(std::unique_ptr<task_base_t> task, std::size_t loop_count = 0, std::chrono::duration<double> sleep_after = std::chrono::milliseconds(0)) :
					task(std::move(task)), loop_count(loop_count), loop_remains(loop_count), sleep_after(sleep_after)
				{

				}

			private:

				std::unique_ptr<task_base_t> task;
				std::size_t loop_count;
				std::size_t loop_remains;
				std::chrono::duration<double> sleep_after;
			};

			void request_shutdown()
			{
				flags.set_flag(flag_t::is_requested_shutdown, true);
			}

			std::shared_ptr<logger_t> get_logger()
			{
				return logger;
			}

		private:

			enum class flag_t
			{
				is_requested_shutdown,

				count
			};

			std::recursive_mutex flag_mutex;
			std::shared_ptr<logger_t> logger;
			flags_t<flag_t> flags;

			virtual void add_task_local(std::unique_ptr<task_t> task) = 0;
		};

	}
}

#include "utility/process/runner/engine_thread.hpp"
#include "utility/process/runner/spawn.hpp"
#include "utility/process/runner/sync.hpp"
#include "utility/process/runner/thread_pool.hpp"

#endif