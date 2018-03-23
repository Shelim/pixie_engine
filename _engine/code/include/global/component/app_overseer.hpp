#ifndef ENGINE_GLOBAL_COMPONENT_APP_OVERSEER_HPP
#define ENGINE_GLOBAL_COMPONENT_APP_OVERSEER_HPP
#pragma once

#include "global/core/app.hpp"
#include <memory>

namespace engine
{

    class app_overseer_t
    {

    public:

        virtual ~app_overseer_t()
        {

        }

        typedef std::function<void()> callback_void_t;
        typedef std::function<void(std::shared_ptr<app_t>)> callback_app_t;
        typedef std::function<void(app_t::instance_id_t)> callback_app_instance_t;

#define ENGINE_APP_OVERSEER_RUN_INSTANCE_DEF(...) DEFINE_ENUM_ONLY_1ST_TYPE(run_app_instance_t, __VA_ARGS__)
#define ENGINE_APP_OVERSEER_RUN_OTHER_DEF(...) DEFINE_ENUM_ONLY_1ST_TYPE(run_app_other_t, __VA_ARGS__)
#include "def/app_overseer.def"

        virtual void run_app(app_t::kind_t kind, std::unique_ptr<app_context_t> context, run_app_instance_t instance = run_app_instance_t::allow_multiple, run_app_other_t other = run_app_other_t::keep, callback_app_t app_running = [](std::shared_ptr<app_t>){}, callback_void_t run_failed = [](){}) = 0;
        virtual void terminate_app(app_t::instance_id_t instance, callback_void_t on_completed = [](){}) = 0;
        virtual void close_app(app_t::instance_id_t instance, callback_void_t on_accepted = [](){}, callback_void_t on_rejected = [](){}) = 0;
        virtual void terminate_all_of(app_t::kind_t app, callback_void_t on_completed = [](){}) = 0;
        virtual void close_all_of(app_t::kind_t app, callback_void_t on_all_accepted = [](){}, callback_app_instance_t on_given_accepted = [](app_t::instance_id_t){}, callback_app_instance_t on_given_rejected = [](app_t::instance_id_t){}) = 0;
        virtual void terminate_all(callback_void_t on_completed = [](){}) = 0;
        virtual void close_all(callback_void_t on_all_accepted = [](){}, callback_app_instance_t on_given_accepted = [](app_t::instance_id_t){}, callback_app_instance_t on_given_rejected = [](app_t::instance_id_t){}) = 0;
        virtual void wait_for_completion() = 0;

    };

}

#define ENGINE_APP_OVERSEER_RUN_INSTANCE_DEF(...) DEFINE_ENUM_ONLY_1ST_TO_STRING(engine::app_overseer_t::run_app_instance_t, __VA_ARGS__)
#define ENGINE_APP_OVERSEER_RUN_OTHER_DEF(...) DEFINE_ENUM_ONLY_1ST_TO_STRING(engine::app_overseer_t::run_app_other_t, __VA_ARGS__)
#include "def/app_overseer.def"

#include "global/component/app_overseer/dummy.hpp"
#include "global/component/app_overseer/real.hpp"

#endif