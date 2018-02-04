#ifndef ENGINE_GLOBAL_CORE_APP_SIGNAL_TERMINATE_HPP
#define ENGINE_GLOBAL_CORE_APP_SIGNAL_TERMINATE_HPP
#pragma once

#include "utility/pattern/flags.hpp"
#include "global/core/app/signal.hpp"

namespace engine
{

    class app_signal_terminate_t : public app_signal_t
    {
        
    public:

#include "def/enum/app_termination_reason.def"

        app_signal_terminate_t(reason_t reason) : app_signal_t(app_signal_t::type_t::terminate), reason(reason)
        {

        }

        void cancel()
        {
            flags.set_flag(flag_t::is_cancelled, true);
        }

        bool is_cancelled() const
        {
            return flags.is_flag(flag_t::is_cancelled);
        }

        reason_t get_reason() const
        {
            return reason;
        }

    private:

        reason_t reason;

        enum class flag_t
        {
            is_cancelled,
            count
        };

        flags_t<flag_t> flags;

    };

}

#define ENGINE_ENUM_HEADER_TO_USE "def/enum/app_termination_reason.def"
#include "global/core/utility/enum_to_string.hpp"

#endif