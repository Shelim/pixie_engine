#ifndef ENGINE_GLOBAL_CORE_UTILITY_OVERSEABLE_HPP
#define ENGINE_GLOBAL_CORE_UTILITY_OVERSEABLE_HPP
#pragma once

#include <cstdint>
#include <memory>
#include "global/core/messenger/msg.hpp"
#include "global/core/messenger/messenger.hpp"
#include "global/core/overseable/provider.hpp"

namespace engine
{

#define ENGINE_OVERSEABLE_DEF(type) class type##_t;\
            class overseable_##type##_t { }; \
	        REGISTER_PROVIDER_BASE_TYPE(overseable_##type##_t, overseable_##type##_provider_base_t)\
    class oversee_##type##_instance_t { \
            public: \
            oversee_##type##_instance_t(std::shared_ptr<holder_t<overseable_##type##_t> > notifier, type##_t * type) : notifier(notifier), type(type) \
            {  for(int i = 0; i < notifier->get_providers_count(); i++) notifier->get_provider(i)->notify(engine::messenger::overseable_type_t::create, type); } \
            ~oversee_##type##_instance_t() \
            {  for(int i = 0; i < notifier->get_providers_count(); i++) notifier->get_provider(i)->notify(engine::messenger::overseable_type_t::destroy, type); } \    
        private: \
            std::shared_ptr<holder_t<overseable_##type##_t> > notifier; \
            type##_t * type;  }; \

#include "def/overseable.def"

}

#endif