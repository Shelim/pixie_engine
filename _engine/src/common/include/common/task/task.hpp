#ifndef ENGINE_COMMON_TASK_HPP
#define ENGINE_COMMON_TASK_HPP
#pragma once

#include "common/logger.hpp"
#include <bitset>
#include <map>
#include <vector>
#include <string>
#include <unordered_set>
#include <thread>
#include <memory>
#include <queue>
#include <mutex>

namespace engine
{
	/*
	namespace task
	{
		enum class caller_t
		{
#define GAME_TASK_THREAD_CALLER_STD(type) thread_##type,
#define GAME_TASK_THREAD_CALLER_CUSTOM_STD(type) thread_##type,
#include "common/std/task_std.hpp"
			force_sync,
			spawn_new
		};

		class step_t final
		{

		public:

			step_t(const step_t & other) = default;

			uint32_t get_id() const
			{
				return id;
			}

			task::caller_t get_caller() const
			{
				return caller;
			}

		private:

			step_t(uint32_t id, task::caller_t caller) : id(id), caller(caller)
			{

			}

			friend class steps_t;

			uint32_t id;
			task::caller_t caller;

		};

		class steps_t final
		{

		public:


		private:

		};
	}

	class task_base_t
	{

	public:
		
		virtual ~task_base_t()
		{
			spawned_thread.join();
		}

		enum class result_t
		{
			running,
			success,
			error,
		};

		bool is_completed()
		{
			return is_flag(flag_t::has_completed_executed);
		}

		result_t get_result() const
		{
			return result;
		}

		void add_step(uint32_t id, task::caller_t caller)
		{
			steps.push(step_t(id, caller));
		}

	protected:

		task_base_t() : result(result_t::running)
		{
			add_step('INIT', task::caller_t::force_sync);
		}

	private:

		friend class tasks_t;

		enum class flag_t
		{
			has_completed_executed,

			count
		};

		result_t result;

		void execute_steps(task::caller_t caller)
		{
			while (!steps.empty())
			{
				step_t step = steps.front();
				if (step.caller == task::caller_t::spawn_new)
				{
					spawned_thread = std::thread([this, &step] { if (!execute_step(step)) { result = result_t::error; set_completed(); } });
					return;
				}
				else if (step.caller != caller && step.caller != task::caller_t::force_sync)
				{
					return;
				}
				else if (!execute_step(step))
				{
					result = result_t::error;
					set_completed();
					return;
				}
			}

			result = result_t::success;
			set_completed();
		}

		void set_completed()
		{
			set_flag(flag_t::has_completed_executed, true);
		}

		std::queue<step_t> steps;

		std::thread spawned_thread;

		std::bitset<static_cast<std::size_t>(flag_t::count)> flags;

		void set_flag(flag_t flag, bool value)
		{
			flags.set(static_cast<std::size_t>(flag), value);
		}

		bool is_flag(flag_t flag) const
		{
			return flags.test(static_cast<std::size_t>(flag));
		}

		virtual bool execute_step(const step_t & step) = 0;

	};
	*/
}


#endif
