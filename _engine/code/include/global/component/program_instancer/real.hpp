#ifndef ENGINE_COMPONENT_PROGRAM_INSTANCER_REAL_HPP
#define ENGINE_COMPONENT_PROGRAM_INSTANCER_REAL_HPP
#pragma once

#include "global/component/program_instancer.hpp"
#include "global/component/app_resolver.hpp"
#include <memory>

namespace engine
{

	class program_instancer_real_t : public program_instancer_t
	{

	public:

		program_instancer_real_t(std::shared_ptr<app_resolver_t> app_resolver) : app_resolver(app_resolver)
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
			return run_new_program(app_resolver->get_startup_args_to_run_new_instance_of(app));
		}
		
        void terminate_this_then_run_new_program(app_t::kind_t app)
		{
			terminate_this_then_run_new_program(app_resolver->get_startup_args_to_run_new_instance_of(app));
		}

	private:

		std::shared_ptr<app_resolver_t> app_resolver;

	};
}

#include "global/component/program_instancer/provider/generic.hpp"
#include "global/component/program_instancer/provider/windows.hpp"

#endif