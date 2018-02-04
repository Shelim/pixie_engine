#ifndef ENGINE_GLOBAL_COMPONENT_PROGRAM_INSTANCES_COMMUNICATOR_CLIENT_HPP
#define ENGINE_GLOBAL_COMPONENT_PROGRAM_INSTANCES_COMMUNICATOR_CLIENT_HPP
#pragma once

#include "global/core/app.hpp"

namespace engine
{

    class program_instances_communicator_client_t
    {

    public:

        virtual ~program_instances_communicator_client_t()
        {

        }

    };

}

#include "global/component/program_instances_communicator_client/dummy.hpp"
#include "global/component/program_instances_communicator_client/real.hpp"

#endif