#ifndef ENGINE_GLOBAL_POLICY_INSTANCES_PROGRAM_HPP
#define ENGINE_GLOBAL_POLICY_INSTANCES_PROGRAM_HPP
#pragma once

namespace engine
{

    namespace global
    {

        class instances_program_t
        {

        public:

            virtual ~instances_program_t()
            {

            }

        };

    }
}

#include "global/policy/instances_program/multiple.hpp"
#include "global/policy/instances_program/single.hpp"

#endif