#ifndef ENGINE_GLOBAL_CORE_APP_INTERRUPTION_ASK_FOR_CLOSE_HPP
#define ENGINE_GLOBAL_CORE_APP_INTERRUPTION_ASK_FOR_CLOSE_HPP
#pragma once


#include "global/core/app/interruption/ask_for.hpp"

namespace engine
{

    class interruption_ask_for_close_t : public interruption_ask_for_t
    {

        public:

            interruption_ask_for_close_t(app_t::instance_id_t target, callback_t on_allowed, callback_t on_refused = [](){}) : interruption_ask_for_t(type_t::ask_for_close, target, default_t::allowed, std::move(on_allowed), std::move(on_refused))
            {

            }


    };


}

#endif