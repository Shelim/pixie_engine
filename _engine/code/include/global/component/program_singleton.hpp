#ifndef ENGINE_GLOBAL_PROGRAM_SINGLETON_HPP
#define ENGINE_GLOBAL_PROGRAM_SINGLETON_HPP
#pragma once

#include "global/app.hpp"

namespace engine
{

    namespace global
    {

        class program_singleton_t
        {

        public:

            virtual ~program_singleton_t()
            {

            }

        };

    }

}

#include "global/component/program_singleton/dummy.hpp"
#include "global/component/program_singleton/real.hpp"

#endif