#ifndef ENGINE_GLOBAL_CORE_BOOTSTRAPPER_STANDARD_HPP
#define ENGINE_GLOBAL_CORE_BOOTSTRAPPER_STANDARD_HPP
#pragma once

#include "global/core/app/bootstrapper.hpp"

#if PIXIE_WINDOWS

#define USE_STANDARD_LOCAL_PROVIDERS() \
			USE_PROVIDER_FOR(terminal, windows)

#if PIXIE_IS_DEBUG_BUILD

#define USE_STANDARD_LOCAL_SETTINGS() \

#else

#define USE_STANDARD_LOCAL_SETTINGS() \

#endif

#define BOOTSTRAP_APP(app_id, context) \
			BEGIN_BOOTSTRAPPER(app_id, context) \
			ALL_COMPONENTS_BY_DEFAULT_ARE(enabled) \
            USE_STANDARD_LOCAL_PROVIDERS() \
            USE_STANDARD_LOCAL_SETTINGS() \
			APP(app_id) \
			END_BOOTSTRAPPER()
#else
#error This platform has no standard settings
#endif

#endif