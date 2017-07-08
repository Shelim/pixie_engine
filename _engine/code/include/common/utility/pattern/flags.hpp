#ifndef ENGINE_COMMON_UTILITY_PATTERN_FLAGS_HPP
#define ENGINE_COMMON_UTILITY_PATTERN_FLAGS_HPP
#pragma once

#include <unordered_map>
#include <bitset>
#include <type_traits>

namespace engine
{
	template<class flag_t> class flags_t final
	{

	private:

		typedef typename std::underlying_type<flag_t>::type utype_t;

	public:

		void set_flag(flag_t flag, bool value = true)
		{
			flags.set(static_cast<utype_t>(flag), value);
		}

		bool is_flag(flag_t flag) const
		{
			return flags.test(static_cast<utype_t>(flag));
		}

		void clear_all()
		{
			flags.reset();
		}

	private:

		std::bitset<static_cast<utype_t>(flag_t::count)> flags;
	};

}

#endif