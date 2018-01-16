#ifndef ENGINE_GLOBAL_APPS_RUNNING_HPP
#define ENGINE_GLOBAL_APPS_RUNNING_HPP
#pragma once

#include "global/core/app.hpp"

namespace engine
{

    namespace global
    {

        class apps_running_t
        {

        public:

            virtual ~apps_running_t()
            {

            }

        };

    }

}

#include "global/component/apps_running/dummy.hpp"
#include "global/component/apps_running/real.hpp"

#endif