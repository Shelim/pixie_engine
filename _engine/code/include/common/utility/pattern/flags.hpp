#ifndef ENGINE_COMMON_UTILITY_PATTERN_FLAGS_HPP
#define ENGINE_COMMON_UTILITY_PATTERN_FLAGS_HPP
#pragma once

#include <unordered_map>
#include <bitset>
#include <type_traits>
#include "utility/pattern/enum.hpp"

namespace engine
{
	template<class flag_t> class flags_t final
	{

	public:

		void set_flag(flag_t flag, bool val = true)
		{
			flags.set(to_value(flag), val);
		}

		bool is_flag(flag_t flag) const
		{
			return flags.test(to_value(flag));
		}

		void clear_all()
		{
			flags.reset();
		}

	private:

		std::bitset<to_value(flag_t::count)> flags;
	};

}

#endif
