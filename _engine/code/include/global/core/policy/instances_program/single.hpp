#ifndef ENGINE_IGNITER_INSTANCES_PROGRAM_SINGLE_HPP
#define ENGINE_IGNITER_INSTANCES_PROGRAM_SINGLE_HPP
#pragma once

#include "global/core/policy/instances_program.hpp"

namespace engine
{

    class instances_program_single_t : public instances_program_t
    {

    public:

        instances_program_single_t() : instances_program_t(type_t::force_single)
        {

        }

    };

}

#endif