#ifndef ENGINE_GLOBAL_CORE_APP_INTERRUPTION_ASK_FOR_NEW_INSTANCE_HPP
#define ENGINE_GLOBAL_CORE_APP_INTERRUPTION_ASK_FOR_NEW_INSTANCE_HPP
#pragma once


#include "global/core/app/interruption/ask_for.hpp"

namespace engine
{

    class interruption_ask_for_new_instance_t : public interruption_ask_for_t
    {

        public:

            interruption_ask_for_new_instance_t(app_t::instance_id_t target, callback_t on_allowed) : interruption_ask_for_t(type_t::ask_for_new_instance, target, default_t::refused, std::move(on_allowed))
            {

            }


    };


}

#endif