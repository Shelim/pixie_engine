#ifndef ENGINE_GLOBAL_CORE_APP_INTERRUPTION_ASK_FOR_HPP
#define ENGINE_GLOBAL_CORE_APP_INTERRUPTION_ASK_FOR_HPP
#pragma once


#include "global/core/app/interruption.hpp"

namespace engine
{

    class interruption_ask_for_t : public interruption_t
    {

        public:

            typedef std::function<void()> callback_t;

            enum class default_t
            {
                allowed,
                refused
            };

            class handle_t
            {
                public:

                    handle_t(default_t default_is, callback_t on_allowed = [](){}, callback_t on_refused = [](){}) : default_is(default_is), on_allowed(on_allowed), on_refused(on_refused)
                    {

                    }

                    ~handle_t()
                    {
                        execute_unhandled_handler();
                    }

                    void allow()
                    {
                        if(!was_handled())
                        {
                            mark_as_handled();
                            on_allowed();
                        }
                    }

                    void refuse()
                    {
                        if(!was_handled())
                        {
                            mark_as_handled();
                            on_refused();
                        }
                    }

                    void execute_unhandled_handler() 
                    {
                        switch(default_is)
                        {
                            case default_t::allowed: allow(); break;
                            case default_t::refused: refuse(); break;
                        }
                    }

                    bool was_handled()
                    {
                        return flags.is_flag(flag_t::was_handled);
                    }
                
                private:

                    enum class flag_t
                    {
                        was_handled,
                        count
                    };

                    flags_t<flag_t> flags;

                    void mark_as_handled()
                    {
                        flags.set_flag(flag_t::was_handled, true);
                    }

                    default_t default_is;
                    callback_t on_allowed;
                    callback_t on_refused;
            };

            void allow()
            {
                mark_as_handled();
                if(handle) handle->allow();
            }

            void refuse()
            {
                mark_as_handled();
                if(handle) handle->refuse();
            }

            void execute_unhandled_handler() final
            {
                mark_as_handled();
                if(handle) handle->execute_unhandled_handler();
            }
           
            std::unique_ptr<handle_t> extract_handle_for_later_use()
            {
                mark_as_handled();
                return std::move(handle);
            }

        protected:

            interruption_ask_for_t(type_t type, app_t::instance_id_t target, default_t default_is, callback_t on_allowed = [](){}, callback_t on_refused = [](){}) : interruption_t(type, target), handle(std::make_unique<handle_t>(default_is, std::move(on_allowed), std::move(on_refused)))
            {

            }

        private:
           
            std::unique_ptr<handle_t> handle;

    };


}

#include "global/core/app/interruption/ask_for/close.hpp"
#include "global/core/app/interruption/ask_for/new_instance.hpp"

#endif