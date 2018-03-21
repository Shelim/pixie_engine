#ifndef ENGINE_GLOBAL_CORE_IGNITER_STANDARD_HPP
#define ENGINE_GLOBAL_CORE_IGNITER_STANDARD_HPP
#pragma once

#include "global/core/program/igniter.hpp"

#if PIXIE_WINDOWS

#define USE_STANDARD_POLICIES() \
    PLATFORM_ALLOWS_POLICIES(instances_application, multiple, single) \
    PLATFORM_ALLOWS_POLICIES(instances_program, single) \
    PLATFORM_ALLOWS_POLICIES(renderer_thread, detached) 

#define USE_STANDARD_GLOBAL_PROVIDERS() \
    USE_PROVIDERS_FOR(accountable_app, messenger) \
    USE_PROVIDERS_FOR(app_accounter, console) \
    USE_PROVIDER_FOR(config, config_storage) \
    USE_PROVIDERS_FOR(config_changed, messenger, logger) \
    USE_PROVIDER_FOR(config_storage, windows_registry) \
    USE_PROVIDER_FOR(filesystem, windows) \
    USE_PROVIDERS_FOR(logger, console, temp_file) \
    USE_PROVIDERS_FOR(accountable_thread, messenger) \
    USE_PROVIDER_FOR(thread_stats, windows) \
    USE_PROVIDERS_FOR(thread_accounter, console) \
    USE_PROVIDER_FOR(profiler, remotery)

#if PIXIE_IS_DEBUG_BUILD

#define USE_STANDARD_GLOBAL_SETTINGS() \
    USE_SETTINGS(config_t, debug) \
    USE_SETTINGS(config_storage_t, normal)

#else

#define USE_STANDARD_GLOBAL_SETTINGS() \
    USE_SETTINGS(config_t, release) \
    USE_SETTINGS(config_storage_t, normal)

#endif

#else
#error This platform has no standard settings
#endif

#endif