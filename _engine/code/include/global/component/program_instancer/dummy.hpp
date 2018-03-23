#ifndef ENGINE_COMPONENT_PROGRAM_INSTANCER_DUMMY_HPP
#define ENGINE_COMPONENT_PROGRAM_INSTANCER_DUMMY_HPP
#pragma once

#include "global/component/program_instancer.hpp"

namespace engine
{

	class program_instancer_dummy_t : public program_instancer_t
	{

	public:

		program_instancer_dummy_t()
		{

		}

		bool run_new_program(args_t args) final
		{
			return false;
		}

        void terminate_this_then_run_new_program(args_t args) final
		{

		}

        bool run_new_program(app_t::kind_t app) final
		{
			return false;
		}
		
        void terminate_this_then_run_new_program(app_t::kind_t app)
		{

		}

	private:

	};
}

#endif