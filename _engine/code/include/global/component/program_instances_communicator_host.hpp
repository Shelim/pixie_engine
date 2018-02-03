#ifndef ENGINE_GLOBAL_COMPONENT_PROGRAM_INSTANCES_COMMUNICATOR_HOST_HPP
#define ENGINE_GLOBAL_COMPONENT_PROGRAM_INSTANCES_COMMUNICATOR_HOST_HPP
#pragma once

#include "global/core/app.hpp"

namespace engine
{

    namespace global
    {

        class program_instances_communicator_host_t
        {

        public:

            virtual ~program_instances_communicator_host_t()
            {

            }

        };

    }

}

#include "global/component/program_instances_communicator_host/dummy.hpp"
#include "global/component/program_instances_communicator_host/real.hpp"

#endif