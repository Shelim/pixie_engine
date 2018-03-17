#ifndef ENGINE_GLOBAL_CORE_APP_INTERRUPTION_ASK_FOR_HPP
#define ENGINE_GLOBAL_CORE_APP_INTERRUPTION_ASK_FOR_HPP
#pragma once


#include "global/core/app/interruption.hpp"

namespace engine
{

    class interruption_ask_for_t : public interruption_t
    {

        public:

            typedef std::function<void()> callback_t;

            void allow()
            {
                mark_as_handled();
                on_allowed();
            }

            void refuse()
            {
                mark_as_handled();
                on_refused();
            }

            void execute_unhandled_handler() final
            {
                switch(default_is)
                {
                    case default_t::allowed: allow(); break;
                    case default_t::refused: refuse(); break;
                }
            }

        protected:

            enum class default_t
            {
                allowed,
                refused
            };

            interruption_ask_for_t(type_t type, app_t::instance_id_t target, default_t default_is, callback_t on_allowed = [](){}, callback_t on_refused = [](){}) : interruption_t(type, target), default_is(default_is), on_allowed(std::move(on_allowed)), on_refused(std::move(on_refused))
            {

            }

        private:
            default_t default_is;
            callback_t on_allowed;
            callback_t on_refused;

    };


}

#include "global/core/app/interruption/ask_for/close.hpp"
#include "global/core/app/interruption/ask_for/new_instance.hpp"

#endif