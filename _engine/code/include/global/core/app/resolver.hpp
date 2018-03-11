#ifndef ENGINE_GLOBAL_APP_RESOLVER_HPP
#define ENGINE_GLOBAL_APP_RESOLVER_HPP
#pragma once

#include "global/core/app.hpp"

namespace engine
{
	namespace client
	{
		app_t::kind_t resolve_app(app_context_t * context);
	}

}

#endif