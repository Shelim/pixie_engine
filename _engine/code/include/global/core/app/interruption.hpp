#ifndef ENGINE_GLOBAL_CORE_APP_INTERRUPTION_HPP
#define ENGINE_GLOBAL_CORE_APP_INTERRUPTION_HPP
#pragma once


#include "global/core/app.hpp"

namespace engine
{
    class app_context_t;

    class interruption_t
    {

    public:

        virtual ~interruption_t()
        {

        }

#define ENGINE_APP_INTERRUPTION_DEF(...) DEFINE_ENUM_ONLY_1ST_TYPE(type_t, __VA_ARGS__)
#include "def/app_interruption.def"

        type_t get_type() const
        {
            return type;
        }

        app_t::instance_id_t get_target() const
        {
            return target;
        }

        template<class actual_t> actual_t * get()
        {
            return std::static_cast<actual_t*>(this);
        }

        void mark_as_handled()
        {
            flags.set_flag(flag_t::was_handled, true);
        }

        virtual void execute_unhandled_handler()
        {
            // Do nothing
        }

        bool was_handled()
        {
            return flags.is_flag(flag_t::was_handled);
        }

    protected:

        interruption_t(type_t type, app_t::instance_id_t target) : type(type), target(target)
        {

        }
    
    private:
        
        app_t::instance_id_t target;
        enum class flag_t
        {
            was_handled,
            count
        };
        flags_t<flag_t> flags;

        type_t type;

    };
}

#define ENGINE_APP_INTERRUPTION_DEF(...) DEFINE_ENUM_ONLY_1ST_TO_STRING(engine::interruption_t::type_t, __VA_ARGS__)
#include "def/app_interruption.def"

#include "global/core/app/interruption/ask_for.hpp"
#include "global/core/app/interruption/terminate.hpp"

#endif