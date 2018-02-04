#ifndef ENGINE_COMMON_UTILITY_DATA_RESULTS_HPP
#define ENGINE_COMMON_UTILITY_DATA_RESULTS_HPP
#pragma once

#include "global/core/messenger/messenger.hpp"
#include "global/core/data/state.hpp"
#include "global/core/data/provider.hpp"
#include "global/core/messenger/msg_scanner_found_item.hpp"
#include <vector>
#include <memory>

namespace engine
{
	namespace data
	{

		class results_t final
		{

		public:

			results_t(state_t * state, std::shared_ptr<messenger_t> messenger) : state(state), messenger(messenger)
			{

			}

			void begin_scanning()
			{
				state->clear();
			}

			void add_result(std::unique_ptr<provider_actual_t> result)
			{
				msg_found_item.set(this, result.get());
				messenger->post_message_sync(&msg_found_item);
				if(msg_found_item.is_accepted())
					state->add_provider_actual(std::move(result));
			}

			void end_scanning()
			{
				state->calculate_directories();
			}

		private:

			msg_scanner_found_item_t msg_found_item;
			state_t * state;
			std::shared_ptr<messenger_t> messenger;
		};
	}

}

#endif