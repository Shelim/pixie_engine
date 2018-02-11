
#ifndef ENGINE_OSERSEABLE_PROVIDER_MESSENGER_HPP
#define ENGINE_OSERSEABLE_PROVIDER_MESSENGER_HPP
#pragma once

#include "global/core/messenger/msg.hpp"
#include "global/core/messenger/messenger.hpp"
#include "global/core/overseable/provider.hpp"
#include "utility/pattern/provider.hpp"

namespace engine
{

#define ENGINE_OVERSEABLE_DEF(type) class overseable_##type##_provider_messenger_t : public overseable_##type##_provider_base_t {\
	public: \
        overseable_##type##_provider_messenger_t(std::shared_ptr<engine::messenger_overseable_##type##_t> messenger) : messenger(messenger){ }\
        void notify(engine::messenger::overseable_type_t overseable_type, type##_t * type) final \
        { messenger->write(std::make_unique<engine::messenger::msg_overseable_##type##_t>(overseable_type, type)); }\
    private:\
        std::shared_ptr<engine::messenger_overseable_##type##_t> messenger;	};
#include "def/overseable.def"

}

#endif