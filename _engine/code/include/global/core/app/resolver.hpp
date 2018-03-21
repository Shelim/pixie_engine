#ifndef ENGINE_GLOBAL_APP_RESOLVER_HPP
#define ENGINE_GLOBAL_APP_RESOLVER_HPP
#pragma once

#include "global/core/app.hpp"

namespace engine
{
	namespace client
	{
		app_t::kind_t resolve_app(app_context_t * context);
		args_t startup_args_for_app(app_t::kind_t kind);
	}

}

#endif