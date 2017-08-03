#ifndef ENGINE_COMMON_UTILITY_PATTERN_FOURCC_HPP
#define ENGINE_COMMON_UTILITY_PATTERN_FOURCC_HPP
#pragma once

#include <cstdint>

namespace engine
{
	class id_t
	{

	public:

		id_t(const uint32_t value) : value(value)
		{

		}

		operator uint32_t() const { return value; }

	private:

		const uint32_t value;

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
