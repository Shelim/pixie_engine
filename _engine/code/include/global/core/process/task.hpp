#ifndef ENGINE_COMMON_UTILITY_PROCESS_TASK_HPP
#define ENGINE_COMMON_UTILITY_PROCESS_TASK_HPP

#pragma once

#include "utility/pattern/flags.hpp"
#include "utility/pattern/enum.hpp"
#include "utility/text/ustring.hpp"
#include "global/core/process/token.hpp"
#include <functional>

namespace engine
{

	class task_base_t
	{

	public:

		virtual ~task_base_t()
		{

		}

		enum class result_t
		{
			running,
			completed
		};

		virtual result_t run(process::token_t * token) = 0;

		virtual ustring_t get_name() const
		{
			return "Generic task"_u;
		}

	protected:


	private:


	};

	class task_func_t : public task_base_t
	{

	public:

		typedef std::function<result_t(process::token_t*)> func_t;

		task_func_t(func_t func, const ustring_t & name) : func(func), name(name)
		{

		}

		result_t run(process::token_t * token) final
		{
			return func(token);
		}

		virtual ustring_t get_name() const override
		{
			return name;
		}

	private:

		func_t func;
		const ustring_t name;
	};

}

#endif