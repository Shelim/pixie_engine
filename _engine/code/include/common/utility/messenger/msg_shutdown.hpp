#ifndef ENGINE_COMMON_UTILITY_MESSENGER_MSG_SHUTDOWN_HPP
#define ENGINE_COMMON_UTILITY_MESSENGER_MSG_SHUTDOWN_HPP
#pragma once

#include <cstdint>
#include "utility/messenger/msg_base.hpp"
#include "utility/pattern/fourcc.hpp"
#include "utility/pattern/flags.hpp"

namespace engine
{

	class msg_shutdown_t : public msg_base_t
	{

	public:

		static const id_t type;

		enum class flag_t
		{
			force,
			display_logger_raports,
			generate_crashdump,
			generate_callstack,
			count
		};

		msg_shutdown_t(std::initializer_list<flag_t> list) : msg_base_t(type)
		{
			for (auto flag : list)
			{
				flags.set_flag(flag, true);
			}
		}

		bool is_forced()
		{
			return flags.is_flag(flag_t::force);
		}

		bool is_display_logger_raports_requested()
		{
			return flags.is_flag(flag_t::display_logger_raports);
		}

		bool is_crashdump_requested()
		{
			return flags.is_flag(flag_t::generate_crashdump);
		}

		bool is_callstack_requested()
		{
			return flags.is_flag(flag_t::generate_callstack);
		}

	private:

		flags_t<flag_t> flags;

	};

}

#endif