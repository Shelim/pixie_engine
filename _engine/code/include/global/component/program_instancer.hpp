#ifndef ENGINE_GLOBAL_COMPONENT_PROGRAM_INSTANCER_HPP
#define ENGINE_GLOBAL_COMPONENT_PROGRAM_INSTANCER_HPP
#pragma once

#include "global/core/app.hpp"

namespace engine
{

    class program_instancer_t
    {

    public:

        virtual ~program_instancer_t()
        {

        }

        virtual bool run_new_program(args_t args) = 0;
        virtual bool run_new_program(app_t::kind_t app) = 0;

    };

}

#include "global/component/program_instancer/dummy.hpp"
#include "global/component/program_instancer/real.hpp"

#endif