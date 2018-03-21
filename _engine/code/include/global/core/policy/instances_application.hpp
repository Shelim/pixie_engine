#ifndef ENGINE_GLOBAL_POLICY_INSTANCES_APPLICATION_HPP
#define ENGINE_GLOBAL_POLICY_INSTANCES_APPLICATION_HPP
#pragma once

#include <memory>

namespace engine
{

    class instances_application_t
    {

    public:

        virtual ~instances_application_t()
        {
            
        }

        virtual bool allow_application_instance_start(app_t::kind_t kind) = 0;

    };
}

#include "global/core/policy/instances_application/multiple.hpp"
#include "global/core/policy/instances_application/single.hpp"

#endif