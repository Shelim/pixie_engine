#ifndef ENGINE_COMMON_UTILITY_MESSENGER_MESSENGER_HPP
#define ENGINE_COMMON_UTILITY_MESSENGER_MESSENGER_HPP
#pragma once

#include "global/core/messenger/queue.hpp"
#include "global/core/messenger/msg/accountable.hpp"
#include "global/core/messenger/msg/config_storage.hpp"
#include "global/core/messenger/msg/config.hpp"
#include "global/core/messenger/msg/console.hpp"


namespace engine
{

    namespace messenger
    {

        // Workaround: accountable thread should not have thread in constructor, otherwise we have infinite depedenad loop
        template<> class queue_t<msg_accountable_thread_t> : public queue_base_t<queue_t<msg_accountable_thread_t>, msg_accountable_thread_t, msg_accountable_thread_t::is_queue_async, msg_accountable_thread_t::keep_history>
        {
            static_assert(msg_accountable_thread_t::is_queue_async == false, "Message accountable thread cannot be async (because threading is based on it!)");
            public:
                queue_t() : queue_base_t<queue_t<msg_accountable_thread_t>, msg_accountable_thread_t, msg_accountable_thread_t::is_queue_async, msg_accountable_thread_t::keep_history>(nullptr)
                {

                }

        };
    }
    
#define ENGINE_MESSENGER_QUEUE_IMPL(name) typedef messenger::queue_t<messenger::msg_##name##_t> messenger_##name##_t;
#define ENGINE_MESSENGER_QUEUE_DEF(...) DEFINE_TYPE_PASS(ENGINE_MESSENGER_QUEUE_IMPL, __VA_ARGS__)
#include "def/messenger.def"

}

#endif