#if PIXIE_WINDOWS

#include <exception>
#include <SDL.h>
#include <cstdio>
#include <cstdlib>
#include <thread>
#include <chrono>
#include "global/igniter.hpp"
#include "global/program.hpp"

BEGIN_PLATFORM_POLICY_CONFIGURATION()
PLATFORM_ALLOWS_POLICIES(instances_application, multiple, single)
PLATFORM_ALLOWS_POLICIES(instances_program, single)
PLATFORM_ALLOWS_POLICIES(renderer_thread, detached)
END_PLATFORM_POLICY_CONFIGURATION()

int main(int argc, char * argv[])
{
    engine::global::igniter_t igniter;
    auto program = igniter.ignite_from_main(argc, argv);

    program->wait_for_completion();

    auto extinguisher = program->get_extinguisher();
    extinguisher->extinguish();

    return program->get_return_code();
}

#endif