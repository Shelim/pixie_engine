#ifndef ENGINE_COMPONENT_APP_RESOLVER_REAL_HPP
#define ENGINE_COMPONENT_APP_RESOLVER_REAL_HPP
#pragma once

#include "global/component/app_resolver.hpp"
#include "global/core/app/resolver.hpp"
#include "global/component/program_args.hpp"
#include "client/resolve_app.hpp"

namespace engine
{

	class app_resolver_real_t : public app_resolver_t
	{

	public:

		app_resolver_real_t(std::shared_ptr<client::resolve_app_t> client_resolve_app) : client_resolve_app(client_resolve_app)
		{

		}

		app_t::kind_t get_app_kind_for_given_context(app_context_t * context) final
		{
			return client_resolve_app->resolve_app(context);
		}

		args_t get_startup_args_to_run_new_instance_of(app_t::kind_t kind) final
		{
			return client_resolve_app->startup_args_to_run_new_instance_of(kind);
		}

	private:

		std::shared_ptr<client::resolve_app_t> client_resolve_app;

	};
}

#endif