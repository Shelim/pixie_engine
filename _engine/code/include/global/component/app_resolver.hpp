#ifndef ENGINE_GLOBAL_COMPONENT_APP_RESOLVER_HPP
#define ENGINE_GLOBAL_COMPONENT_APP_RESOLVER_HPP
#pragma once

#include "global/core/app.hpp"

namespace engine
{

    class app_resolver_t
    {

    public:

        virtual ~app_resolver_t()
        {

        }

        virtual app_t::kind_t get_app_kind_for_given_context(app_context_t * context) = 0;
        virtual args_t get_startup_args_to_run_new_instance_of(app_t::kind_t kind) = 0;

    };

}

#include "global/component/app_resolver/dummy.hpp"
#include "global/component/app_resolver/real.hpp"

#endif