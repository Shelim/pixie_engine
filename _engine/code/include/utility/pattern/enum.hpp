#ifndef ENGINE_COMMON_UTILITY_PATTERN_ENUM_HPP
#define ENGINE_COMMON_UTILITY_PATTERN_ENUM_HPP
#pragma once

#include <unordered_map>
#include <bitset>
#include <type_traits>

namespace engine
{

	/**
	 * @brief Converts any strongly-typed enum value into its underlying type
	 * 
	 * @tparam enum_t Enum type
	 * @param[in] val Enum value
	 * @return Value converted to underlying type 
	 */
	template<class enum_t> constexpr auto value_of(enum_t val) -> typename std::underlying_type<enum_t>::type
	{
		return static_cast<typename std::underlying_type<enum_t>::type>(val);
	}

}

#endif
