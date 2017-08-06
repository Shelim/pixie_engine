#ifndef ENGINE_COMMON_UTILITY_MESSENGER_MESSENGER_HPP
#define ENGINE_COMMON_UTILITY_MESSENGER_MESSENGER_HPP
#pragma once

#include <cstdint>
#include <memory>
#include <map>
#include <functional>
#include "utility/messenger/msg_base.hpp"
#include "utility/messenger/msg_kill_messanger.hpp"
#include "utility/container/concurrent_queue.hpp"

namespace engine
{

	class messenger_t
	{

	public:

		messenger_t();
		~messenger_t();

		void post_message(std::unique_ptr<msg_base_t> message)
		{
			messages.push(std::move(message));
		}

		void post_message_sync(std::unique_ptr<msg_base_t> message)
		{
			dispatch_message(message.get());
		}

		typedef std::function<void(msg_base_t*)> callback_func_t;

		class callback_t
		{


		public:

			callback_t(callback_func_t func, void * owning_ptr = nullptr) : callback_function(func), owner(reinterpret_cast<intptr_t>(owning_ptr))
			{

			}

			void call(msg_base_t * msg)
			{
				callback_function(msg);
			}

			bool is_owned_by(void * owning_ptr)
			{
				return (owner == reinterpret_cast<intptr_t>(owning_ptr));
			}

		private:

			callback_func_t callback_function;
			intptr_t owner;

		};

		typedef std::map<uint32_t, callback_t> callbacks_t;

		class callbacks_by_msg_type_t
		{

		public:

			callbacks_by_msg_type_t() : next_key(0)
			{

			}

			uint32_t attach(callback_func_t callback, void * owning_ptr = nullptr)
			{
				std::lock_guard<std::recursive_mutex> guard(callback_manipulation_mutex);

				callbacks.insert(std::make_pair(next_key, callback_t(callback, owning_ptr)));
				return (next_key++);
			}

			void deatach(uint32_t key)
			{
				std::lock_guard<std::recursive_mutex> guard(callback_manipulation_mutex);

				auto iter = callbacks.find(key);
				if (iter != callbacks.end())
					callbacks.erase(iter);
			}

			void deatach_all(void * owning_ptr)
			{
				std::lock_guard<std::recursive_mutex> guard(callback_manipulation_mutex);

				for (auto iter = callbacks.begin(); iter != callbacks.end();)
				{
					if (iter->second.is_owned_by(owning_ptr))
						iter = callbacks.erase(iter);
					else
						++iter;
				}
			}

			void deatach_all()
			{
				std::lock_guard<std::recursive_mutex> guard(callback_manipulation_mutex);

				callbacks.clear();
			}

			void call(msg_base_t* msg)
			{
				std::lock_guard<std::recursive_mutex> guard(callback_manipulation_mutex);

				for (auto & iter : callbacks)
					iter.second.call(msg);
			}

		private:

			callbacks_t callbacks;
			uint32_t next_key;
			mutable std::recursive_mutex callback_manipulation_mutex;
		};

		typedef std::map<uint32_t, callbacks_by_msg_type_t> callbacks_by_msg_types_t;

		uint32_t attach(uint32_t msg_type, callback_func_t callback)
		{
			return callbacks[msg_type].attach(callback);
		}

		void deatach(uint32_t msg_type, uint32_t key)
		{
			callbacks[msg_type].deatach(key);
		}

		void deatach_all(uint32_t msg_type)
		{
			callbacks[msg_type].deatach_all();
		}

		void deatach_all()
		{
			callbacks.clear();
		}

	private:

		uint32_t attach(uint32_t msg_type, callback_func_t callback, void * owning_ptr)
		{
			return callbacks[msg_type].attach(callback, owning_ptr);
		}

		void deatach_all(void * owning_ptr)
		{
			for (auto & callback : callbacks)
				callback.second.deatach_all(owning_ptr);
		}

		friend class callback_container_t;

		concurrent_queue_t<std::unique_ptr<msg_base_t> > messages;
		std::thread messages_thread;

		void messages_thread_func();

		void dispatch_message(msg_base_t * msg);

		callbacks_by_msg_types_t callbacks;
	};


	class callback_container_t
	{

	public:

		callback_container_t(std::shared_ptr<messenger_t> messager, void * this_ptr) : messager(messager), owning_ptr(this_ptr)
		{

		}

		~callback_container_t()
		{
			deatach_all();
		}

		uint32_t attach(uint32_t msg_type, messenger_t::callback_func_t callback)
		{
			return messager->attach(msg_type, callback, owning_ptr);
		}

		void deatach(uint32_t msg_type, uint32_t key)
		{
			messager->deatach(msg_type, key);
		}

		void deatach_all()
		{
			if (owning_ptr) messager->deatach_all(owning_ptr);
		}

	private:

		void *owning_ptr;
		std::shared_ptr<messenger_t> messager;
	};
}

#endif