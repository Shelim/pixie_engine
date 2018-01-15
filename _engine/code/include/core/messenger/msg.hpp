#ifndef ENGINE_COMMON_UTILITY_MESSENGER_MSG_BASE_HPP
#define ENGINE_COMMON_UTILITY_MESSENGER_MSG_BASE_HPP
#pragma once

#include <cstdint>
#include "utility/pattern/fourcc.hpp"
#include "utility/concurrention/signal.hpp"
#include "utility/text/ustring.hpp"

namespace engine
{

	namespace messenger
	{
		template<class msg_actual_t> using callback_t = std::function<void(msg_actual_t*)>;
		
		enum class history_t
		{
			skip,
			dump_if_available
		};

		class msg_base_t
		{

		public:

			virtual ~msg_base_t();

#include "def/enum/messenger_type.def"

			type_t get_type() const;

		protected:

			msg_base_t(type_t type);

		private:

			type_t type;

		};

		template<class msg_actual_t> class msg_t : public msg_base_t
		{

		public:

			msg_t(std::shared_ptr<msg_actual_t> actual) : msg_base_t(type_t::msg), actual(actual)
			{

			}

			std::shared_ptr<msg_actual_t> get_actual()
			{
				return actual;
			}

		private:

			std::shared_ptr<msg_actual_t> actual;

		};

		template<class msg_actual_t> class queue_t;
		template<class msg_actual_t> class instance_t;

		template<class msg_actual_t> class msg_internal_register_instance : public msg_base_t
		{

		public:

			class info_t
			{
			public:

				std::unique_ptr<instance_t<msg_actual_t> > wait_for_completion()
				{
					signal.wait();
					return std::move(result);
				}

			private:

				template<class msg_actual_t> friend class msg_internal_register_instance;

				std::unique_ptr<instance_t<msg_actual_t>> result;
				signal_t signal;
			};

			msg_internal_register_instance(callback_t<msg_actual_t> callback, history_t history, info_t * info_to_populate) : msg_base_t(type_t::register_instance), callback(callback), history(history), info_to_populate(info_to_populate)
			{

			}

			callback_t<msg_actual_t> get_callback()
			{
				return callback;
			}

			history_t get_history()
			{
				return history;
			}

			void raport_completion(std::unique_ptr<instance_t<msg_actual_t>> instance)
			{
				info_to_populate->result = std::move(instance);
				info_to_populate->signal.signal();
			}

		private:

			callback_t<msg_actual_t> callback;
			history_t history;
			info_t * info_to_populate;

		};

		template<class msg_actual_t> class msg_internal_unregister_instance : public msg_base_t
		{

		public:

			class info_t
			{
			public:

				void wait_for_completion()
				{
					signal.wait();
				}

			private:

				template<class msg_actual_t> friend class msg_internal_unregister_instance;

				signal_t signal;
			};

			msg_internal_unregister_instance(instance_t<msg_actual_t> * instance, info_t * info_to_populate) : msg_base_t(type_t::unregister_instance), instance(instance), info_to_populate(info_to_populate)
			{

			}

			instance_t<msg_actual_t> * get_instance()
			{
				return instance;
			}

			void raport_completion()
			{
				info_to_populate->signal.signal();
			}

		private:

			instance_t<msg_actual_t> * instance;
			info_t * info_to_populate;

		};
	}

}

namespace engine
{

	namespace messenger
	{

		template<class msg_actual_t> inline ustring_t get_msg_type(msg_actual_t * msg = nullptr)
		{
			return "Unknown"_u;
		}

#define ENGINE_MESSENGER_QUEUE_DEF(name) class msg_##name##_t; template<> inline ustring_t get_msg_type(msg_##name##_t * msg) { return #name##_u; };
#include "def/messenger.def"

	}
}

#define ENGINE_ENUM_HEADER_TO_USE "def/enum/messenger_type.def"
#include "core/utility/enum_to_string.hpp"

#endif