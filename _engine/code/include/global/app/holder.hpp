#ifndef ENGINE_GLOBAL_APP_HOLDER_HPP
#define ENGINE_GLOBAL_APP_HOLDER_HPP
#pragma once

#include <memory>
#include "global/app.hpp"
#include "global/app/context.hpp"
#include "utility/concurrention/signal.hpp"

namespace engine
{

    namespace global
    {

        class app_holder_t
        {

            public:

                app_holder_t(std::unique_ptr<app_t> app, std::unique_ptr<app_context_t> context) : app(std::move(app)), context(std::move(context)), return_code(app_t::return_code_t::still_running), thread([this]{this->run_app();})
                {
                    
                }

                ~app_holder_t()
                {
                    wait_till_completed();
                    thread.join();
                }

                app_t::return_code_t get_return_code()
                {
                    return return_code;
                }

                void wait_till_completed()
                {
                    signal_for_completion.wait();
                }

                enum class result_t
                {
                    successful,
                    rejected
                };

                result_t terminate(app_signal_terminate_t::reason_t reason)
                {
                    app_signal_terminate_t signal_terminate(reason);
                    send_signal(&signal_terminate);

                    if(signal_terminate.is_cancelled())
                        return result_t::rejected;

                    wait_till_completed();
                    return result_t::successful;
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

                std::thread thread;
                signal_t signal_for_completion;

        };

    }

}


#endif