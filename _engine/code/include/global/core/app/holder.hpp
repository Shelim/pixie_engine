#ifndef ENGINE_GLOBAL_CORE_APP_HOLDER_HPP
#define ENGINE_GLOBAL_CORE_APP_HOLDER_HPP
#pragma once

#include <memory>
#include "global/core/app.hpp"
#include "global/core/app/context.hpp"
#include "utility/concurrention/signal.hpp"
#include "global/core/thread/thread.hpp"

namespace engine
{
    class app_holder_t
    {

        public:

            app_holder_t(std::unique_ptr<app_t> app, std::unique_ptr<app_context_t> context, std::shared_ptr<thread_factory_t> thread_factory) : app(std::move(app)), context(std::move(context)), return_code(app_t::return_code_t::still_running)
            {
                thread = thread_factory->create(this->app->get_app(), this->context->get_instance_id(), format_string("App #1####2# main thread"_u, this->app->get_app(), this->context->get_instance_id()), [this]{this->run_app();});
            }

            ~app_holder_t()
            {
                wait_till_completed();
            }

            app_t::return_code_t get_return_code()
            {
                return return_code;
            }

            void wait_till_completed()
            {
                signal_for_completion.wait();
            }

            void send_signal(app_signal_t * app_signal)
            {
                app->signal(app_signal);
            }

        private:

            void run_app()
            {
                signal_t::guard_t guard(&signal_for_completion);
                return_code = app->main(std::move(context));
            }
            
            std::unique_ptr<app_t> app;
            std::unique_ptr<app_context_t> context;

            app_t::return_code_t return_code;

			std::unique_ptr<thread_t> thread;
            signal_t signal_for_completion;

    };
    
}


#endif