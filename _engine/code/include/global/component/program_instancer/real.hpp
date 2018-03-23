#ifndef ENGINE_COMPONENT_PROGRAM_INSTANCER_REAL_HPP
#define ENGINE_COMPONENT_PROGRAM_INSTANCER_REAL_HPP
#pragma once

#include "global/component/program_instancer.hpp"
#include "global/component/app_resolver.hpp"
#include "utility/pattern/provider.hpp"
#include "global/core/policy/instances_program.hpp"
#include <memory>

namespace engine
{
	class program_instancer_provider_base_t
	{

	public:

		virtual ~program_instancer_provider_base_t()
		{

		}

		virtual bool run_new_program(args_t args) = 0;

	};

	REGISTER_PROVIDER_BASE_TYPE(program_instancer_t, program_instancer_provider_base_t)

	class program_instancer_real_t : public program_instancer_t
	{

	public:

		program_instancer_real_t(std::shared_ptr<app_resolver_t> app_resolver, std::unique_ptr<holder_t<program_instancer_t> > program_instancer_provider, std::shared_ptr<instances_program_t> policy_instances_program) : app_resolver(app_resolver), program_instancer_provider(std::move(program_instancer_provider)), policy_instances_program(policy_instances_program)
		{

		}

		bool run_new_program(args_t args) final
		{
			if(policy_instances_program->get_type() == engine::instances_program_t::type_t::allow_multiple)
				return program_instancer_provider->get_provider()->run_new_program(args);
			else
				return false;
		}

        bool run_new_program(app_t::kind_t app) final
		{
			return run_new_program(app_resolver->get_startup_args_to_run_new_instance_of(app));
		}

	private:

		std::shared_ptr<app_resolver_t> app_resolver;
		std::unique_ptr<holder_t<program_instancer_t> > program_instancer_provider;
		std::shared_ptr<instances_program_t> policy_instances_program;
	};
}

#include "global/component/program_instancer/provider/generic.hpp"
#include "global/component/program_instancer/provider/windows.hpp"

#endif