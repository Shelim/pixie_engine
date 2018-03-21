#ifndef ENGINE_COMPONENT_APP_RESOLVER_REAL_HPP
#define ENGINE_COMPONENT_APP_RESOLVER_REAL_HPP
#pragma once

#include "global/component/app_resolver.hpp"
#include "global/core/app/resolver.hpp"

namespace engine
{

	class app_resolver_real_t : public app_resolver_t
	{

	public:

		app_resolver_real_t()
		{

		}

		app_t::kind_t get_app_kind_for_given_context(app_context_t * context) final
		{
			return engine::client::resolve_app(context);
		}

		args_t get_args_for_app_kind(app_t::kind_t kind) final
		{
			return engine::client::startup_args_for_app(kind);
		}

	private:

	};
}

#endif