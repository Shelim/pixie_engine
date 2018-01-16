#ifndef ENGINE_GLOBAL_APP_RUNNER_HPP
#define ENGINE_GLOBAL_APP_RUNNER_HPP
#pragma once

#include "global/core/app.hpp"

namespace engine
{

    namespace global
    {

        class app_runner_t
        {

        public:

            virtual ~app_runner_t()
            {

            }

        };

    }

}

#include "global/component/app_runner/dummy.hpp"
#include "global/component/app_runner/real.hpp"

#endif