#ifndef ENGINE_GLOBAL_CORE_UTILITY_ACCOUNTABLE_HPP
#define ENGINE_GLOBAL_CORE_UTILITY_ACCOUNTABLE_HPP
#pragma once

#include <cstdint>
#include <memory>
#include "global/core/messenger/msg.hpp"
#include "global/core/messenger.hpp"
#include "global/core/accountable/provider.hpp"

namespace engine
{

#define ENGINE_ACCOUNTABLE_IMPL(type) \
            class type##_t; \
            \
            class accountable_##type##_t { }; \
            \
	        REGISTER_PROVIDER_BASE_TYPE(accountable_##type##_t, accountable_##type##_provider_base_t) \
            \
            \
            class accountable_##type##_instance_t \
            { \
                public: \
                    \
                    accountable_##type##_instance_t(std::shared_ptr<holder_t<accountable_##type##_t> > notifier, type##_t * type) \
                    :notifier(notifier), type(type) \
                    { \
                        for(int i = 0; i < notifier->get_providers_count(); i++) \
                            notifier->get_provider(i)->notify(engine::messenger::accountable_type_t::create, type);\
                    } \
                    ~accountable_##type##_instance_t() \
                    {  \
                        for(int i = 0; i < notifier->get_providers_count(); i++) \
                            notifier->get_provider(i)->notify(engine::messenger::accountable_type_t::destroy, type);\
                    } \
                private: \
                    std::shared_ptr<holder_t<accountable_##type##_t> > notifier; \
                    type##_t * type; \
            }; \

#define ENGINE_ACCOUNTABLE_DEF(...) DEFINE_TYPE_PASS(ENGINE_ACCOUNTABLE_IMPL, __VA_ARGS__)
#include "def/accountable.def"

}

#endif