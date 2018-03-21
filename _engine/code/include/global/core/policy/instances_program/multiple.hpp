#ifndef ENGINE_IGNITER_INSTANCES_PROGRAM_MULTIPLE_HPP
#define ENGINE_IGNITER_INSTANCES_PROGRAM_MULTIPLE_HPP
#pragma once

#include "global/core/policy/instances_program.hpp"

namespace engine
{
    class instances_program_multiple_t : public instances_program_t
    {

    public:

        class policy_implementation_multiple_t : public policy_implementation_t
        {

        public:

            void on_program_instance_start() final
            {
                
            }

        };

    };

}

#endif