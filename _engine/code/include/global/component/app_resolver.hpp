#ifndef ENGINE_GLOBAL_COMPONENT_APP_RESOLVER_HPP
#define ENGINE_GLOBAL_COMPONENT_APP_RESOLVER_HPP
#pragma once

#include "global/core/app.hpp"

namespace engine
{

    namespace global
    {

        class app_resolver_t
        {

        public:

            virtual ~app_resolver_t()
            {

            }

        };

    }

}

#include "global/component/app_resolver/dummy.hpp"
#include "global/component/app_resolver/real.hpp"

#endif