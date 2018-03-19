
#ifndef ENGINE_ACCOUNTABLE_PROVIDER_HPP
#define ENGINE_ACCOUNTABLE_PROVIDER_HPP
#pragma once


namespace engine
{

#define ENGINE_ACCOUNTABLE_IMPL(type) \
	\
	class accountable_##type##_provider_base_t \
	{ \
	public:	\
		virtual ~accountable_##type##_provider_base_t() {} \
        virtual void notify(engine::messenger::accountable_type_t accountable_type, type##_t * type) = 0; \
	};

#define ENGINE_ACCOUNTABLE_DEF(...) DEFINE_TYPE_PASS(ENGINE_ACCOUNTABLE_IMPL, __VA_ARGS__)
#include "def/accountable.def"

}

#include "global/core/accountable/provider/messenger.hpp"
#include "global/core/accountable/provider/mockup.hpp"

#endif