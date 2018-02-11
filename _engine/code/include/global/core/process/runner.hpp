#ifndef ENGINE_COMMON_UTILITY_PROCESS_RUNNER_HPP
#define ENGINE_COMMON_UTILITY_PROCESS_RUNNER_HPP

#pragma once

#include <memory>
#include <chrono>
#include <mutex>
#include "utility/pattern/fourcc.hpp"
#include "global/core/process/task.hpp"
#include "global/core/app.hpp"

namespace engine
{
	namespace process
	{		
		typedef std::function<void()> callback_t;

		class runner_base_t
		{

		public:

			runner_base_t()
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

			result_t add_task(std::unique_ptr<task_base_t> task, callback_t on_completion = {})
			{
				return add_looped_task(std::move(task), on_completion, 1);
			}
			
			result_t add_looped_task(std::unique_ptr<task_base_t> task, callback_t on_completion = {}, std::size_t loop_count = 0, std::chrono::duration<double> sleep_after = std::chrono::milliseconds(0));

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

				callback_t get_callback_on_completion() const
				{
					return on_completion;
				}

				task_base_t::result_t run(runner_base_t * owner)
				{
					if (loop_count == 0 || loop_remains > 0)
					{
						token_t token(owner);
						if (task->run(&token) != task_base_t::result_t::running)
						{
							if(on_completion) on_completion();
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
					{
						if(on_completion) on_completion();
						return task_base_t::result_t::completed;
					}
					if (loop_count == 0 && owner->is_requested_shutdown())
					{
						if(on_completion) on_completion();
						return task_base_t::result_t::completed;
					}
					return task_base_t::result_t::running;
				}

				task_t(std::unique_ptr<task_base_t> task, callback_t on_completion = {}, std::size_t loop_count = 0, std::chrono::duration<double> sleep_after = std::chrono::milliseconds(0)) :
					task(std::move(task)), loop_count(loop_count), loop_remains(loop_count), sleep_after(sleep_after)
				{

				}

			private:

				std::unique_ptr<task_base_t> task;
				std::size_t loop_count;
				std::size_t loop_remains;
				std::chrono::duration<double> sleep_after;
				callback_t on_completion;
			};

			void request_shutdown()
			{
				flags.set_flag(flag_t::is_requested_shutdown, true);
			}

		private:

			enum class flag_t
			{
				is_requested_shutdown,

				count
			};

			std::recursive_mutex flag_mutex;
			flags_t<flag_t> flags;

			virtual void add_task_local(std::unique_ptr<task_t> task) = 0;
		};

	}
}

#include "global/core/process/runner/program_thread.hpp"
#include "global/core/process/runner/spawn.hpp"
#include "global/core/process/runner/sync.hpp"
#include "global/core/process/runner/thread_pool.hpp"


namespace engine
{
	namespace process
	{	

		class runner_spawn_factory_t
		{

			public:

				runner_spawn_factory_t(std::shared_ptr<thread_factory_t> thread_factory) : thread_factory(thread_factory)
				{

				}

				std::unique_ptr<runner_spawn_t> create(const ustring_t & name)
				{
					return std::make_unique<runner_spawn_t>(name, thread_factory);
				}

				std::unique_ptr<runner_spawn_t> create(app_t::kind_t app, app_t::instance_id_t app_instance_id, const ustring_t & name)
				{
					return std::make_unique<runner_spawn_t>(app, app_instance_id, name, thread_factory);
				}

			private:

				std::shared_ptr<thread_factory_t> thread_factory;

		};

		class runner_thread_pool_factory_t
		{

			public:

				runner_thread_pool_factory_t(std::shared_ptr<thread_factory_t> thread_factory) : thread_factory(thread_factory)
				{

				}

				std::unique_ptr<runner_thread_pool_t> create(std::size_t count, const ustring_t & name)
				{
					return std::make_unique<runner_thread_pool_t>(count, name, thread_factory);
				}

				std::unique_ptr<runner_thread_pool_t> create(std::size_t count, app_t::kind_t app, app_t::instance_id_t app_instance_id, const ustring_t & name)
				{
					return std::make_unique<runner_thread_pool_t>(count, app, app_instance_id, name, thread_factory);
				}

			private:

				std::shared_ptr<thread_factory_t> thread_factory;

		};

	}
}

#endif