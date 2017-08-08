#ifndef ENGINE_COMMON_UTILITY_CONCURRENTION_THRED_POOL_HPP
#define ENGINE_COMMON_UTILITY_CONCURRENTION_THRED_POOL_HPP

#include <queue>
#include <mutex>
#include <memory>
#include <array>
#include <thread>
#include "component/logger.hpp"
#include "utility/pattern/flags.hpp"
#include "utility/container/concurrent_queue.hpp"

#pragma once

namespace engine
{
	class thread_pool_job_t;

	class thread_pool_base_t
	{

	public:

		virtual ~thread_pool_base_t()
		{

		}

		virtual uint32_t get_count() const = 0;

		virtual void enqueue_job(std::unique_ptr<thread_pool_job_t> job) = 0;

		bool is_shutdown_requested()
		{
			return flags.is_flag(flag_t::request_shutdown);
		}

		std::shared_ptr<logger_t> get_logger()
		{
			return logger;
		}

	protected:

		thread_pool_base_t(std::shared_ptr<logger_t> logger) : logger(logger)
		{

		}

		enum class flag_t
		{
			request_shutdown,
			count
		};

		flags_t<flag_t> flags;

	private:

		std::shared_ptr<logger_t> logger;

	};

	class thread_pool_token_t
	{

	public:

		bool is_shutdown_requested()
		{
			return owner->is_shutdown_requested();
		}

		std::shared_ptr<logger_t> get_logger()
		{
			return owner->get_logger();
		}

		template<uint32_t count> friend class thread_pool_t;

	private:

		thread_pool_token_t(thread_pool_base_t * owner) : owner(owner)
		{

		}

		thread_pool_base_t * owner;
		
	};

	class thread_pool_job_t
	{

	public:

		virtual ~thread_pool_job_t()
		{

		}

		template<uint32_t> friend class thread_pool_t;

	private:

		virtual void run(thread_pool_token_t * token) = 0;

		virtual ustring_t get_name() const
		{
			return "Generic Job"_u;
		}
	};

	template<uint32_t count = 4> class thread_pool_t : public thread_pool_base_t
	{

	public:

		thread_pool_t(std::shared_ptr<logger_t> logger) : thread_pool_base_t(logger), token(this)
		{
			auto task = get_logger()->log_task_start(thread_pools, "Creating thread pool of #1# threads"_u, count);

			for (uint32_t i = 0; i < count; i++)
			{
				worker_threads[i] = std::thread([this]() { thread_func(); });
			}

			get_logger()->log_msg(thread_pools, "Thread pool #1# has been created"_u, (void*)this);
			get_logger()->log_task_done(task);
		}
		~thread_pool_t()
		{
			auto task = get_logger()->log_task_start(thread_pools, "Thread pool #2# is being destroyed. #1# threads to be released"_u, count, (void*)this);
			flags.set_flag(flag_t::request_shutdown, true);
			for (uint32_t i = 0; i < count; i++)
			{
				jobs.push(nullptr);
			}
			for (uint32_t i = 0; i < count; i++)
			{
				get_logger()->log_msg(thread_pools, "Waiting for thread #1# in thread pool #4# (#2# out of #3#)..."_u, worker_threads[i].get_id(), (i+1), count, (void*)this);
				try
				{
					worker_threads[i].join();
				}
				catch (const std::system_error& e) {}
			}
			get_logger()->log_msg(thread_pools, "All threads in thread pool #1# has been released"_u, (void*)this);
			get_logger()->log_task_done(task);
		}

		uint32_t get_count() const final
		{
			return count;
		}

		void enqueue_job(std::unique_ptr<thread_pool_job_t> job) final
		{
			if (job)
				jobs.push(std::move(job));
		}

	private:

		std::array<std::thread, count> worker_threads;

		concurrent_queue_t<std::unique_ptr<thread_pool_job_t>> jobs;

		thread_pool_token_t token;

		void thread_func()
		{
			get_logger()->log_msg(thread_pools, "Starting worker thread #1# in thread pool #2#"_u, std::this_thread::get_id(), (void*)this);
			for (;;)
			{
				std::unique_ptr<thread_pool_job_t> job = std::move(jobs.pop());
				if (!job) break;

				get_logger()->log_msg(thread_pools, "Executing '#1#' in worker thread #2# in thread pool #3#"_u, job->get_name(), std::this_thread::get_id(), (void*)this);
				job->run(&token);
				get_logger()->log_msg(thread_pools, "Execution of '#1#' completed in worker thread #2# in thread pool #3#"_u, job->get_name(), std::this_thread::get_id(), (void*)this);
			}
			get_logger()->log_msg(thread_pools, "Worker thread #1# in thread pool #2# has been killed"_u, std::this_thread::get_id(), (void*)this);
		}
	};

}

#endif