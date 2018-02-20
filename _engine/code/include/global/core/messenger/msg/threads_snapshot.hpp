#ifndef ENGINE_COMMON_UTILITY_MESSENGER_MSG_THREADS_SNAPSHOT_UPDATED_HPP
#define ENGINE_COMMON_UTILITY_MESSENGER_MSG_THREADS_SNAPSHOT_UPDATED_HPP
#pragma once

#include <cstdint>
#include <memory>
#include "global/core/messenger/msg.hpp"
#include "utility/pattern/fourcc.hpp"

namespace engine
{
	
    namespace messenger
    {

        class msg_threads_snapshot_updated_t
		{

		public:

			static constexpr bool is_queue_async = false;
			static constexpr bool is_instance_async = true;
			static constexpr bool keep_history = false;

        private:

        };

    }

}

#endif