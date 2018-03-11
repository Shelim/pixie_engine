#ifndef ENGINE_GLOBAL_CORE_APP_HPP
#define ENGINE_GLOBAL_CORE_APP_HPP
#pragma once

#include <memory>
#include "global/core/app/context.hpp"
#include "utility/concurrention/signal.hpp"
#include "utility/pattern/provider.hpp"

namespace engine
{
    class app_actual_t;
    class accountable_app_t;
    class thread_factory_t;
    class accountable_app_instance_t;
    class thread_t;
    class app_meta_t;

    class app_t
    {

        public:

#include "def/enum/app.def"

            typedef int32_t instance_id_t;

#include "def/enum/main_return_code.def"

            app_t(std::unique_ptr<app_actual_t> app, std::unique_ptr<app_context_t> context, std::shared_ptr<holder_t<accountable_app_t> > notifier, std::shared_ptr<thread_factory_t> thread_factory);

            ~app_t();

            std::shared_ptr<app_meta_t> get_meta()
            {
                return meta;
            }
            
        private:

            void run_app();

            app_context_t * context;
            
            std::unique_ptr<app_actual_t> app;
            std::unique_ptr<accountable_app_instance_t > instance;
            std::shared_ptr<app_meta_t> meta;

			std::unique_ptr<thread_t> thread;

    };

    
}

#define ENGINE_ENUM_HEADER_TO_USE "def/enum/main_return_code.def"
#include "global/core/utility/enum_to_string.hpp"

#define ENGINE_ENUM_HEADER_TO_USE "def/enum/app.def"
#include "global/core/utility/enum_to_string.hpp"

#include "global/core/app/actual.hpp"
#include "global/core/app/meta.hpp"
#include "global/core/app/factory.hpp"

#endif