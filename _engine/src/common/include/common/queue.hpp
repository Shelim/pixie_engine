#ifndef ENGINE_COMMON_QUEUE_HPP
#define ENGINE_COMMON_QUEUE_HPP

#include <queue>
#include <mutex>

#pragma once

namespace engine
{

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

		void push(T & value)
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