#ifndef ENGINE_COMMON_UTILITY_MESSENGER_MSG_KILL_MESSANGER_HPP
#define ENGINE_COMMON_UTILITY_MESSENGER_MSG_KILL_MESSANGER_HPP
#pragma once

#include <cstdint>
#include "utility/messenger/msg_base.hpp"

namespace engine
{

	class msg_kill_messanger_t : public msg_base_t
	{

	public:

		static const id_t type;
		
		msg_kill_messanger_t(): msg_base_t(type)
		{

		}


	};

}

#endif