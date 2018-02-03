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

            class policy_implementation_t
            {

            public:

                virtual ~policy_implementation_t()
                {
                    
                }

                virtual void on_program_instance_start() = 0;

            };

            virtual ~instances_program_t()
            {

            }

            virtual std::unique_ptr<policy_implementation_t> get_implementation() { }

        };

    }
}

#include "global/policy/instances_program/multiple.hpp"
#include "global/policy/instances_program/single.hpp"

#endif