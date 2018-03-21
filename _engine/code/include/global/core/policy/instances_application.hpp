#ifndef ENGINE_GLOBAL_POLICY_INSTANCES_APPLICATION_HPP
#define ENGINE_GLOBAL_POLICY_INSTANCES_APPLICATION_HPP
#pragma once

#include <memory>

namespace engine
{

    class instances_application_t
    {

    public:

        class policy_implementation_t
        {

        public:

            virtual ~policy_implementation_t()
            {
                
            }

            virtual bool allow_application_instance_start(app_t::kind_t kind) = 0;

        };

        virtual ~instances_application_t()
        {

        }

        virtual std::unique_ptr<policy_implementation_t> get_implementation();

    };
}

#include "global/core/policy/instances_application/multiple.hpp"
#include "global/core/policy/instances_application/single.hpp"

#endif