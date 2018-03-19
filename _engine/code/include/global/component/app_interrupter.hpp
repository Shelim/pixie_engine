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

#define ENGINE_APP_INTERRUPTION_PRIORITY_DEF(...) DEFINE_ENUM_ONLY_1ST_TYPE(priority_t, __VA_ARGS__)
#include "def/app_interruption.def"

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

#define ENGINE_APP_INTERRUPTION_PRIORITY_DEF(...) DEFINE_ENUM_ONLY_1ST_TO_STRING(engine::app_interrupter_t::priority_t, __VA_ARGS__)
#include "def/app_interruption.def"

#include "global/component/app_interrupter/dummy.hpp"
#include "global/component/app_interrupter/real.hpp"

#endif