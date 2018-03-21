#ifndef ENGINE_IGNITER_INSTANCES_APPLICATION_SINGLE_HPP
#define ENGINE_IGNITER_INSTANCES_APPLICATION_SINGLE_HPP
#pragma once

#include "global/core/policy/instances_application.hpp"

namespace engine
{

    class instances_application_single_t : public instances_application_t
    {

    public:

        class policy_implementation_single_t : public policy_implementation_t
        {

        public:

            bool allow_application_instance_start(app_t::kind_t kind) final
            {
                return true;
            }

        };

    };

}

#endif