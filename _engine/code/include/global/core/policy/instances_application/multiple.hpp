#ifndef ENGINE_IGNITER_INSTANCES_APPLICATION_MULTIPLE_HPP
#define ENGINE_IGNITER_INSTANCES_APPLICATION_MULTIPLE_HPP
#pragma once

#include "global/core/policy/instances_application.hpp"

namespace engine
{

    class instances_application_multiple_t : public instances_application_t
    {

    public:

        instances_application_multiple_t() : instances_application_t(type_t::allow_multiple)
        {
            
        }

        bool allow_application_instance_start(app_t::kind_t kind) final
        {
            return true;
        }

    };

}

#endif