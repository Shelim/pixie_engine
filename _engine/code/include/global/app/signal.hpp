#ifndef ENGINE_GLOBAL_APP_SIGNAL_HPP
#define ENGINE_GLOBAL_APP_SIGNAL_HPP
#pragma once

namespace engine
{

    namespace global
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

}

#define ENGINE_ENUM_HEADER_TO_USE "def/enum/app_signal.def"
#include "core/utility/enum_to_string.hpp"

#include "global/app/signal/new_instance_request.hpp"
#include "global/app/signal/terminate.hpp"

#endif