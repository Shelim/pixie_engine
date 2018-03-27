#ifndef ENGINE_COMPONENT_PROGRAM_ARGS_DUMMY_HPP
#define ENGINE_COMPONENT_PROGRAM_ARGS_DUMMY_HPP
#pragma once

#include "global/component/program_args.hpp"

namespace engine
{

	class program_args_dummy_t : public program_args_t
	{

	public:

		program_args_dummy_t()
		{

		}

		args_t get_program_args() final
		{
			return args_t("");
		}

	private:

	};
}

#endif