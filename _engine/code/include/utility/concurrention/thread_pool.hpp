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

	protected:

		thread_pool_base_t()
		{

		}

		enum class flag_t
		{
			request_shutdown,
			count
		};

		flags_t<flag_t> flags;

	private:

	};

	class thread_pool_token_t
	{

	public:

		bool is_shutdown_requested()
		{
			return owner->is_shutdown_requested();
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

		thread_pool_t() : token(this)
		{
			for (uint32_t i = 0; i < count; i++)
			{
				worker_threads[i] = std::thread([this]() { thread_func(); });
			}
		}
		~thread_pool_t()
		{
			flags.set_flag(flag_t::request_shutdown, true);
			for (uint32_t i = 0; i < count; i++)
			{
				jobs.push(nullptr);
			}
			for (uint32_t i = 0; i < count; i++)
			{
				try
				{
					worker_threads[i].join();
				}
				catch (const std::system_error& e) {}
			}
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