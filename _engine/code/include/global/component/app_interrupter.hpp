#ifndef ENGINE_GLOBAL_COMPONENT_APP_INTERRUPTER_HPP
#define ENGINE_GLOBAL_COMPONENT_APP_INTERRUPTER_HPP
#pragma once

#include "global/core/app.hpp"

namespace engine
{
    class app_context_t;
    
    class interruption_t
    {

    public:

        virtual ~interruption_t()
        {

        }

        bool was_handled();
        bool begin_handling();
        void end_handling();

    };

    class app_interrupter_t
    {

    public:

        virtual ~app_interrupter_t()
        {

        }

        /*

        class reciever_queue_t
        {
            public:

                virtual ~reciever_queue_t()
                {

                }

                virtual std::shared_ptr<interruption_t> poll();
                virtual std::shared_ptr<interruption_t> wait_for();
        };

        virtual std::unique_ptr<reciever_queue_t> get_reciever(app_context_t * app_context) = 0;

        virtual void send_interruption(app_t::instance_id_t target, std::shared_ptr<interruption_t> interruption) = 0;

        */
    };

}

#include "global/component/app_interrupter/dummy.hpp"
#include "global/component/app_interrupter/real.hpp"

#endif