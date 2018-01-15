#ifndef ENGINE_COMMON_UTILITY_MESSENGER_MESSENGER_HPP
#define ENGINE_COMMON_UTILITY_MESSENGER_MESSENGER_HPP
#pragma once

#include "core/messenger/queue.hpp"
#include "core/messenger/msg/config_storage.hpp"
#include "core/messenger/msg/config.hpp"
#include "core/messenger/msg/console.hpp"


namespace engine
{
    
#define ENGINE_MESSENGER_QUEUE_DEF(name) typedef messenger::queue_t<messenger::msg_##name##_t> messenger_##name##_t;
#include "def/messenger.def"

}

#endif