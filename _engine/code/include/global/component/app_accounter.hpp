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

        typedef std::vector<std::shared_ptr<app_meta_t> > apps_meta_t;

        virtual apps_meta_t get_running_apps() = 0;
    };

}

#include "global/component/app_accounter/dummy.hpp"
#include "global/component/app_accounter/real.hpp"

#endif