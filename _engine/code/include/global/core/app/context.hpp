#ifndef ENGINE_GLOBAL_CORE_APP_CONTEXT_HPP
#define ENGINE_GLOBAL_CORE_APP_CONTEXT_HPP
#pragma once

#include "utility/text/ustring.hpp"
#include "utility/platform/args.hpp"

namespace engine
{

    namespace global
    {

        class program_t;

        class app_context_t
        {

        public:

            app_context_t(std::shared_ptr<program_t> program, const args_t & args) : program(program), args(args)
            {

            }

            std::shared_ptr<program_t> get_program()
            {
                return program;
            }

            const args_t & get_args()
            {
                return args;
            }

        private:

            std::shared_ptr<program_t> program;
            args_t args;
            
        };

    }

}

#endif