#ifndef ENGINE_GLOBAL_COMPONENT_APP_INSTANCER_HPP
#define ENGINE_GLOBAL_COMPONENT_APP_INSTANCER_HPP
#pragma once

#include "global/core/app.hpp"

namespace engine
{

    namespace global
    {

        class app_instancer_t
        {

        public:

            virtual ~app_instancer_t()
            {

            }

        };

    }

}

#include "global/component/app_instancer/dummy.hpp"
#include "global/component/app_instancer/real.hpp"

#endif