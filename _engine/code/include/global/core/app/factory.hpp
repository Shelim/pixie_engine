#ifndef ENGINE_GLOBAL_CORE_APP_FACTORY_HPP
#define ENGINE_GLOBAL_CORE_APP_FACTORY_HPP
#pragma once

#include "global/core/app.hpp"
#include "global/core/app/main.hpp"
#include "utility/pattern/factory.hpp"

namespace engine
{
    
    class app_factory_t
    {

        public:

            app_factory_t(
#define GAME_APP_DEF(app) std::shared_ptr<engine::ifactory<client::app_##app##_t> > app_##app##_factory,
#include "def/app.def"
                std::shared_ptr<holder_t<accountable_app_t> > notifier, std::shared_ptr<thread_factory_t> thread_factory) :
#define GAME_APP_DEF(app) app_##app##_factory(app_##app##_factory),
#include "def/app.def"
                notifier(notifier), thread_factory(thread_factory)
            {

            }

            std::shared_ptr<app_t> create(app_t::kind_t kind, std::unique_ptr<app_context_t> context)
            {
                switch(kind)
                {
#define GAME_APP_DEF(app) case app_t::kind_t::app: return std::make_shared<app_t>(app_##app##_factory->create(), std::move(context), notifier, thread_factory);
                }
                return std::shared_ptr<app_t>();
            }

        private:

#define GAME_APP_DEF(app) std::shared_ptr<engine::ifactory<client::app_##app##_t> > app_##app##_factory;
#include "def/app.def"

            std::shared_ptr<holder_t<accountable_app_t> > notifier;
            std::shared_ptr<thread_factory_t> thread_factory;

    };
}

#endif