#ifndef ENGINE_GLOBAL_COMPONENT_PROGRAM_ARGS_HPP
#define ENGINE_GLOBAL_COMPONENT_PROGRAM_ARGS_HPP
#pragma once

#include "global/core/app.hpp"

namespace engine
{

    class program_args_t
    {

    public:

        virtual ~program_args_t()
        {

        }

    };

}

#include "global/component/program_args/dummy.hpp"
#include "global/component/program_args/real.hpp"

#endif