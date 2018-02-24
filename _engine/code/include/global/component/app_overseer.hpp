#ifndef ENGINE_GLOBAL_COMPONENT_APP_OVERSEER_HPP
#define ENGINE_GLOBAL_COMPONENT_APP_OVERSEER_HPP
#pragma once

#include "global/core/app.hpp"

namespace engine
{

    class app_overseer_t
    {

    public:

        virtual ~app_overseer_t()
        {

        }

        virtual void wait_for_completion() {}

    };

}

#include "global/component/app_overseer/dummy.hpp"
#include "global/component/app_overseer/real.hpp"

#endif