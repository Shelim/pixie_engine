#ifndef ENGINE_COMPONENT_PROGRAM_ARGS_REAL_HPP
#define ENGINE_COMPONENT_PROGRAM_ARGS_REAL_HPP
#pragma once

#include "global/component/program_args.hpp"

namespace engine
{

	class program_args_real_t : public program_args_t
	{

	public:

		program_args_real_t()
		{

		}

		args_t get_program_args() final
		{
			// ToDo!
			args_t ret("");
			return ret;
		}

	private:

	};
}

#endif