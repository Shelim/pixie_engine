#ifndef ENGINE_GLOBAL_COMPONENT_APP_INTERRUPTER_HPP
#define ENGINE_GLOBAL_COMPONENT_APP_INTERRUPTER_HPP
#pragma once

#include "global/core/app.hpp"
#include "global/core/app/interruption.hpp"

namespace engine
{
    
    class app_interrupter_t
    {

    public:

        virtual ~app_interrupter_t()
        {

        }

#include "def/enum/app_interruption_priority.def"

        class instance_t
        {

            public:
                virtual ~instance_t()
                {

                }

        };

        typedef std::function<void(interruption_t*)> handler_t;

        virtual std::unique_ptr<instance_t> register_handler(app_t::kind_t app, app_context_t* context, priority_t priority, handler_t handler) = 0;
        virtual void send_interruption(std::unique_ptr<interruption_t> interruption) = 0;

    private:


    };

}
#define ENGINE_ENUM_HEADER_TO_USE "def/enum/app_interruption.def"
#include "global/core/utility/enum_to_string.hpp"

#define ENGINE_ENUM_HEADER_TO_USE "def/enum/app_interruption_priority.def"
#include "global/core/utility/enum_to_string.hpp"

#include "global/component/app_interrupter/dummy.hpp"
#include "global/component/app_interrupter/real.hpp"

#endif