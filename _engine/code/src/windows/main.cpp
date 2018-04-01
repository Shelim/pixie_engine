#if PIXIE_WINDOWS

#include <exception>
#include <SDL.h>
#include <cstdio>
#include <cstdlib>
#include <thread>
#include <chrono>
#include "global/core/program/igniter.hpp"
#include "global/core/program.hpp"
#include "global/core/program/test_runner.hpp"

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
    USE_STANDARD_POLICIES()
    ALL_GLOBAL_COMPONENTS_BY_DEFAULT_ARE(enabled)
    USE_STANDARD_GLOBAL_PROVIDERS()
    USE_STANDARD_GLOBAL_SETTINGS()
#if !PIXIE_IS_DEBUG_BUILD
    DISABLE_GLOBAL_COMPONENT(profiler)
#endif
    END_PLATFORM_CONFIGURATION()

    std::shared_ptr<engine::program_t> program = windows.ignite_from_main(argc, const_cast<const char**>(argv));
    if(!program) return EXIT_FAILURE; // Failed to ignite (for example different instance is running)

    program->get_app_overseer()->wait_for_completion();
    return program->get_extinguisher()->get_return_code();
}

#endif
#endif