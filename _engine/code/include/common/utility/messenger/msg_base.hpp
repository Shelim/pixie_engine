#ifndef ENGINE_COMMON_UTILITY_MESSENGER_MSG_BASE_HPP
#define ENGINE_COMMON_UTILITY_MESSENGER_MSG_BASE_HPP
#pragma once

#include <cstdint>

namespace engine
{

	class msg_base_t
	{

	public:

		virtual ~msg_base_t()
		{

		}

		typedef uint32_t type_t;

		type_t get_type() const
		{
			return type;
		}

	protected:

		msg_base_t(type_t type) : type(type)
		{

		}

	private:

		type_t type;

	};

}

#endif