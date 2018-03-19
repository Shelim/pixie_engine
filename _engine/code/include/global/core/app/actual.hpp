#ifndef ENGINE_GLOBAL_CORE_APP_ACTUAL_HPP
#define ENGINE_GLOBAL_CORE_APP_ACTUAL_HPP
#pragma once

#include "global/core/app.hpp"
#include "global/core/app/main.hpp"
#include "utility/pattern/flags.hpp"
#include "utility/text/ustring.hpp"
#include "global/core/app/context.hpp"

namespace engine
{

    class app_actual_t
    {

        public:

            virtual ~app_actual_t()
            {

            }

            virtual app_t::return_code_t main(app_context_t* context) = 0;

            app_t::kind_t get_app() const
            {
                return app;
            }

        protected:

            app_actual_t(app_t::kind_t app) : app(app)
            {

            }

        private:

            app_t::kind_t app;
            
    };

    namespace client
    {

#define GAME_APP_IMPL(app) \
        class app_##app##_t : public app_actual_t \
        { \
            public: \
                app_##app##_t() : app_actual_t(app_t::kind_t::app) { } \
                app_t::return_code_t main(app_context_t* context) final { return main_##app(context); } \
        };
#define GAME_APP_DEF(...) DEFINE_TYPE_PASS(GAME_APP_IMPL, __VA_ARGS__)
#include "def/app.def"

    }

}

#endif