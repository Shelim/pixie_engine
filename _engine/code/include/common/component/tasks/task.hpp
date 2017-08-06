#ifndef ENGINE_COMPONENT_TASKS_TASK_ITEM_HPP
#define ENGINE_COMPONENT_TASKS_TASK_ITEM_HPP
#pragma once

#include <cstdint>
#include "utility/container/concurrent_queue.hpp"

namespace engine
{
	enum class task_thread_t
	{
		force_sync,
#define ENGINE_TASK_THREAD_CALLER_STD(type) std_##type,
#define ENGINE_TASK_THREAD_CALLER_CUSTOM_STD(type) thread_##type,
#include "std/task_std.hpp"
		use_thread_from_pool,
		kill_task,
		count
	};

	class task_base_t
	{

	public:

		virtual ~task_base_t()
		{

		}

		enum class result_t
		{
			running,
			success,
			error,
		};

		bool is_completed()
		{
			return result != result_t::running;
		}

		result_t get_result() const
		{
			return result;
		}

		task_thread_t get_next_thread() const
		{
			return next_thread;
		}

	protected:

		task_base_t() : result(result_t::running), next_thread(task_thread_t::force_sync), next_step(make_id_t<'i','n','i','t'>::value)
		{

		}

		void set_next_thread(task_thread_t new_next_thread)
		{
			next_thread = new_next_thread;
		}

	private:

		result_t result;
		id_t next_step;
		task_thread_t next_thread;

		virtual result_t execute(task_thread_t current_thread) = 0;
	};
	
}

#endif