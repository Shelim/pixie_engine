#ifndef ENGINE_COMMON_UTILITY_PATTERN_FOURCC_HPP
#define ENGINE_COMMON_UTILITY_PATTERN_FOURCC_HPP
#pragma once

#include <cstdint>
#include <algorithm>

namespace engine
{
	class id_t
	{

	public:

		typedef uint32_t underlying_type;

		id_t(const underlying_type value) : value(value)
		{

		}

		id_t(const id_t & other) : value(other.value)
		{

		}

		operator underlying_type() const { return value; }

		id_t & operator= (const id_t & other)
		{
			value = other.value;
			return (*this);
		}
		id_t & operator= (id_t && other)
		{
			std::swap(value, other.value);
			return (*this);
		}

	private:

		underlying_type value;

	};

	constexpr uint32_t make_id(const char (&str)[5]) // one byte for null!
	{
		return (static_cast<uint32_t>(str[0]) << 24) | (static_cast<uint32_t>(str[1]) << 16) | (static_cast<uint32_t>(str[2]) << 8) | (static_cast<uint32_t>(str[3]) << 0);
	}

}

#endif
