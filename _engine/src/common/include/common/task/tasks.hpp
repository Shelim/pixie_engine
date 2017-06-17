#ifndef ENGINE_COMMON_TASKS_HPP
#define ENGINE_COMMON_TASKS_HPP
#pragma once

#include "common/task/task.hpp"
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
	class tasks_t final
	{

	};
	/*
	class tasks_t final
	{

	public:

		tasks_t(std::shared_ptr<logger_t> logger) : logger(logger), end_custom_threads(false)
		{
			main_thread_id = std::this_thread::get_id();

			for (std::size_t i = 0; i < static_cast<std::size_t>(custom_threads_t::count); i++)
			{
				custom_threads[i] = std::thread([this, i] { custom_thread(static_cast<custom_threads_t>(i)); });
			}
		}

		~tasks_t()
		{
			end_custom_threads = true;
			for (std::size_t i = 0; i < static_cast<std::size_t>(custom_threads_t::count); i++)
			{
				custom_threads[i].join();
			}
		}

		void run_task(std::unique_ptr<task_base_t> task)
		{
			std::lock_guard<std::recursive_mutex> guard(task_list_mutex);
			tasks.push_back(std::move(task));
			execute_pending_tasks(task::caller_t::force_sync);
		}

		void init_update()
		{
			execute_pending_tasks(task::caller_t::thread_main);
			
			clear_completed_operations();
		}

		void update_from_renderer()
		{
			execute_pending_tasks(task::caller_t::thread_renderer);
		}

		void execute_pending_tasks(task::caller_t caller)
		{
			std::lock_guard<std::recursive_mutex> guard(task_list_mutex);
			for (std::size_t op = tasks.size(); op--> 0;)
			{
				tasks[op]->execute_steps(caller);
			}
		}

	private:

		enum class custom_threads_t
		{
#define GAME_TASK_THREAD_CALLER_CUSTOM_STD(name) name,
#include "common/std/task_std.hpp"
			count
		};

		std::array<std::thread, static_cast<std::size_t>(custom_threads_t::count)> custom_threads;

		bool end_custom_threads;

		std::thread::id main_thread_id;

		void custom_thread(custom_threads_t id)
		{

			task::caller_t caller = task::caller_t::force_sync;

#define GAME_TASK_THREAD_CALLER_CUSTOM_STD(name) if(id == custom_threads_t::name) caller = task::caller_t::thread_##name

			if (caller == task::caller_t::force_sync) return;

			for (;;)
			{
				if (end_custom_threads) break;

				execute_pending_tasks(caller);
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			}
		}

		void clear_completed_operations()
		{
			std::lock_guard<std::recursive_mutex> guard(task_list_mutex);

			for (std::size_t op = tasks.size(); op-- > 0;)
			{
				if (tasks[op]->is_completed())
					tasks.erase(tasks.begin() + op);
			}
		}

		std::shared_ptr<logger_t> logger;

		std::vector<std::unique_ptr<task_base_t> > tasks;

		std::recursive_mutex task_list_mutex;
	};
	*/
}

#endif