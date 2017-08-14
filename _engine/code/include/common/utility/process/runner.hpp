#ifndef ENGINE_COMMON_UTILITY_PROCESS_RUNNER_HPP
#define ENGINE_COMMON_UTILITY_PROCESS_RUNNER_HPP

#pragma once

#include <memory>
#include <chrono>
#include "utility/pattern/fourcc.hpp"

namespace engine
{
	class processes_internal_t;

	enum class process_runner_id_t : uint32_t
	{
#define ENGINE_PROCESS_RUNNER_STD(name, x1, x2, x3, x4) name = make_id_t<x1,x2,x3,x4>::value,
#include "std/process_runner_std.hpp"
		count
	};

	template<process_runner_id_t id> class process_runner_t
	{

	public:

		process_runner_t(std::shared_ptr<processes_internal_t> processes) : processes(processes)
		{

		}

		~process_runner_t()
		{
			run_until_app_exits();
		}

		bool run_single_if_available()
		{
			return processes->run_single_if_available(get_id());
		}

		void run_for(std::chrono::duration<double> min_available_time)
		{
			std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
			std::chrono::steady_clock::time_point now = start;

			while (now - start <= min_available_time)
			{
				if (!run_single_if_available())
					return;
				now = std::chrono::steady_clock::now();
			}
		}

		void run_until_there_are_no_more_jobs()
		{
			while (run_single_if_available())
			{

			}
		}

		void run_until_app_exits()
		{
			while (!processes->has_requested_shutdown() && !processes->has_all_processes_completed())
			{
				run_single_if_available();
			}
		}

		constexpr process_runner_id_t get_id()
		{
			return id;
		}

	private:

		std::shared_ptr<processes_internal_t> processes;

	};

#define ENGINE_PROCESS_RUNNER_STD(name, x1, x2, x3, x4) typedef process_runner_t<process_runner_id_t::name> proces_runner_##name##_t;
#include "std/process_runner_std.hpp"
}

#endif