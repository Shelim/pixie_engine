#ifndef ENGINE_COMMON_UTILITY_PROCESS_SERVICE_HPP
#define ENGINE_COMMON_UTILITY_PROCESS_SERVICE_HPP

#pragma once

#include "global/core/process/process.hpp"
#include "global/core/process/runner.hpp"

namespace engine
{
	class service_base_t
	{

	public:

		virtual ~service_base_t()
		{

		}

		virtual ustring_t get_name() const
		{
			return "Generic"_u;
		}

		virtual std::chrono::duration<double> get_sleep_after() const
		{
			return std::chrono::milliseconds(0);
		}

		virtual task_base_t::result_t run() = 0;

		virtual void on_start()
		{

		}
		virtual void on_end_requested()
		{

		}

	};

	template<class service_object_t> class service_t : public process_base_t
	{ 

	public:

		service_t(std::shared_ptr<process::runner_spawn_factory_t> runner_spawn, std::unique_ptr<service_object_t> object) : object(std::move(object))
		{
			ustring_t name = format_string("Service '#1#'"_u, this->object->get_name());
			runner = runner_spawn->create(name);
		}

		void start()
		{
			std::lock_guard<std::recursive_mutex> guard(service_running_mutex);
			if (!flags.is_flag(flag_t::is_running))
			{
				flags.set_flag(flag_t::is_running, true);
				get_object()->on_start();
				if (!flags.is_flag(flag_t::requested_shutdown_not_yet_handled))
					runner->add_looped_task(std::make_unique<task_t>(this), {}, 0, get_object()->get_sleep_after());
			}
		}

		void end()
		{
			std::lock_guard<std::recursive_mutex> guard(service_running_mutex);
			if (flags.is_flag(flag_t::is_running))
			{
				get_object()->on_end_requested();
				flags.set_flag(flag_t::requested_shutdown_not_yet_handled, true);
				flags.set_flag(flag_t::is_running, false);
			}
		}

		bool is_running()
		{
			std::lock_guard<std::recursive_mutex> guard(service_running_mutex);
			return flags.is_flag(flag_t::is_running);
		}

		service_object_t * get_object()
		{
			return object.get();
		}

	    ~service_t()
		{
			 end();
		}

		virtual ustring_t get_name() const
		{
			return format_string("#1# service"_u, object->get_name());
		}

	private:

		bool is_shutdown()
		{
			std::lock_guard<std::recursive_mutex> guard(service_running_mutex);
			bool ret = false;
			if (!flags.is_flag(flag_t::is_running) && flags.is_flag(flag_t::requested_shutdown_not_yet_handled))
			{
				ret = true;
				flags.set_flag(flag_t::requested_shutdown_not_yet_handled, false);
			}
			return ret;
		}

		friend class task_t;

		typedef service_t<service_object_t> service_real_t;

		class task_t : public task_base_t
		{

		public:

			task_t(service_real_t * owner) : owner(owner)
			{

			}

			result_t run(process::token_t * token) final
			{
				if (owner->get_object()->run() != result_t::running)
				{
					owner->end();
					return result_t::completed;
				}

				if (owner->is_shutdown())
				{
					return result_t::completed;
				}
				return result_t::running;
			}

			ustring_t get_name() const final
			{
				return owner->get_object()->get_name();
			}

		private:

			service_real_t* owner;
		};

		enum class flag_t
		{
			is_running,
			requested_shutdown_not_yet_handled,
			count
		};

		flags_t<flag_t> flags;
		std::recursive_mutex service_running_mutex;
		std::unique_ptr<process::runner_spawn_t> runner;
		std::unique_ptr<service_object_t> object;

	};

}

#endif