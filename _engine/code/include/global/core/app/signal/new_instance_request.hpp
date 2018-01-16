#ifndef ENGINE_GLOBAL_CORE_APP_SIGNAL_NEW_INSTANCE_REQUEST_HPP
#define ENGINE_GLOBAL_CORE_APP_SIGNAL_NEW_INSTANCE_REQUEST_HPP
#pragma once

#include "global/core/app/signal.hpp"
#include "utility/text/ustring.hpp"

namespace engine
{

    namespace global
    {

        class app_signal_new_instance_request_t : public app_signal_t
        {

        public:

            app_signal_new_instance_request_t(const ustring_collection_t & args) : app_signal_t(app_signal_t::type_t::new_instance_request), args(args), result(result_t::allow)
            {
                
            }

            const ustring_collection_t & get_args() const
            {
                return args;
            }

            enum class result_t
            {
                allow,
                deny,
                count
            };

            result_t get_result() const
            {
                return result;
            }

            void deny()
            {
                result = result_t::deny;
            }

        private:

            const ustring_collection_t & args;
            result_t result;

        };

    }

}

#endif