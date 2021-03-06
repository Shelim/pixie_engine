#ifndef ENGINE_COMMON_UTILITY_CONTAINER_SYNC_QUEUE_HPP
#define ENGINE_COMMON_UTILITY_CONTAINER_SYNC_QUEUE_HPP

#include <queue>
#include <mutex>

#pragma once

namespace engine
{
	template<class T> class sync_queue_t final
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

			T front = std::move(queue.front());
			queue.pop();

			return std::move(front);
		}

		T pop_check(T if_empty)
		{
			std::lock_guard<std::recursive_mutex> guard(mutex);

			if (queue.empty()) return std::move(if_empty);

			T front = std::move(queue.front());
			queue.pop();

			return std::move(front);
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