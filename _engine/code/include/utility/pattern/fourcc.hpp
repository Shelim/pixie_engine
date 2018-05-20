#ifndef ENGINE_COMMON_UTILITY_PATTERN_FOURCC_HPP
#define ENGINE_COMMON_UTILITY_PATTERN_FOURCC_HPP
#pragma once

#include <cstdint>
#include <algorithm>

namespace engine
{

	/**
	 * \~English @brief A (very) lightweight unique resource identifier
	 * 
	 * This class is 4-bytes long identifier directly convertable to ustring_t (4 ASCII characters).
	 * 
	 * @note This class is endianess-aware (therefore you cannot use character literal trick `'abcd'`, because they do not give strong guarantee of endianess)
	 * 
	 * Internally this class contains only single uint32_t field and has implicit conversion to and from uint32_t
	 * 
	 * To define id_t instance from four characters-length string use @ref make_id() which is defined as constexpr (so it can for example be used in switch label)
	 * 
	 * @code{.cpp}
	 * id_t foo = make_id("mfoo");
	 * switch(foo)
	 * {
	 *    case make_id("mfoo"):
	 *      // outputs "The ID is mfoo!"
	 *      format_string("The ID is #1#!", foo);
	 *    break;
	 * }
	 * @endcode
	 */
	class id_t
	{

	public:

		typedef uint32_t underlying_type;

		/**
		 * \~English @brief Constructs id_t from uint32_t. This is implicit operation
		 * 
		 * @param[in] value Source value
		 */
		id_t(const underlying_type value) : value(value)
		{

		}

		/**
		 * \~English @brief Copy-construct id_t from other id_t
		 * 
		 * @param[in] other Source id_t
		 */
		id_t(const id_t & other) : value(other.value)
		{

		}

		/**
		 * \~English @brief Converts id_t to uint32_t. This is implicit operation
		 * 
		 * @return value of this id_t
		 */
		operator underlying_type() const { return value; }

		/**
		 * \~English @brief Copy-assigns other id_t to this id_t
		 * 
		 * @param[in] other id_t
		 * @return this (for chaining)
		 */
		id_t & operator= (const id_t & other)
		{
			value = other.value;
			return (*this);
		}
		/**
		 * \~English @brief Move-assigns other id_t to this id_t
		 * 
		 * @param[in] other id_t
		 * @return this (for chaining)
		 */
		id_t & operator= (id_t && other)
		{
			std::swap(value, other.value);
			return (*this);
		}

	private:

		underlying_type value;

	};

	/**
	 * \~English @brief Creates id_t compatible uint32_t (id_t is directly and implicitly constructable from uint32_t)
	 * 
	 * @note This function is preferable to literal trick `'abcd'`, because they the latter does not give strong guarantee of endianess
	 * 
	 * @note This function is defined as constexpr, therefore it can be used inside switch labels
	 * 
	 * @code{.cpp}
	 * id_t foo = make_id("mfoo");
	 * switch(foo)
	 * {
	 *    case make_id("mfoo"):
	 *      // outputs "The ID is mfoo!"
	 *      format_string("The ID is #1#!", foo);
	 *    break;
	 * }
	 * @endcode
	 * 
	 * @param[in] str 4-character long text that will be converted to id_t (5th character is @c \\0 byte mark automatically added by compiler)
	 * @return id_t compatible uint32_t
	 * @see id_t
	 */
	constexpr uint32_t make_id(const char (&str)[5]) // one byte for null!
	{
		return (static_cast<uint32_t>(str[0]) << 24) | (static_cast<uint32_t>(str[1]) << 16) | (static_cast<uint32_t>(str[2]) << 8) | (static_cast<uint32_t>(str[3]) << 0);
	}

}

#endif
