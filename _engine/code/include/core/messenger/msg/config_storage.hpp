#ifndef ENGINE_COMMON_UTILITY_MESSENGER_MSG_CONFIG_STORAGE_HPP
#define ENGINE_COMMON_UTILITY_MESSENGER_MSG_CONFIG_STORAGE_HPP
#pragma once

#include <cstdint>
#include <memory>
#include "core/messenger/msg.hpp"
#include "utility/pattern/fourcc.hpp"
#include "component/config_common/enum.hpp"
#include "core/manifest/app.hpp"

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

#include "def/enum/config_storage_msg.def"

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

#define ENGINE_ENUM_HEADER_TO_USE "def/enum/config_storage_msg.def"
#include "core/utility/enum_to_string.hpp"

#endif