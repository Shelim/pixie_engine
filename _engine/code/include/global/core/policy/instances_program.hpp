#ifndef ENGINE_GLOBAL_POLICY_INSTANCES_PROGRAM_HPP
#define ENGINE_GLOBAL_POLICY_INSTANCES_PROGRAM_HPP
#pragma once

namespace engine
{

    class instances_program_t
    {

    public:

        virtual ~instances_program_t()
        {

        }

        virtual void on_program_instance_start() = 0;

    };

}

#include "global/core/policy/instances_program/multiple.hpp"
#include "global/core/policy/instances_program/single.hpp"

#endif