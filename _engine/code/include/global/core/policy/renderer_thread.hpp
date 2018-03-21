#ifndef ENGINE_GLOBAL_POLICY_RENDERER_THREAD_HPP
#define ENGINE_GLOBAL_POLICY_RENDERER_THREAD_HPP
#pragma once

namespace engine
{

    class renderer_thread_t
    {

    public:

        class policy_implementation_t
        {

        public:

            virtual ~policy_implementation_t()
            {
                
            }

        };

        virtual ~renderer_thread_t()
        {

        }

    };
}

#include "global/core/policy/renderer_thread/callable_by_platform.hpp"
#include "global/core/policy/renderer_thread/detached.hpp"

#endif