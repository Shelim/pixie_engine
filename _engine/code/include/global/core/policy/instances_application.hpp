#ifndef ENGINE_GLOBAL_POLICY_INSTANCES_APPLICATION_HPP
#define ENGINE_GLOBAL_POLICY_INSTANCES_APPLICATION_HPP
#pragma once

namespace engine
{

    class instances_application_t
    {

    public:

        virtual ~instances_application_t()
        {

        }

    };
}

#include "global/core/policy/instances_application/multiple.hpp"
#include "global/core/policy/instances_application/single.hpp"

#endif