#if PIXIE_WINDOWS

#include <exception>
#include <SDL.h>
#include <cstdio>
#include <cstdlib>
#include <thread>
#include <chrono>
#include "global/core/igniter.hpp"
#include "global/core/program.hpp"
#include "global/core/test_runner.hpp"

#if PIXIE_IS_TESTER_BUILD

int main(int argc, char * argv[])
{
    engine::test_runner_t runner;

    runner.run_test(argc, argv);
    runner.wait_till_completed();

    return runner.get_return_code();
}

#else

int main(int argc, char * argv[])
{
    BEGIN_PLATFORM_CONFIGURATION(windows)

    PLATFORM_ALLOWS_POLICIES(instances_application, multiple, single)
    PLATFORM_ALLOWS_POLICIES(instances_program, single)
    PLATFORM_ALLOWS_POLICIES(renderer_thread, detached)

    ALL_GLOBAL_COMPONENTS_BY_DEFAULT_ARE(enabled)

    USE_PROVIDER_FOR(config, config_storage)
    USE_PROVIDERS_FOR(config_changed, messenger, logger)
    USE_PROVIDER_FOR(config_storage, windows_registry)
    USE_PROVIDER_FOR(filesystem, windows)
    USE_PROVIDERS_FOR(logger, console, temp_file)
    USE_PROVIDERS_FOR(accountable_thread, messenger)
    USE_PROVIDER_FOR(thread_stats, windows)
    USE_PROVIDERS_FOR(thread_accounter, console)

#if PIXIE_IS_DEBUG_BUILD
    USE_SETTINGS(config_t, debug)
    USE_PROVIDER_FOR(profiler, remotery)
#else
    USE_SETTINGS(config_t, release)
    DISABLE_GLOBAL_COMPONENT(profiler)
#endif
    USE_SETTINGS(config_storage_t, normal)

    END_PLATFORM_CONFIGURATION()

    std::shared_ptr<engine::program_t> program = windows.ignite_from_main(argc, argv);
    if(!program) return EXIT_FAILURE; // Failed to ignite (for example different instance is running)

    program->get_app_overseer()->wait_for_completion();
    return program->get_extinguisher()->get_return_code();
}

#endif
#endif