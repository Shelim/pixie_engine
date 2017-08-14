#ifndef ENGINE_COMMON_UTILITY_PROCESS_PROCESS_HPP
#define ENGINE_COMMON_UTILITY_PROCESS_PROCESS_HPP

#pragma once

#include "utility/pattern/flags.hpp"
#include "utility/pattern/enum.hpp"
#include "utility/text/ustring.hpp"
#include "utility/process/runner.hpp"

namespace engine
{

	class process_t
	{

	public:

		virtual ~process_t()
		{

		}

		virtual ustring_t get_name() const = 0;

		bool has_requested_shutdown()
		{
			return flags.is_flag(flag_t::requested_shutdown);
		}

		void request_shutdown()
		{
			flags.set_flag(flag_t::requested_shutdown);
		}

		enum class return_t
		{
			completed,
			pending
		};

		process_runner_id_t get_next_runner() const
		{
			return next_runner;
		}

	protected:

		process_t(process_runner_id_t first_runner = process_runner_id_t::force_sync) : next_runner(first_runner)
		{

		}

		void set_next_runner(process_runner_id_t val)
		{
			this->next_runner = val;
		}

	private:

		process_runner_id_t next_runner;

		friend class processes_internal_t;

		virtual return_t execute_local(process_runner_id_t runner) = 0;

		bool execute(process_runner_id_t runner)
		{
			return_t ret = execute_local(runner);
			if (ret == return_t::pending) return false;

			return true;
		}

		enum class flag_t
		{
			requested_shutdown,

			count
		};

		flags_t<flag_t> flags;
	};
}

#endif