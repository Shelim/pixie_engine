#ifndef ENGINE_GLOBAL_COMPONENT_APP_ACCOUNTER_HPP
#define ENGINE_GLOBAL_COMPONENT_APP_ACCOUNTER_HPP
#pragma once

#include "global/core/app.hpp"

namespace engine
{

    class app_accounter_t
    {

    public:

        virtual ~app_accounter_t()
        {

        }

        virtual void wait_for_completion() {}

    };

}

#include "global/component/app_accounter/dummy.hpp"
#include "global/component/app_accounter/real.hpp"

#endif