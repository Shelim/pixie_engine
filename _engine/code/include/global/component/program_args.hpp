#ifndef ENGINE_GLOBAL_COMPONENT_PROGRAM_ARGS_HPP
#define ENGINE_GLOBAL_COMPONENT_PROGRAM_ARGS_HPP
#pragma once

#include "global/core/app.hpp"
#include "utility/platform/args.hpp"

namespace engine
{

    class program_args_t
    {

    public:

        virtual ~program_args_t()
        {

        }

        virtual args_t get_program_args() = 0;

    };

}

#include "global/component/program_args/dummy.hpp"
#include "global/component/program_args/real.hpp"

#endif