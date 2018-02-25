#ifndef ENGINE_GLOBAL_CORE_APP_CONTEXT_HPP
#define ENGINE_GLOBAL_CORE_APP_CONTEXT_HPP
#pragma once

#include "utility/text/ustring.hpp"
#include "utility/platform/args.hpp"
#include <functional>

namespace engine
{

    class instance_id_t {};

    extern instance_id_t instance_id;

    class program_t;

    class app_context_t
    {

    public:

        app_context_t(std::shared_ptr<program_t> program, const args_t & args) : program(program), args(args), instance_id(next_instance_id++)
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

        int32_t get_instance_id() const
        {
            return instance_id;
        }

    private:

        std::shared_ptr<program_t> program;
        args_t args;
        int32_t instance_id;
        static int32_t next_instance_id;
    };

}

#endif