#ifndef ENGINE_CLIENT_RESOLVE_APP_HPP
#define ENGINE_CLIENT_RESOLVE_APP_HPP
#pragma once

#include "global/core/app.hpp"
#include "global/component/program_args.hpp"

namespace engine
{

    namespace client
    {

        class resolve_app_t
        {

            public:

                resolve_app_t(std::shared_ptr<program_args_t> program_args) : program_args(program_args)
                {

                }
            
                engine::app_t::kind_t resolve_app(app_context_t * context)
                {
                    return engine::app_t::kind_t::game; // ToDo: This should be populated from context above!!!
                }

                engine::args_t startup_args_to_run_new_instance_of(app_t::kind_t kind)
                {
                    return program_args->get_program_args();  // ToDo: This should be populated from kind above!!!
                }

            private:

                std::shared_ptr<program_args_t> program_args;
        };

    }


}

#endif