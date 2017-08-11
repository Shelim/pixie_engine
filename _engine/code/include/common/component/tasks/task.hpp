#ifndef ENGINE_COMPONENT_TASKS_TASK_ITEM_HPP
#define ENGINE_COMPONENT_TASKS_TASK_ITEM_HPP
#pragma once

#include <cstdint>
#include "utility/container/concurrent_queue.hpp"
#include "utility/pattern/flags.hpp"

namespace engine
{

	class task_base_t
	{

	public:

		enum class runner_t
		{
			force_sync,
#define ENGINE_TASK_THREAD_CALLER_STD(type) std_##type,
#define ENGINE_TASK_THREAD_CALLER_CUSTOM_STD(type) thread_##type,
#include "std/task_std.hpp"
			use_thread_from_pool,
			kill_task,
			count
		};

		virtual ~task_base_t()
		{

		}

		enum class flag_t
		{
			running,
			requested_shutdown,

			count
		};		

		bool has_completed()
		{
			return !flags.is_flag(flag_t::running);
		}

		bool is_requested_shutdown()
		{
			return flags.is_flag(flag_t::requested_shutdown);
		}

		runner_t get_next_runner() const
		{
			return next_runner;
		}

	protected:

		task_base_t() : next_runner(runner_t::force_sync)
		{
			flags.set_flag(flag_t::running, true);
		}

		void request_shutdown()
		{
			flags.set_flag(flag_t::requested_shutdown, true);
		}

		void set_next_runner(runner_t runner)
		{
			next_runner = runner;
		}

	private:

		flags_t<flag_t> flags;
		runner_t next_runner;

		virtual bool execute(runner_t current_thread) = 0;
	};
}

#endif