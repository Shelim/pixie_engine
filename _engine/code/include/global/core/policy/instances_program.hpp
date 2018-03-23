#ifndef ENGINE_GLOBAL_POLICY_INSTANCES_PROGRAM_HPP
#define ENGINE_GLOBAL_POLICY_INSTANCES_PROGRAM_HPP
#pragma once

#include "utility/pattern/enum.hpp"

namespace engine
{

    class instances_program_t
    {

    public:

        virtual ~instances_program_t()
        {

        }

#define ENGINE_POLICY_INSTANCE_PROGRAM_DEF(...) DEFINE_ENUM_ONLY_1ST_TYPE(type_t, __VA_ARGS__)
#include "def/policy.def"

        type_t get_type() const
        {
            return type;
        }
    
    protected:

        instances_program_t(type_t type) : type(type)
        {
            
        }

    private:

        type_t type;

    };

}

#define ENGINE_POLICY_INSTANCE_PROGRAM_DEF(...) DEFINE_ENUM_ONLY_1ST_TO_STRING(engine::instances_program_t::type_t, __VA_ARGS__)
#include "def/policy.def"

#include "global/core/policy/instances_program/multiple.hpp"
#include "global/core/policy/instances_program/single.hpp"

#endif