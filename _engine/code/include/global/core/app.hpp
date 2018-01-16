#ifndef ENGINE_GLOBAL_CORE_APP_HPP
#define ENGINE_GLOBAL_CORE_APP_HPP
#pragma once

#include "utility/pattern/flags.hpp"
#include "utility/text/ustring.hpp"
#include "global/core/app/signal.hpp"
#include "global/core/app/context.hpp"
#include "core/manifest/app.hpp"

namespace engine
{

    namespace global
    {

        class app_t
        {

            public:

                virtual ~app_t()
                {

                }

    #include "def/enum/main_return_code.def"

                virtual return_code_t main(std::unique_ptr<app_context_t> context) = 0;

                virtual void signal(app_signal_t * signal)
                {

                }

                manifest_app_t::app_t get_app() const
                {
                    return app;
                }

            protected:

                app_t(manifest_app_t::app_t app) : app(app)
                {

                }

            private:

                manifest_app_t::app_t app;
                
        };

    }

}

#define ENGINE_ENUM_HEADER_TO_USE "def/enum/main_return_code.def"
#include "core/utility/enum_to_string.hpp"

#endif