#ifndef ENGINE_GLOBAL_COMPONENT_THREAD_OVERSEER_HPP
#define ENGINE_GLOBAL_COMPONENT_THREAD_OVERSEER_HPP
#pragma once

#include "global/core/app.hpp"

namespace engine
{

    class thread_overseer_t
    {

    public:

        virtual ~thread_overseer_t()
        {

        }

    };

}

#include "global/component/thread_overseer/dummy.hpp"
#include "global/component/thread_overseer/real.hpp"

#endif