#ifndef ENGINE_COMMON_UTILITY_CONCURRENTION_THRED_POOL_HPP
#define ENGINE_COMMON_UTILITY_CONCURRENTION_THRED_POOL_HPP

#include <queue>
#include <mutex>
#include <memory>
#include <array>
#include <thread>
#include "utility/pattern/flags.hpp"
#include "utility/container/concurrent_queue.hpp"

#pragma once

namespace engine
{
	class thread_pool_job_t;
	class thread_factory_t;
	class thread_t;

	class thread_pool_t;

	class thread_pool_token_t
	{

	public:

		bool is_shutdown_requested();

		friend class thread_pool_t;

	private:

		thread_pool_token_t(thread_pool_t * owner) : owner(owner)
		{

		}

		thread_pool_t * owner;
		
	};

	class thread_pool_job_t
	{

	public:

		virtual ~thread_pool_job_t()
		{

		}

		friend class thread_pool_t;

	private:

		virtual void run(thread_pool_token_t * token) = 0;

		virtual ustring_t get_name() const
		{
			return "Generic Job"_u;
		}
	};

	class thread_pool_t
	{

	public:

		thread_pool_t(std::size_t count, const ustring_t & name, std::shared_ptr<thread_factory_t> thread_factory);
		thread_pool_t(std::size_t count, app_t::kind_t app, app_t::instance_id_t app_instance_id, const ustring_t & name, std::shared_ptr<thread_factory_t> thread_factory);
		~thread_pool_t();

		uint32_t get_count() const
		{
			return count;
		}

		void enqueue_job(std::unique_ptr<thread_pool_job_t> job)
		{
			if (job)
				jobs.push(std::move(job));
		}

		bool is_shutdown_requested()
		{
			return flags.is_flag(flag_t::request_shutdown);
		}

	private:

		std::size_t count;

		enum class flag_t
		{
			request_shutdown,
			count
		};

		flags_t<flag_t> flags;

		std::vector<std::unique_ptr<thread_t>> worker_threads;

		concurrent_queue_t<std::unique_ptr<thread_pool_job_t>> jobs;

		thread_pool_token_t token;

		void thread_func()
		{
			for (;;)
			{
				std::unique_ptr<thread_pool_job_t> job = std::move(jobs.pop());
				if (!job) break;

				job->run(&token);
			}
		}
	};

}

#endif