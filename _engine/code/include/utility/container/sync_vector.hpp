#ifndef ENGINE_COMMON_UTILITY_CONTAINER_SYNC_VECTOR_HPP
#define ENGINE_COMMON_UTILITY_CONTAINER_SYNC_VECTOR_HPP

#include <mutex>
#include <vector>

#pragma once

namespace engine
{
	template<class T> class sync_vector_t final
	{

	public:

		typedef typename std::vector<T>::value_type value_type;
		typedef typename std::vector<T>::allocator_type allocator_type;
		typedef typename std::vector<T>::size_type size_type;
		typedef typename std::vector<T>::difference_type difference_type;
		typedef typename std::vector<T>::reference reference;
		typedef typename std::vector<T>::const_reference const_reference;
		typedef typename std::vector<T>::pointer pointer;
		typedef typename std::vector<T>::const_pointer const_pointer;
		typedef typename std::vector<T>::iterator iterator;
		typedef typename std::vector<T>::const_iterator const_iterator;
		typedef typename std::vector<T>::reverse_iterator reverse_iterator;
		typedef typename std::vector<T>::const_reverse_iterator const_reverse_iterator;

		inline reference at(size_type pos)
		{
			std::lock_guard<std::recursive_mutex> guard(mutex);

			return vector.at(pos);
		}

		inline const_reference at(size_type pos) const
		{
			std::lock_guard<std::recursive_mutex> guard(mutex);

			return vector.at(pos);
		}

		inline reference operator[](size_type pos)
		{
			std::lock_guard<std::recursive_mutex> guard(mutex);

			return vector[pos];
		}

		inline const_reference operator[](size_type pos) const
		{
			std::lock_guard<std::recursive_mutex> guard(mutex);

			return vector[pos];
		}

		inline reference front()
		{
			std::lock_guard<std::recursive_mutex> guard(mutex);

			return vector.front();
		}

		inline const_reference front() const
		{
			std::lock_guard<std::recursive_mutex> guard(mutex);

			return vector.front();
		}

		inline reference back()
		{
			std::lock_guard<std::recursive_mutex> guard(mutex);

			return vector.back();
		}

		inline const_reference back() const
		{
			std::lock_guard<std::recursive_mutex> guard(mutex);

			return vector.back();
		}

		inline bool empty() const noexcept
		{
			std::lock_guard<std::recursive_mutex> guard(mutex);

			return vector.empty();
		}

		inline size_type size() const noexcept
		{
			std::lock_guard<std::recursive_mutex> guard(mutex);

			return vector.size();
		}

		inline size_type max_size() const noexcept
		{
			std::lock_guard<std::recursive_mutex> guard(mutex);

			return vector.max_size();
		}

		inline void reserve(size_type new_cap)
		{
			std::lock_guard<std::recursive_mutex> guard(mutex);

			vector.reserve(new_cap);
		}

		inline size_type capacity() const noexcept
		{
			std::lock_guard<std::recursive_mutex> guard(mutex);

			return vector.capacity();
		}

		inline void shrink_to_fit()
		{
			std::lock_guard<std::recursive_mutex> guard(mutex);

			vector.shrink_to_fit();
		}

		inline void clear() noexcept
		{
			std::lock_guard<std::recursive_mutex> guard(mutex);

			vector.clear();
		}

		inline iterator insert(const_iterator pos, const T & value)
		{
			std::lock_guard<std::recursive_mutex> guard(mutex);

			return vector.insert(pos, value);
		}

		inline iterator insert(const_iterator pos, T && value)
		{
			std::lock_guard<std::recursive_mutex> guard(mutex);

			return vector.insert(pos, value);
		}

		inline iterator insert(const_iterator pos, size_type count, const T& value)
		{
			std::lock_guard<std::recursive_mutex> guard(mutex);

			return vector.insert(pos, count, value);
		}

		template<class input_iter> inline iterator insert(const_iterator pos, input_iter first, input_iter last)
		{
			std::lock_guard<std::recursive_mutex> guard(mutex);

			return vector.insert(pos, first, last);
		}

		inline iterator insert(const_iterator pos, std::initializer_list<T> ilist)
		{
			std::lock_guard<std::recursive_mutex> guard(mutex);

			return vector.insert(pos, ilist);
		}

		template<class... args_t> inline iterator emplace(const_iterator pos, args_t&&... args)
		{
			std::lock_guard<std::recursive_mutex> guard(mutex);

			return vector.emplace(pos, std::foward(args...));
		}

		inline iterator erase(const_iterator pos)
		{
			std::lock_guard<std::recursive_mutex> guard(mutex);

			return vector.erase(pos);
		}

		inline iterator erase(const_iterator first, const_iterator last)
		{
			std::lock_guard<std::recursive_mutex> guard(mutex);

			return vector.erase(first, last);
		}

		inline void push_back(T&& value)
		{
			std::lock_guard<std::recursive_mutex> guard(mutex);

			vector.push_back(std::move(value));
		}

		template<class... args_t> inline void emplace_back(args_t&&... args)
		{
			std::lock_guard<std::recursive_mutex> guard(mutex);

			vector.emplace_back(std::forward(args...));
		}

		inline void pop_back()
		{
			std::lock_guard<std::recursive_mutex> guard(mutex);

			vector.pop_back();
		}

		inline void resize(size_type count)
		{
			std::lock_guard<std::recursive_mutex> guard(mutex);

			vector.resize(count);
		}

		inline void resize(size_type count, const value_type& value)
		{
			std::lock_guard<std::recursive_mutex> guard(mutex);

			vector.resize(count, value);
		}

	private:

		mutable std::recursive_mutex mutex;
		std::vector<T> vector;


	};
}

#endif