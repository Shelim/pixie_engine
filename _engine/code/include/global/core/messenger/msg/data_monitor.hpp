#ifndef ENGINE_COMMON_UTILITY_MESSENGER_MSG_DATA_MONITOR_HPP
#define ENGINE_COMMON_UTILITY_MESSENGER_MSG_DATA_MONITOR_HPP
#pragma once

#include <cstdint>
#include <memory>
#include "global/core/messenger/msg.hpp"
#include "utility/pattern/fourcc.hpp"
#include "global/core/data/scanner.hpp"

namespace engine
{
	
    namespace messenger
    {

        class msg_data_monitor_t
		{

		public:

			static constexpr bool is_queue_async = false;
			static constexpr bool is_instance_async = false;
			static constexpr bool keep_history = false;


            msg_data_monitor_t(engine::data::scanners_t && scanners) : scanners(std::move(scanners))
            {

            }

            engine::data::scanners_t && extract_scanners()
            {
                return std::move(scanners);
            }

        private:
        
            engine::data::scanners_t scanners;

        };

    }

}

#endif