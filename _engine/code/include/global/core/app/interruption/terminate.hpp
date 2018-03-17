#ifndef ENGINE_GLOBAL_CORE_APP_INTERRUPTION_TERMINATE_HPP
#define ENGINE_GLOBAL_CORE_APP_INTERRUPTION_TERMINATE_HPP
#pragma once


#include "global/core/app/interruption.hpp"

namespace engine
{

    class interruption_terminate_t : public interruption_t
    {

        public:
            interruption_terminate_t(app_t::instance_id_t target) : interruption_t(type_t::terminate, target)
            {

            }

    };


}

#endif