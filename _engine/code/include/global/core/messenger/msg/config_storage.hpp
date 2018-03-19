#ifndef ENGINE_COMMON_UTILITY_MESSENGER_MSG_CONFIG_STORAGE_HPP
#define ENGINE_COMMON_UTILITY_MESSENGER_MSG_CONFIG_STORAGE_HPP
#pragma once

#include <cstdint>
#include <memory>
#include "global/core/messenger/msg.hpp"
#include "utility/pattern/fourcc.hpp"
#include "global/component/config_common/enum.hpp"

namespace engine
{
	
    namespace messenger
    {

        class msg_config_storage_t
		{

		public:

			static constexpr bool is_queue_async = false;
			static constexpr bool is_instance_async = false;
			static constexpr bool keep_history = false;

#define ENGINE_CONFIG_STORAGE_MSG(...) DEFINE_ENUM_ONLY_1ST_TYPE(type_t, __VA_ARGS__)
#include "def/config.def"

            type_t get_type() const
            {
                return type;
            }

            msg_config_storage_t(type_t type) : type(type)
            {

            }

        private:
        
            type_t type;

        };

    }

}

#define ENGINE_CONFIG_STORAGE_MSG(...) DEFINE_ENUM_ONLY_1ST_TO_STRING(engine::messenger::msg_config_storage_t::type_t, __VA_ARGS__)
#include "def/config.def"

#endif