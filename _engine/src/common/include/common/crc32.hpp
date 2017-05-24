#ifndef ENGINE_COMMON_CRC32_HPP
#define ENGINE_COMMON_CRC32_HPP
#pragma once

#include <cinttypes>
#include <cstddef>
#include "ustring.hpp"

namespace engine
{
	/**
	* @brief Helper class for @c CRC32 calculations
	*/
	class crc32_t final
	{

	public:

		/**
		* @brief Calculates @c CRC32 of given buffer
		*
		* @note This is the only place where we use raw pointer-to-void and length to indicate buffer. This is nasty, I know it, but sometimes you need to use right tool for the job.
		*
		* @param[in] buffer Any type of buffer to calculate @c CRC32 of
		* @param[in] length Length of data in @c buffer. @c buffer has to be AT LEAST the size of @c length, in bytes
		* @return @c CRC32 of the given buffer
		* @see calculate_from_string
		*/
		uint32_t calculate(void * buffer, std::size_t length);

		/**
		* @brief Calculates @c CRC32 of given @c UTF-8 or @c ASCII string
		*
		* @param[in] str @c UTF-8 or @c ASCII string
		* @return @c CRC32 of the given string
		* @see calculate
		*/
		uint32_t calculate_from_string(const char * str);
	
		/**
		* @brief Calculates @c CRC32 of given ustring_t
		*
		* @param[in] str ustring_t for calculation
		* @return @c CRC32 of the given string
		* @see calculate
		*/
		inline uint32_t calculate_from_string(const ustring_t & str)
		{
			calculate_from_string(str.get_cstring());
		}

	};
}

#endif