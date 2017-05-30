#ifndef ENGINE_COMMON_QUEUE_HPP
#define ENGINE_COMMON_QUEUE_HPP

#include <queue>
#include <mutex>
#include <condition_variable>

#pragma once

namespace engine
{

	template <class T> class queue_async_t
	{

	public:

		queue_async_t()
		{
			
		}

		void push(const T & t)
		{
			std::lock_guard<std::mutex> lock(mutex);
			queue.push(t);
			condition.notify_one();
		}


		T pop(void)
		{
			std::unique_lock<std::mutex> lock(mutex);
			while (queue.empty())
			{
				condition.wait(lock);
			}
			T ret = queue.front();
			queue.pop();
			return ret;
		}

	private:
		std::queue<T> queue;
		mutable std::mutex mutex;
		std::condition_variable condition;
	};

	template<class T> class queue_t final
	{

	public:

		bool is_empty()
		{
			std::lock_guard<std::recursive_mutex> guard(mutex);

			return queue.empty();
		}

		auto size()
		{
			std::lock_guard<std::recursive_mutex> guard(mutex);

			return queue.size();
		}

		T pop()
		{
			std::lock_guard<std::recursive_mutex> guard(mutex);

			T front = queue.front();
			queue.pop();

			return front;
		}

		void push(const T & value)
		{
			std::lock_guard<std::recursive_mutex> guard(mutex);

			queue.push(value);
		}

		void push(T && value)
		{
			std::lock_guard<std::recursive_mutex> guard(mutex);

			queue.push(std::move(value));
		}

		template< class... Args > void emplace(Args&&... args)
		{
			std::lock_guard<std::recursive_mutex> guard(mutex);

			queue.emplace(std::forward<Args>(args)...);
		}

	private:

		std::recursive_mutex mutex;
		std::queue<T> queue;


	};

}

#endif