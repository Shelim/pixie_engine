#ifndef ENGINE_COMPONENT_APP_RESOLVER_DUMMY_HPP
#define ENGINE_COMPONENT_APP_RESOLVER_DUMMY_HPP
#pragma once

#include "global/component/app_resolver.hpp"

namespace engine
{

	class app_resolver_dummy_t : public app_resolver_t
	{

	public:

		app_resolver_dummy_t()
		{

		}

		app_t::kind_t get_app_kind_for_given_context(app_context_t * context) final
		{
			return app_t::kind_t::_engine;
		}

		args_t get_args_for_app_kind(app_t::kind_t kind) final
		{
			return args_t("");
		}

	private:

	};
}

#endif