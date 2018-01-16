#ifndef ENGINE_GLOBAL_CORE_APP_CONTEXT_HPP
#define ENGINE_GLOBAL_CORE_APP_CONTEXT_HPP
#pragma once

#include "utility/text/ustring.hpp"

namespace engine
{

    namespace global
    {

        class program_t;

        class app_context_t
        {

        public:

            app_context_t(program_t * program, const ustring_collection_t & args) : program(program), args(args)
            {

            }

            const ustring_collection_t & get_args()
            {
                return args;
            }

            program_t * get_program()
            {
                return program;
            }

        private:

            ustring_collection_t args;
            program_t *program;
            
        };

    }

}

#endif