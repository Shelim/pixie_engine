#ifndef ENGINE_IGNITER_INSTANCES_PROGRAM_MULTIPLE_HPP
#define ENGINE_IGNITER_INSTANCES_PROGRAM_MULTIPLE_HPP
#pragma once

#include "global/core/policy/instances_program.hpp"

namespace engine
{
    class instances_program_multiple_t : public instances_program_t
    {

    public:

        instances_program_multiple_t() : instances_program_t(type_t::allow_multiple)
        {

        }
        
    };

}

#endif