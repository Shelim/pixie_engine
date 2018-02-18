#ifndef ENGINE_COMMON_UTILITY_MESSENGER_MSG_ACCOUNTABLE_HPP
#define ENGINE_COMMON_UTILITY_MESSENGER_MSG_ACCOUNTABLE_HPP
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

#include "def/enum/accountable.def"

        template<class object_t> class msg_accountable_t
		{

		public:

			static constexpr bool is_queue_async = false;
			static constexpr bool is_instance_async = false;
			static constexpr bool keep_history = true;

            accountable_type_t get_type() const
            {
                return type;
            }

            object_t * get_object()
            {
                return object;
            }

            msg_accountable_t(accountable_type_t type, object_t * object) : type(type), object(object)
            {
                
            }

            virtual ~msg_accountable_t()
            {

            }

        private:
        
            accountable_type_t type;
            object_t * object;

        };

#define ENGINE_ACCOUNTABLE_DEF(item) \
        class msg_accountable_##item##_t : public msg_accountable_t<item##_t> \
        { \
            public: msg_accountable_##item##_t(accountable_type_t type, item##_t * object) \
                : msg_accountable_t(type, object) \
                { \
                } \
        };

#include "def/accountable.def"

    }

}

#define ENGINE_ENUM_HEADER_TO_USE "def/enum/accountable.def"
#include "global/core/utility/enum_to_string.hpp"

#endif