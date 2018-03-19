
#ifndef ENGINE_OSERSEABLE_PROVIDER_MESSENGER_HPP
#define ENGINE_OSERSEABLE_PROVIDER_MESSENGER_HPP
#pragma once

#include "global/core/messenger/msg.hpp"
#include "global/core/messenger.hpp"
#include "global/core/accountable/provider.hpp"
#include "utility/pattern/provider.hpp"

namespace engine
{

#define ENGINE_ACCOUNTABLE_IMPL(type) \
    class accountable_##type##_provider_messenger_t : public accountable_##type##_provider_base_t \
    {\
	public: \
        accountable_##type##_provider_messenger_t(std::shared_ptr<engine::messenger_accountable_##type##_t> messenger) \
            : messenger(messenger) \
            { \
            } \
        void notify(engine::messenger::accountable_type_t accountable_type, type##_t * type) final \
        { \
            messenger->write(std::make_unique<engine::messenger::msg_accountable_##type##_t>(accountable_type, type)); \
        }\
    private:\
        std::shared_ptr<engine::messenger_accountable_##type##_t> messenger;	\
    };

#define ENGINE_ACCOUNTABLE_DEF(...) DEFINE_TYPE_PASS(ENGINE_ACCOUNTABLE_IMPL, __VA_ARGS__)
#include "def/accountable.def"

}

#endif