#ifndef ENGINE_COMPONENT_PROGRAM_ARGS_REAL_HPP
#define ENGINE_COMPONENT_PROGRAM_ARGS_REAL_HPP
#pragma once

#include "global/component/program_args.hpp"

namespace engine
{

	class program_args_real_t : public program_args_t
	{

	public:

		program_args_real_t(args_t args) : args(args)
		{

		}

		args_t get_program_args() final
		{
			return args;
		}

	private:

		args_t args;

	};
}

#endif