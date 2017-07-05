#ifndef ENGINE_MODEL_MANIFEST_APP_HPP
#define ENGINE_MODEL_MANIFEST_APP_HPP
#pragma once

namespace engine
{
	class manifest_app_t;
}
	
#if PIXIE_WINDOWS

#if PIXIE_IS_DEBUG_BUILD
#if PIXIE_IS_PORTABLE_BUILD
#include "_autogen/windows_x86/app/manifest_app_debug_portable.hpp"
#else
#include "_autogen/windows_x86/app/manifest_app_debug.hpp"
#endif
#else
#if PIXIE_IS_PORTABLE_BUILD
#include "_autogen/windows_x86/app/manifest_app_release_portable.hpp"
#else
#include "_autogen/windows_x86/app/manifest_app_release.hpp"
#endif
#endif

#else

#error "Autogen requires known platform - did you forgot to define PIXIE_PLATFORM_FULL?"

#endif

#endif