#ifndef ENGINE_COMMON_UTILITY_CONTAINER_CONCURRENT_QUEUE_HPP
#define ENGINE_COMMON_UTILITY_CONTAINER_CONCURRENT_QUEUE_HPP

#include <queue>
#include <mutex>
#include <condition_variable>

#pragma once

namespace engine
{

	template <class T> class concurrent_queue_t
	{

	public:

		concurrent_queue_t()
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

}

#endif