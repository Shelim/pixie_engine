#ifndef ENGINE_GLOBAL_COMPONENT_EXTINGUISHER_HPP
#define ENGINE_GLOBAL_COMPONENT_EXTINGUISHER_HPP
#pragma once

#include "global/core/app.hpp"

namespace engine
{

    namespace global
    {

        class extinguisher_t
        {

        public:

            virtual ~extinguisher_t()
            {

            }

            virtual int32_t get_return_code() {return 0;}

        };

    }

}

#include "global/component/extinguisher/dummy.hpp"
#include "global/component/extinguisher/real.hpp"

#endif