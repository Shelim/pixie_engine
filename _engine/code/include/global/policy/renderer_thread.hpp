#ifndef ENGINE_GLOBAL_POLICY_RENDERER_THREAD_HPP
#define ENGINE_GLOBAL_POLICY_RENDERER_THREAD_HPP
#pragma once

namespace engine
{

    namespace global
    {

        class renderer_thread_t
        {

        public:

            virtual ~renderer_thread_t()
            {

            }

        };

    }
}

#include "global/policy/renderer_thread/callable_by_platform.hpp"
#include "global/policy/renderer_thread/detached.hpp"

#endif