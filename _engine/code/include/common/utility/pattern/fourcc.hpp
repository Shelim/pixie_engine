#ifndef ENGINE_COMMON_UTILITY_PATTERN_FOURCC_HPP
#define ENGINE_COMMON_UTILITY_PATTERN_FOURCC_HPP
#pragma once

#include <cstdint>

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

	template<uint32_t val1, uint32_t val2, uint32_t val3, uint32_t val4> class make_id_t
	{

	public:

		static const uint32_t value = (((val1 & 0xff) << 24) | ((val2 & 0xff) << 16) | ((val3 & 0xff) << 8) | (val4 & 0xff));

	private:

		make_id_t()
		{

		}

	};
}

#endif
