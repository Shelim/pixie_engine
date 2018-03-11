#ifndef ENGINE_GLOBAL_CORE_APP_META_HPP
#define ENGINE_GLOBAL_CORE_APP_META_HPP
#pragma once

#include "global/core/app.hpp"
#include "utility/pattern/flags.hpp"
#include <chrono>

namespace engine
{

    class app_meta_t
    {

    public:

        app_meta_t(app_t::kind_t app, std::unique_ptr<app_context_t> context) : app(app), context(std::move(context)), return_code(app_t::return_code_t::still_running), creation_time(std::chrono::steady_clock::now())
        {

        }

        app_t::kind_t get_app() const
        {
            return app;
        }

        app_t::instance_id_t get_instance_id() const
        {
            return context->get_instance_id();
        }

        app_t::return_code_t get_return_code()
        {
            return return_code;
        }

        void wait_till_completed()
        {
            signal_for_completion.wait();
        }

        std::chrono::duration<double> get_total_time() const
        {
            return std::chrono::steady_clock::now() - creation_time;
        }

    private:

        friend class app_t;

        void set_return_code(app_t::return_code_t return_code)
        {
            this->return_code = return_code;
        }

        void notify_on_completion()
        {
            signal_for_completion.signal();
        }

        app_t::kind_t app;

        std::unique_ptr<app_context_t> context;

        app_t::return_code_t return_code;
        signal_t signal_for_completion;
        
		std::chrono::steady_clock::time_point creation_time;

    };

}

#endif