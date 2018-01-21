#if PIXIE_WINDOWS

#include <exception>
#include <SDL.h>
#include <cstdio>
#include <cstdlib>
#include <thread>
#include <chrono>
#include "global/igniter.hpp"
#include "global/core/program.hpp"

BEGIN_PLATFORM_POLICY_CONFIGURATION()
PLATFORM_ALLOWS_POLICIES(instances_application, multiple, single)
PLATFORM_ALLOWS_POLICIES(instances_program, single)
PLATFORM_ALLOWS_POLICIES(renderer_thread, detached)
END_PLATFORM_POLICY_CONFIGURATION()

std::unique_ptr<engine::global::program_t> program;

int main(int argc, char * argv[])
{
    engine::global::igniter_t igniter;
    program = igniter.ignite_from_main(argc, argv);

    program->wait_for_completion();
    return program->get_return_code();
}

#endif