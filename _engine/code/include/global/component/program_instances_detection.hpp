#ifndef ENGINE_GLOBAL_COMPONENT_PROGRAM_INSTANCES_DETECTION_HPP
#define ENGINE_GLOBAL_COMPONENT_PROGRAM_INSTANCES_DETECTION_HPP
#pragma once

#include "global/core/app.hpp"

namespace engine
{

    namespace global
    {

        class program_instances_detection_t
        {

        public:

            virtual ~program_instances_detection_t()
            {

            }

        };

    }

}

#include "global/component/program_instances_detection/dummy.hpp"
#include "global/component/program_instances_detection/real.hpp"

#endif