#ifndef ENGINE_COMMON_UTILITY_PROCESS_PROCESSES_HPP
#define ENGINE_COMMON_UTILITY_PROCESS_PROCESSES_HPP

#pragma once

#include <memory>
#include <mutex>
#include "component/logger.hpp"
#include "utility/process/runner.hpp"
#include "utility/concurrention/thread_pool.hpp"

namespace engine
{
	class process_t;
	
	class processes_internal_t
	{

	public:

		processes_internal_t(std::shared_ptr<logger_t> logger);

	private:

		enum class flag_t
		{
			requested_shutdown,

			count
		};
		flags_t<flag_t> flags;
		
		friend class processes_t;

		template<process_runner_id_t id> friend class process_runner_t;


		void start_process(std::unique_ptr<process_t> process);

		void shutdown_all();

		bool is_process_available(process_runner_id_t runner_id);
		process_t * get_process_to_run(process_runner_id_t runner_id);
		void run_process_internal(process_t * process, process_runner_id_t runner_id);
		bool run_single_if_available(process_runner_id_t runner_id);
		bool has_all_processes_completed();

		void run_sync();

		bool has_requested_shutdown()
		{
			return flags.is_flag(flag_t::requested_shutdown);
		}

		std::vector<std::unique_ptr<process_t> > processes;
		std::recursive_mutex processes_mutex;

		std::shared_ptr<logger_t> logger;

		class process_job_t final : public engine::thread_pool_job_t
		{

		public:

			process_job_t(std::shared_ptr<processes_internal_t> processes, process_t * process_to_run) : processes(processes), process_to_run(process_to_run)
			{

			}

			engine::ustring_t get_name() const final
			{
				return format_string("Thread for: #1#"_u, process_to_run->get_name());
			}

			void run(engine::thread_pool_token_t * token) final
			{
			}

		private:
			std::shared_ptr<processes_internal_t> processes;
			process_t * process_to_run;
		};

		thread_pool_t<> thread_pool;

	};


	class processes_t
	{

	public:

		processes_t(std::shared_ptr<processes_internal_t> processes_internal) : processes_internal(processes_internal)
		{

		}

		~processes_t()
		{
			processes_internal->shutdown_all();
		}


		void start_process(std::unique_ptr<process_t> process)
		{
			processes_internal->start_process(std::move(process));
		}

	private:

		std::shared_ptr<processes_internal_t> processes_internal;
	};
}

#endif