#ifndef ENGINE_CORE_CONSOLE_MSG_ACCOUNTABLE_HPP
#define ENGINE_CORE_CONSOLE_MSG_ACCOUNTABLE_HPP
#pragma once

#include <chrono>
#include "utility/text/ustring.hpp"
#include "utility/pattern/flags.hpp"
#include "utility/debug/callstack.hpp"
#include "utility/platform/debug.hpp"
#include "global/core/messenger/msg.hpp"
#include "global/core/thread/meta.hpp"
#include "global/core/app/meta.hpp"

namespace engine
{

    namespace console
    {

#include "def/enum/console_accounter.def"

        template<class object_t> class accountable_item_t : public messenger::msg_console_t::actual_t
        {
        public:
            
            virtual ~accountable_item_t()
            {

            }

            std::shared_ptr<object_t> get_object()
            {
                return object;
            }

            accountable_type_t get_accountable_type() const
            {
                return accountable_type;
            }

        protected:
        
            accountable_item_t(std::shared_ptr<object_t> object, accountable_type_t accountable_type, type_t type, source_t source, app_t::kind_t app = app_t::kind_t::_engine, app_t::instance_id_t app_instance_id = 0) : actual_t(app, app_instance_id, type, source, kind_t::msg), object(object), accountable_type(accountable_type)
            {

            }
            
        private:

            std::shared_ptr<object_t> object;
            accountable_type_t accountable_type;

        };

        class thread_accountable_item_t : public accountable_item_t<thread_meta_t>
        {

        public:

            thread_accountable_item_t(std::shared_ptr<thread_meta_t> thread_meta, accountable_type_t accountable_type) : accountable_item_t(thread_meta, accountable_type, messenger::msg_console_t::actual_t::type_t::thread_accountable, messenger::msg_console_t::actual_t::source_t::threads, thread_meta->get_app(), thread_meta->get_app_instance_id())
            {

            }


        };

        class app_accountable_item_t : public accountable_item_t<app_meta_t>
        {

        public:

            app_accountable_item_t(std::shared_ptr<app_meta_t> app_meta, accountable_type_t accountable_type) : accountable_item_t(app_meta, accountable_type, messenger::msg_console_t::actual_t::type_t::app_accountable, messenger::msg_console_t::actual_t::source_t::apps, app_meta->get_app(), app_meta->get_instance_id())
            {

            }


        };

    }

}


#define ENGINE_ENUM_HEADER_TO_USE "def/enum/console_accounter.def"
#include "global/core/utility/enum_to_string.hpp"

#endif