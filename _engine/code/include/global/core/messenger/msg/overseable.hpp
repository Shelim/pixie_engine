#ifndef ENGINE_COMMON_UTILITY_MESSENGER_MSG_OVERSEABLE_HPP
#define ENGINE_COMMON_UTILITY_MESSENGER_MSG_OVERSEABLE_HPP
#pragma once

#include <cstdint>
#include <memory>
#include "global/core/messenger/msg.hpp"
#include "utility/pattern/fourcc.hpp"
#include "global/component/config_common/enum.hpp"

namespace engine
{

    class thread_t;
	
    namespace messenger
    {

#include "def/enum/overseable.def"

        template<class object_t> class msg_overseable_t
		{

		public:

			static constexpr bool is_queue_async = false;
			static constexpr bool is_instance_async = false;
			static constexpr bool keep_history = true;

            overseable_type_t get_type() const
            {
                return type;
            }

            object_t * get_oversee()
            {
                return oversee;
            }

            msg_overseable_t(overseable_type_t type, object_t * oversee) : type(type), oversee(oversee)
            {
                
            }

            virtual ~msg_overseable_t()
            {

            }

        private:
        
            overseable_type_t type;
            object_t * oversee;

        };

#define ENGINE_OVERSEABLE_DEF(item) class msg_overseable_##item##_t : public msg_overseable_t<item##_t> { public: msg_overseable_##item##_t(overseable_type_t type, item##_t * oversee) : msg_overseable_t(type, oversee){} };
#include "def/overseable.def"

    }

}

#define ENGINE_ENUM_HEADER_TO_USE "def/enum/overseable.def"
#include "global/core/utility/enum_to_string.hpp"

#endif