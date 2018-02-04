#ifndef ENGINE_GLOBAL_CORE_APP_SIGNAL_HPP
#define ENGINE_GLOBAL_CORE_APP_SIGNAL_HPP
#pragma once

namespace engine
{
    class app_signal_t
    {

        public:

            virtual ~app_signal_t()
            {

            }

#include "def/enum/app_signal.def"

            type_t get_type() const
            {
                return type;
            }

        protected:

            app_signal_t(type_t type) : type(type)
            {
                
            }

        private:

            type_t type;

    };

}

#define ENGINE_ENUM_HEADER_TO_USE "def/enum/app_signal.def"
#include "global/core/utility/enum_to_string.hpp"

#include "global/core/app/signal/new_instance_request.hpp"
#include "global/core/app/signal/terminate.hpp"

#endif