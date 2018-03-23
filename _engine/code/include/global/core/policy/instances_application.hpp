#ifndef ENGINE_GLOBAL_POLICY_INSTANCES_APPLICATION_HPP
#define ENGINE_GLOBAL_POLICY_INSTANCES_APPLICATION_HPP
#pragma once

#include <memory>
#include "utility/pattern/enum.hpp"

namespace engine
{

    class instances_application_t
    {

    public:

        virtual ~instances_application_t()
        {
            
        }

#define ENGINE_POLICY_INSTANCE_APPLICATION_DEF(...) DEFINE_ENUM_ONLY_1ST_TYPE(type_t, __VA_ARGS__)
#include "def/policy.def"

        virtual bool allow_application_instance_start(app_t::kind_t kind) = 0;

        type_t get_type() const
        {
            return type;
        }

    protected:

        instances_application_t(type_t type) : type(type)
        {

        }

    private:

        type_t type;

    };
}

#define ENGINE_POLICY_INSTANCE_APPLICATION_DEF(...) DEFINE_ENUM_ONLY_1ST_TO_STRING(engine::instances_application_t::type_t, __VA_ARGS__)
#include "def/policy.def"

#include "global/core/policy/instances_application/multiple.hpp"
#include "global/core/policy/instances_application/single.hpp"
#include "global/core/policy/instances_application/single_given_kind.hpp"

#endif