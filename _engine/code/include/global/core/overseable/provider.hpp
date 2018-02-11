
#ifndef ENGINE_OSERSEABLE_PROVIDER_HPP
#define ENGINE_OSERSEABLE_PROVIDER_HPP
#pragma once


namespace engine
{

#define ENGINE_OVERSEABLE_DEF(type) class overseable_##type##_provider_base_t { \
	public:	virtual ~overseable_##type##_provider_base_t() {}\
        virtual void notify(engine::messenger::overseable_type_t overseable_type, type##_t * type) = 0; \
	};
#include "def/overseable.def"

}

#include "global/core/overseable/provider/messenger.hpp"
#include "global/core/overseable/provider/mockup.hpp"

#endif