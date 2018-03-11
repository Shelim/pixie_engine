#ifndef ENGINE_COMMON_UTILITY_TEXT_USTRING_HPP
#define ENGINE_COMMON_UTILITY_TEXT_USTRING_HPP
#pragma once

#include <string>
#include <vector>
#include <cstdio>
#include <cstring>
#include <cstdarg>
#include <cinttypes>
#include <thread>
#include <limits>
#include <array>
#include <chrono>
#include <functional>
#include <sstream>
#include <iomanip>
#include <utility>
#include <iterator>
#include <codecvt>
#include <cereal/access.hpp>
#include "global/core/vfs/filesystem.hpp"
#include "utility/renderer/color.hpp"
#include "utility/pattern/fourcc.hpp"

namespace engine
{
	class ustring_t;
}

engine::ustring_t operator""_u(const char * str, std::size_t);

namespace engine
{
	/**
	 * @brief Unicode Symbol (32-bit unsigned value)
	 */
	typedef int_least32_t usymbol_t;

	/**
	* @brief Converts usymbol_t into uppercaseR
	*
	* It will first try to use platform specific library, failing to simple @c ASCII replacement if none are present
	*
	* @param[in] item usymbol_t to uppercasing
	* @return Uppercased usymbol_t
	* @see to_lower
	*/
	usymbol_t to_upper(usymbol_t item);

	/**
	* @brief Converts usymbol_t into lowercase
	*
	* It will first try to use platform specific library, failing to simple @c ASCII replacement if none are present
	*
	* @param[in] item usymbol_t to lowercasing
	* @return Lowercased usymbol_t
	* @see to_upper
	*/
	usymbol_t to_lower(usymbol_t item);

	/**
	* @brief Check if given usymbol_t contains whitespace
	*
	* It will first try to use platform specific library, failing to simple @c ASCII check if none are present
	*
	* @param[in] item usymbol_t to check
	* @return true if usymbol_t contains whitespace
	*/
	bool is_whitespace_ascii(char item);

	/**
	* @brief Check if given usymbol_t contains path separator
	*
	* @remark This function MAY BE platform depedent!
	*
	* @param[in] item usymbol_t to check
	* @return true if usymbol_t contains path separator
	*/
	bool is_path_separator_ascii(char ch);

	/**
	* @brief Check if given usymbol_t contains number
	*
	* @remark This function is platform independeant
	*
	* @param[in] item usymbol_t to check
	* @return true if usymbol_t contains number
	*/
	bool is_numeric(char ch);

	/**
	* @brief Check if given usymbol_t contains ASCII character (a-z or A-Z)
	*
	* @remark This function is platform independeant
	*
	* @param[in] item usymbol_t to check
	* @return true if usymbol_t contains ASCII character (a-z or A-Z)
	*/
	bool is_character_ascii(char ch);

	/**
	 * @brief Class for handling @c UTF-8 string
	 */
	class ustring_t final
	{

	public:

		friend class iterator_t;

		/**
		 * @brief Class for iterating @c UTF-8 string
		 * 
		 * @see ustring_t
		 */
		class iterator_t
		{

		public:

			typedef ptrdiff_t difference_type;
			typedef usymbol_t value_type;
			typedef const usymbol_t & reference;
			typedef const usymbol_t * pointer;
			typedef std::forward_iterator_tag iterator_category;

			/**
			 * @brief Constuct iterator from other iterator
			 */
			iterator_t(const iterator_t& other) : str(other.str), decoded_symbol(other.decoded_symbol), pos(other.pos)
			{

			}

			/**
			 * @brief Destructor
			 */
			~iterator_t()
			{

			}

			/**
			 * @brief Copy-assign iterator from other iterator
			 * 
			 * @param[in] other iterator to copy
			 */
			iterator_t& operator=(const iterator_t& other)
			{
				str = other.str;
				decoded_symbol = other.decoded_symbol;
				pos = other.pos;

				return (*this);
			}

			/**
			 * @brief Go to the next usymbol_t in @c UTF-8 string
			 */
			iterator_t& operator++()
			{
				pos += _symbol_size(pos);

				const char * p2 = pos;
				decoded_symbol = str->_decode(p2);

				return (*this);
			}

			/**
			 * @brief Get current usymbol_t in @c UTF-8 string
			 */
			reference operator*() const
			{
				return decoded_symbol;
			}

			/**
			 * @brief Swap two iterators
			 * 
			 * @param[in] other iterator to swap
			 */
			void swap(iterator_t & other)
			{
				std::swap(str, other.str);
				std::swap(decoded_symbol, other.decoded_symbol);
				std::swap(pos, other.pos);
			}

			/**
			 * @brief Go to the next usymbol_t in @c UTF-8 string
			 */
			iterator_t operator++(int)
			{
				iterator_t ret = *this;
				++(*this);

				return ret;
			}

			/**
			 * @brief Get current usymbol_t in @c UTF-8 string
			 */
			value_type operator*()
			{
				return decoded_symbol;
			}

			/**
			 * @brief Get current usymbol_t in @c UTF-8 string
			 */
			pointer operator->() const
			{
				return &decoded_symbol;
			}

			/**
			 * @brief Compare two iterators
			 * 
			 * @param[in] other iterator to compare
			 * @return true for equality, false otherwise
			 */
			inline bool equals(const iterator_t& other) const
			{
				return pos == other.pos;
			}

			friend class ustring_t;

		private:

			iterator_t(const ustring_t * str) : str(str)
			{
				pos = str->get_cstring();

				const char * p2 = pos;
				decoded_symbol = str->_decode(p2);
			}

			struct end_t
			{
			
			};

			iterator_t(const ustring_t * str, end_t) : str(str)
			{
				pos = str->get_cstring();
				pos += str->_str.length();

				decoded_symbol = 0;
			}

			const ustring_t * str;
			usymbol_t decoded_symbol;
			const char * pos;
		};
		
		typedef iterator_t const_iterator_t;

		friend struct std::hash<engine::ustring_t>;

		//////////////////////////////////////////////////////////////////////////

		/**
		 * @brief Converts @c ASCII string into ustring_t
		 *
		 * @param[in] str @c ASCII string to convert
		 * @return Converted ustring_t
		 */
		static ustring_t from_ascii(const char * str);
		/**
		 * @brief Converts @c UTF-8 string into ustring_t
		 *
		 * @param[in] str @c UTF-8 string to convert
		 * @return Converted ustring_t
		 */
		static ustring_t from_utf8(const char * str);
		/**
		 * @brief Converts @c Wide string into ustring_t
		 *
		 * @param[in] str @c Wide string to convert
		 * @return Converted ustring_t
		 */
		static ustring_t from_wide(const wchar_t * str);
		/**
		* @brief Converts @c usymbol_t literal into (1-element) ustring_t
		*
		* @param[in] str @c usymbol_t literal to convert
		* @return Converted ustring_t
		*/
		static ustring_t from_symbol(usymbol_t str);

		//////////////////////////////////////////////////////////////////////////

		/**
		 * @brief Get iterator to first usymbol_t in given @c UTF-8 string
		 * 
		 * @return iterator to first usymbol_t
		 */
		iterator_t begin()
		{
			return iterator_t(this);
		}

		/**
		 * @brief Get iterator to one-past-end usymbol_t in given @c UTF-8 string
		 * 
		 * @return iterator to one-past-end usymbol_t
		 */
		iterator_t end()
		{
			return iterator_t(this, iterator_t::end_t{});
		}

		/**
		 * @brief Get constant iterator to first usymbol_t in given @c UTF-8 string
		 * 
		 * @return constant iterator to first usymbol_t
		 */
		const_iterator_t begin() const
		{
			return iterator_t(this);
		}

		/**
		 * @brief Get constant iterator to one-past-end usymbol_t in given @c UTF-8 string
		 * 
		 * @return constant iterator to one-past-end usymbol_t
		 */
		const_iterator_t end() const
		{
			return iterator_t(this, iterator_t::end_t{});
		}

		//////////////////////////////////////////////////////////////////////////

		/**
		* @brief Removes any whitespace (only @c ASCII) from ustring_t
		*
		* @note On some platform this can actually remove @c UTF-8 whitespace as well
		*
		* @return Trimmed ustring_t
		* @see left_trim, right_trim
		*/
		ustring_t trim() const
		{
			return trim_string(*this);
		}

		/**
		 * @brief Removes any whitespace (only @c ASCII) from ustring_t
		 *
		 * @note On some platform this can actually remove @c UTF-8 whitespace as well
		 *
		 * @param[in] str ustring_t for trimming
		 * @return Trimmed ustring_t
		 * @see left_trim, right_trim
		 */
		static ustring_t trim_string(const ustring_t & str);
		/**
		 * @brief Removes any whitespace (only @c ASCII) from ustring_t
		 *
		 * This converts first argument into ustring_t, then passes execution into ustring_t::trim
		 *
		 * @note On some platform this can actually remove @c UTF-8 whitespace as well
		 *
		 * @param[in] str @c ASCII string for trimming
		 * @return Trimmed ustring_t
		 * @see trim, left_trim, right_trim
		 */
		static ustring_t trim_ascii(const char * str)
		{
			return trim_string(from_ascii(str));
		}
		/**
		 * @brief Removes any whitespace (only @c ASCII) from ustring_t
		 *
		 * This converts first argument into ustring_t, then passes execution into ustring_t::trim
		 *
		 * @note On some platform this can actually remove @c UTF-8 whitespace as well
		 *
		 * @param[in] str @c UTF-8 string for trimming
		 * @return Trimmed ustring_t
		 * @see trim, left_trim, right_trim
		 */
		static ustring_t trim_utf8(const char * str)
		{
			return trim_string(from_utf8(str));
		}
		/**
		 * @brief Removes any whitespace (only @c ASCII) from ustring_t
		 *
		 * This converts first argument into ustring_t, then passes execution into ustring_t::trim
		 *
		 * @note On some platform this can actually remove @c UTF-8 whitespace as well
		 *
		 * @param[in] str @c Wide string for trimming
		 * @return Trimmed ustring_t
		 * @see trim, left_trim, right_trim
		 */
		static ustring_t trim_wide(const wchar_t * str)
		{
			return trim_string(from_wide(str));
		}

		//////////////////////////////////////////////////////////////////////////

		/**
		* @brief Removes any whitespace (only @c ASCII) from left side of ustring_t
		*
		* @note On some platform this can actually remove @c UTF-8 whitespace as well
		*
		* @return Trimmed ustring_t
		* @see trim, right_trim
		*/
		ustring_t left_trim() const
		{
			return left_trim_string(*this);
		}

		/**
		 * @brief Removes any whitespace (only @c ASCII) from left side of ustring_t
		 *
		 * @note On some platform this can actually remove @c UTF-8 whitespace as well
		 *
		 * @param[in] str ustring_t for trimming
		 * @return Trimmed ustring_t
		 * @see trim, right_trim
		 */
		static ustring_t left_trim_string(const ustring_t & str);
		/**
		 * @brief Removes any whitespace (only @c ASCII) from left side of ustring_t
		 *
		 * This converts first argument into ustring_t, then passes execution into ustring_t::left_trim
		 *
		 * @note On some platform this can actually remove @c UTF-8 whitespace as well
		 *
		 * @param[in] str @c ASCII string for trimming
		 * @return Trimmed ustring_t
		 * @see trim, left_trim, right_trim
		 */
		static ustring_t left_trim_ascii(const char * str)
		{
			return left_trim_string(from_ascii(str));
		}
		/**
		 * @brief Removes any whitespace (only @c ASCII) from left side of ustring_t
		 *
		 * This converts first argument into ustring_t, then passes execution into ustring_t::left_trim
		 *
		 * @note On some platform this can actually remove @c UTF-8 whitespace as well
		 *
		 * @param[in] str @c UTF-8 string for trimming
		 * @return Trimmed ustring_t
		 * @see trim, left_trim, right_trim
		 */
		static ustring_t left_trim_utf8(const char * str)
		{
			return left_trim_string(from_utf8(str));
		}
		/**
		 * @brief Removes any whitespace (only @c ASCII) from left side of ustring_t
		 *
		 * This converts first argument into ustring_t, then passes execution into ustring_t::left_trim
		 *
		 * @note On some platform this can actually remove @c UTF-8 whitespace as well
		 *
		 * @param[in] str @c Wide string for trimming
		 * @return Trimmed ustring_t
		 * @see trim, left_trim, right_trim
		 */
		static ustring_t left_trim_wide(const wchar_t * str)
		{
			return left_trim_string(from_wide(str));
		}

		//////////////////////////////////////////////////////////////////////////

		/**
		* @brief Removes any whitespace (only @c ASCII) from right side of ustring_t
		*
		* @note On some platform this can actually remove @c UTF-8 whitespace as well
		*
		* @return Trimmed ustring_t
		* @see trim, left_trim
		*/
		ustring_t right_trim() const
		{
			return right_trim_string(*this);
		}

		/**
		 * @brief Removes any whitespace (only @c ASCII) from right side of ustring_t
		 *
		 * @note On some platform this can actually remove @c UTF-8 whitespace as well
		 *
		 * @param[in] str ustring_t for trimming
		 * @return Trimmed ustring_t
		 * @see trim, left_trim
		 */
		static ustring_t right_trim_string(const ustring_t & str);
		/**
		 * @brief Removes any whitespace (only @c ASCII) from right side of ustring_t
		 *
		 * This converts first argument into ustring_t, then passes execution into ustring_t::right_trim
		 *
		 * @note On some platform this can actually remove @c UTF-8 whitespace as well
		 *
		 * @param[in] str @c ASCII string for trimming
		 * @return Trimmed ustring_t
		 * @see trim, left_trim, right_trim
		 */
		static ustring_t right_trim_ascii(const char * str)
		{
			return right_trim_string(from_ascii(str));
		}
		/**
		 * @brief Removes any whitespace (only @c ASCII) from right side of ustring_t
		 *
		 * This converts first argument into ustring_t, then passes execution into ustring_t::right_trim
		 *
		 * @note On some platform this can actually remove @c UTF-8 whitespace as well
		 *
		 * @param[in] str @c UTF-8 string for trimming
		 * @return Trimmed ustring_t
		 * @see trim, left_trim, right_trim
		 */
		static ustring_t right_trim_utf8(const char * str)
		{
			return right_trim_string(from_utf8(str));
		}
		/**
		 * @brief Removes any whitespace (only @c ASCII) from right side of ustring_t
		 *
		 * This converts first argument into ustring_t, then passes execution into ustring_t::right_trim
		 *
		 * @note On some platform this can actually remove @c UTF-8 whitespace as well
		 *
		 * @param[in] str @c Wide string for trimming
		 * @return Trimmed ustring_t
		 * @see trim, left_trim, right_trim
		 */
		static ustring_t right_trim_wide(const wchar_t * str)
		{
			return right_trim_string(from_wide(str));
		}

		//////////////////////////////////////////////////////////////////////////

		/**
		* @brief Converts ustring_t into uppercase
		*
		* It will first try to use platform specific library, failing to simple @c ASCII replacement if none are present
		*
		* @return Uppercased ustring_t
		* @see to_lower
		*/
		ustring_t to_upper() const
		{
			return to_upper_string(*this);
		}

		/**
		 * @brief Converts ustring_t into uppercase
		 *
		 * It will first try to use platform specific library, failing to simple @c ASCII replacement if none are present
		 *
		 * @param[in] str ustring_t for uppercasing
		 * @return Uppercased ustring_t
		 * @see to_lower
		 */
		static ustring_t to_upper_string(const ustring_t & str);
		/**
		 * @brief Converts ustring_t into uppercase
		 *
		 * It will first try to use platform specific library, failing to simple @c ASCII replacement if none are present
		 * This converts first argument into ustring_t, then passes execution into ustring_t::to_upper
		 *
		 * @param[in] str @c ASCII string for uppercasing
		 * @return Uppercased ustring_t
		 * @see to_upper, to_lower
		 */
		static ustring_t to_upper_ascii(const char * str)
		{
			return to_upper_string(from_ascii(str));
		}
		/**
		 * @brief Converts ustring_t into uppercase
		 *
		 * It will first try to use platform specific library, failing to simple @c ASCII replacement if none are present
		 * This converts first argument into ustring_t, then passes execution into ustring_t::to_upper
		 *
		 * @param[in] str @c UTF-8 string for uppercasing
		 * @return Uppercased ustring_t
		 * @see to_upper, to_lower
		 */
		static ustring_t to_upper_utf8(const char * str)
		{
			return to_upper_string(from_utf8(str));
		}
		/**
		 * @brief Converts ustring_t into uppercase
		 *
		 * It will first try to use platform specific library, failing to simple @c ASCII replacement if none are present
		 * This converts first argument into ustring_t, then passes execution into ustring_t::to_upper
		 *
		 * @param[in] str @c WIDE string for uppercasing
		 * @return Uppercased ustring_t
		 * @see to_upper, to_lower
		 */
		static ustring_t to_upper_wide(const wchar_t * str)
		{
			return to_upper_string(from_wide(str));
		}

		//////////////////////////////////////////////////////////////////////////

		/**
		* @brief Converts ustring_t into lowercase
		*
		* It will first try to use platform specific library, failing to simple @c ASCII replacement if none are present
		*
		* @return Lowercased ustring_t
		* @see to_upper
		*/
		ustring_t to_lower() const
		{
			return to_lower_string(*this);
		}

		/**
		 * @brief Converts ustring_t into lowercase
		 *
		 * It will first try to use platform specific library, failing to simple @c ASCII replacement if none are present
		 *
		 * @param[in] str ustring_t for lowercasing
		 * @return Lowercased ustring_t
		 * @see to_upper
		 */
		static ustring_t to_lower_string(const ustring_t & str);
		/**
		 * @brief Converts ustring_t into lowercase
		 *
		 * It will first try to use platform specific library, failing to simple @c ASCII replacement if none are present
		 * This converts first argument into ustring_t, then passes execution into ustring_t::to_lower
		 *
		 * @param[in] str @c ASCII string for lowercasing
		 * @return Lowercased ustring_t
		 * @see to_upper, to_lower
		 */
		static ustring_t to_lower_ascii(const char * str)
		{
			return to_lower_string(from_ascii(str));
		}
		/**
		 * @brief Converts ustring_t into lowercase
		 *
		 * It will first try to use platform specific library, failing to simple @c ASCII replacement if none are present
		 * This converts first argument into ustring_t, then passes execution into ustring_t::to_lower
		 *
		 * @param[in] str @c UTF-8 string for lowercasing
		 * @return Lowercased ustring_t
		 * @see to_upper, to_lower
		 */
		static ustring_t to_lower_utf8(const char * str)
		{
			return to_lower_string(from_utf8(str));
		}
		/**
		 * @brief Converts ustring_t into lowercase
		 *
		 * It will first try to use platform specific library, failing to simple @c ASCII replacement if none are present
		 * This converts first argument into ustring_t, then passes execution into ustring_t::to_lower
		 *
		 * @param[in] str @c Wide string for lowercasing
		 * @return Lowercased ustring_t
		 * @see to_upper, to_lower
		 */
		static ustring_t to_lower_wide(const wchar_t * str)
		{
			return to_lower_string(from_wide(str));
		}

		//////////////////////////////////////////////////////////////////////////

		/**
		* @brief Replaces ALL occurence of from into to
		*
		* Returns changed string. This string remains unaffected
		*
		* @param[in] from ustring_t what to 
		* @param[in] to ustring_t for what
		* @return replaced string
		*/
		ustring_t replace(const ustring_t & from, const ustring_t & to) const;

		//////////////////////////////////////////////////////////////////////////

		/**
		* @brief Appends given symbol to the end of this string
		*
		* @param[in] symbol usymbol_t to append
		*/
		ustring_t & append(usymbol_t symbol)
		{
			_encode(symbol);
			return *this;
		}

		/**
		* @brief Appends given string to the end of this string
		*
		* Appends given string to the end of this string. No conversion will be made
		*
		* @param[in] str ustring_t to append
		* @return this object, so you can chain appending
		*/
		ustring_t & append(const ustring_t & str);

		/**
		* @brief Appends given string to the end of this string
		*
		* Appends given string to the end of this string.
		*
		* @param[in] str @c ASCII string to append
		* @return this object, so you can chain appending
		*/
		ustring_t & append_ascii(const char * str)
		{
			return append(from_ascii(str));
		}

		/**
		* @brief Appends given string to the end of this string
		*
		* Appends given string to the end of this string.
		*
		* @param[in] str @c UTF-8 string to append
		* @return this object, so you can chain appending
		*/
		ustring_t & append_utf8(const char * str)
		{
			return append(from_utf8(str));
		}

		/**
		* @brief Appends given string to the end of this string
		*
		* Appends given string to the end of this string.
		*
		* @param[in] str @c Wide string to append
		* @return this object, so you can chain appending
		*/
		ustring_t & append_wide(const wchar_t * str)
		{
			return append(from_wide(str));
		}

		//////////////////////////////////////////////////////////////////////////

		/**
		* @brief Return part of given ustring_t
		*
		* Returns the portion of string ranging from @c pos parameter till string end. @c pos argument can be negative, position will be then calculated based on the string ending
		* @note this function is safe for all data, ie. calling it with @c pos > ustring_t::len is perfectly defined and will return emtpy string
		*
		* @param[in] pos starting position in string (0 is the first character). Can be negative
		* @return Portion of source ustring_t
		*/
		ustring_t substr(int_fast32_t pos) const
		{
			return substr_string(*this, pos);
		}
		/**
		* @brief Return part of given ustring_t
		*
		* Returns the portion of string ranging from @c pos parameter for @c len characters. @c pos argument can be negative, position will be then calculated based on the string ending. @c len argument can be also negative, length will be then calculated based on the string ending
		* @note this function is safe for all data, ie. calling it with @c pos > ustring_t::len is perfectly defined and will return emtpy string
		*
		* @param[in] pos starting position in string (0 is the first character). Can be negative
		* @param[in] len length of the substring. Can be negative
		* @return Portion of source ustring_t
		*/
		ustring_t substr(int_fast32_t pos, int_fast32_t len) const
		{
			return substr_string(*this, pos, len);
		}

		/**
		 * @brief Return part of given ustring_t
		 *
		 * Returns the portion of string ranging from @c pos parameter till string end. @c pos argument can be negative, position will be then calculated based on the string ending
		 * @note this function is safe for all data, ie. calling it with @c pos > ustring_t::len is perfectly defined and will return emtpy string
		 *
		 * @param[in] str source ustring_t
		 * @param[in] pos starting position in string (0 is the first character). Can be negative
		 * @return Portion of source ustring_t
		 */
		static ustring_t substr_string(const ustring_t & str, int_fast32_t pos);
		/**
		 * @brief Return part of given ustring_t
		 *
		 * Returns the portion of string ranging from @c pos parameter for @c len characters. @c pos argument can be negative, position will be then calculated based on the string ending. @c len argument can be also negative, length will be then calculated based on the string ending
		 * @note this function is safe for all data, ie. calling it with @c pos > ustring_t::len is perfectly defined and will return emtpy string
		 *
		 * @param[in] str source ustring_t
		 * @param[in] pos starting position in string (0 is the first character). Can be negative
		 * @param[in] len length of the substring. Can be negative
		 * @return Portion of source ustring_t
		 */
		static ustring_t substr_string(const ustring_t & str, int_fast32_t pos, int_fast32_t len);
		/**
		 * @brief Return part of given ustring_t
		 *
		 * Returns the portion of string ranging from @c pos parameter till string end. @c pos argument can be negative, position will be then calculated based on the string ending
		 *
		 * This converts first argument into ustring_t, then passes execution into ustring_t::substr
		 *
		 * @note this function is safe for all data, ie. calling it with @c pos > ustring_t::len is perfectly defined and will return emtpy string
		 *
		 * @param[in] str source @c ASCII string
		 * @param[in] pos starting position in string (0 is the first character). Can be negative
		 * @return Portion of source ustring_t
		 * @see substr
		 */
		static ustring_t substr_ascii(const char * str, int_fast32_t pos)
		{
			return substr_string(from_ascii(str), pos);
		}
		/**
		 * @brief Return part of given ustring_t
		 *
		 * Returns the portion of string ranging from @c pos parameter for @c len characters. @c pos argument can be negative, position will be then calculated based on the string ending. @c len argument can be also negative, length will be then calculated based on the string ending
		 *
		 * This converts first argument into ustring_t, then passes execution into ustring_t::substr
		 *
		 * @note this function is safe for all data, ie. calling it with @c pos > ustring_t::len is perfectly defined and will return emtpy string
		 *
		 * @param[in] str source @c ASCII string
		 * @param[in] pos starting position in string (0 is the first character). Can be negative
		 * @param[in] len length of the substring. Can be negative
		 * @return Portion of source ustring_t
		 * @see substr
		 */
		static ustring_t substr_ascii(const char * str, int_fast32_t pos, int_fast32_t len)
		{
			return substr_string(from_ascii(str), pos, len);
		}
		/**
		 * @brief Return part of given ustring_t
		 *
		 * Returns the portion of string ranging from @c pos parameter till string end. @c pos argument can be negative, position will be then calculated based on the string ending
		 *
		 * This converts first argument into ustring_t, then passes execution into ustring_t::substr
		 *
		 * @note this function is safe for all data, ie. calling it with @c pos > ustring_t::len is perfectly defined and will return emtpy string
		 *
		 * @param[in] str source @c UTF-8 string
		 * @param[in] pos starting position in string (0 is the first character). Can be negative
		 * @return Portion of source ustring_t
		 * @see substr
		 */
		static ustring_t substr_utf8(const char * str, int_fast32_t pos)
		{
			return substr_string(from_utf8(str), pos);
		}
		/**
		 * @brief Return part of given ustring_t
		 *
		 * Returns the portion of string ranging from @c pos parameter for @c len characters. @c pos argument can be negative, position will be then calculated based on the string ending. @c len argument can be also negative, length will be then calculated based on the string ending
		 *
		 * This converts first argument into ustring_t, then passes execution into ustring_t::substr
		 *
		 * @note this function is safe for all data, ie. calling it with @c pos > ustring_t::len is perfectly defined and will return emtpy string
		 *
		 * @param[in] str source @c UTF-8 string
		 * @param[in] pos starting position in string (0 is the first character). Can be negative
		 * @param[in] len length of the substring. Can be negative
		 * @return Portion of source ustring_t
		 * @see substr
		 */
		static ustring_t substr_utf8(const char * str, int_fast32_t pos, int_fast32_t len)
		{
			return substr_string(from_utf8(str), pos, len);
		}
		/**
		 * @brief Return part of given ustring_t
		 *
		 * Returns the portion of string ranging from @c pos parameter till string end. @c pos argument can be negative, position will be then calculated based on the string ending
		 *
		 * This converts first argument into ustring_t, then passes execution into ustring_t::substr
		 *
		 * @note this function is safe for all data, ie. calling it with @c pos > ustring_t::len is perfectly defined and will return emtpy string
		 *
		 * @param[in] str source @c Wide string
		 * @param[in] pos starting position in string (0 is the first character). Can be negative
		 * @return Portion of source ustring_t
		 * @see substr
		 */
		static ustring_t substr_wide(const wchar_t * str, int_fast32_t pos)
		{
			return substr_string(from_wide(str), pos);
		}
		/**
		 * @brief Return part of given ustring_t
		 *
		 * Returns the portion of string ranging from @c pos parameter for @c len characters. @c pos argument can be negative, position will be then calculated based on the string ending. @c len argument can be also negative, length will be then calculated based on the string ending
		 *
		 * This converts first argument into ustring_t, then passes execution into ustring_t::substr
		 *
		 * @note this function is safe for all data, ie. calling it with @c pos > ustring_t::len is perfectly defined and will return emtpy string
		 *
		 * @param[in] str source @c Wide string
		 * @param[in] pos starting position in string (0 is the first character). Can be negative
		 * @param[in] len length of the substring. Can be negative
		 * @return Portion of source ustring_t
		 * @see substr
		 */
		static ustring_t substr_wide(const wchar_t * str, int_fast32_t pos, int_fast32_t len)
		{
			return substr_string(from_wide(str), pos, len);
		}

		//////////////////////////////////////////////////////////////////////////

		/**
		* @brief Returns filename from path given in str
		*
		* Gets filename from given path. If path ends with '/' then empty string will be returned
		*
		* @return ustring_t containing filename portion of path
		*/
		ustring_t basename() const
		{
			return basename_string(*this);
		}

		/**
		 * @brief Returns filename from path given in str
		 *
		 * Gets filename from given path. If path ends with '/' then empty string will be returned
		 *
		 * @param[in] str ustring_t containing path to file
		 * @return ustring_t containing filename portion of path
		 */
		static ustring_t basename_string(const ustring_t & str);
		/**
		 * @brief Returns filename from path given in str
		 *
		 * Gets filename from given path. If path ends with '/' then empty string will be returned
		 *
		 * This converts first argument into ustring_t, then passes execution into ustring_t::basename
		 *
		 * @param[in] str @c ASCII string containing path to file
		 * @return ustring_t containing filename portion of path
		 * @see basename
		 */
		static ustring_t basename_ascii(const char * str)
		{
			return basename_string(from_ascii(str));
		}
		/**
		 * @brief Returns filename from path given in str
		 *
		 * Gets filename from given path. If path ends with '/' then empty string will be returned
		 *
		 * This converts first argument into ustring_t, then passes execution into ustring_t::basename
		 *
		 * @param[in] str @c UTF-8 string containing path to file
		 * @return ustring_t containing filename portion of path
		 * @see basename
		 */
		static ustring_t basename_utf8(const char * str)
		{
			return basename_string(from_utf8(str));
		}
		/**
		 * @brief Returns filename from path given in str
		 *
		 * Gets filename from given path. If path ends with '/' then empty string will be returned
		 *
		 * This converts first argument into ustring_t, then passes execution into ustring_t::basename
		 *
		 * @param[in] str @c Wide string containing path to file
		 * @return ustring_t containing filename portion of path
		 * @see basename
		 */
		static ustring_t basename_wide(const wchar_t * str)
		{
			return basename_string(from_wide(str));
		}

		//////////////////////////////////////////////////////////////////////////

		/**
		 * @brief Initialize emtpy string
		 *
		 * Pretty fast method, should allocate no memory. Constructs empty UTF-8 string
		 */
		ustring_t();
		/**
		 * @brief Makes copy of string
		 *
		 * Makes full copy of @c other string into this instance of ustring_t (no string pooling!)
		 *
		 * @param[in] other Source ustring_t
		 */
		ustring_t(const ustring_t & other);
		/**
		* @brief Moves string
		*
		* Moves @c other string into this instance of ustring_t (no string pooling!)
		*
		* @param[in] other Source ustring_t
		*/
		ustring_t(ustring_t && other) noexcept;
		/**
		 * @brief Makes copy of string
		 *
		 * Makes full copy of @c other string into this instance of ustring_t (no string pooling!)
		 *
		 * @param[in] other Source ustring_t
		 */
		ustring_t & operator= (const ustring_t & other);
		/**
		* @brief Moves string
		*
		* Moves @c other string into this instance of ustring_t (no string pooling!)
		*
		* @param[in] other Source ustring_t
		*/
		ustring_t & operator= (ustring_t && other);
		/**
		 * @brief Frees internal data
		 *
		 * @note Any pointer obtained via ustring_t::get_cstring will not be valid after this!
		 */
		~ustring_t();

		//////////////////////////////////////////////////////////////////////////

		/**
		 * @brief Return length of ustring_t
		 *
		 * @return Length of ustring_t (in glyphs)
		 * @see is_empty
		 */
		uint_fast32_t len() const;
		/**
		 * @brief Check if ustring_t is empty
		 *
		 * @note This is usually faster then checking ustring_t::len()
		 *
		 * @return @c true, if ustring_t is empty, @c false otherwise
		 */
		bool is_empty() const;

		//////////////////////////////////////////////////////////////////////////

		/**
		* @brief Return first index of given usymbol_t in ustring_t
		*
		* Return first index of given usymbol_t in ustring_t, or -1 if is not found. Optional argument @c first tells how many chars from beginning have to be skipped
		*
		* @param[in] symbol usymbol_t usymbol_t to be found
		* @param[in] first How many chars from beginning have to be skipped
		* @return Position of `symbol` character in this ustring_t, or -1 if there is no
		* @see last_index_of
		*/
		int_fast32_t index_of(usymbol_t symbol, int_fast32_t first = 0) const;

		/**
		* @brief Return first index of given usymbol_t in ustring_t
		*
		* Return first index of given usymbol_t in ustring_t, or -1 if is not found. Optional argument @c first tells how many chars from beginning have to be skipped
		*
		* @param[in] symbol usymbol_t usymbol_t to be found
		* @param[in] first How many chars from beginning have to be skipped
		* @return Position of `symbol` character in this ustring_t, or -1 if there is no
		* @see last_index_of
		*/
		int_fast32_t index_of_ascii(char symbol, int_fast32_t first = 0) const
		{
			return index_of(symbol, first);
		}

		/**
		* @brief Return first index of given usymbol_t in ustring_t
		*
		* Return first index of given usymbol_t in ustring_t, or -1 if is not found. Optional argument @c first tells how many chars from beginning have to be skipped
		*
		* @param[in] symbol usymbol_t usymbol_t to be found
		* @param[in] first How many chars from beginning have to be skipped
		* @return Position of `symbol` character in this ustring_t, or -1 if there is no
		* @see last_index_of
		*/
		int_fast32_t index_of_wide(wchar_t symbol, int_fast32_t first = 0) const
		{
			return index_of(symbol, first);
		}

		//////////////////////////////////////////////////////////////////////////

		/**
		 * @brief Return first index of given substring in ustring_t
		 *
		 * Return first index of given substring in ustring_t, or -1 if is not found. Optional argument @c first tells how many chars from beginning have to be skipped
		 *
		 * @param[in] str ustring_t substring to be found
		 * @param[in] first How many chars from beginning have to be skipped
		 * @return Position of first @c str character in this ustring_t, or -1 if there is no
		 * @see last_index_of
		 */
		int_fast32_t index_of(const ustring_t & str, int_fast32_t first = 0) const;
		/**
		 * @brief Return first index of given substring in ustring_t
		 *
		 * Return first index of given substring in ustring_t, or -1 if is not found. Optional argument @c first tells how many chars from beginning have to be skipped
		 *
		 * This converts first argument into ustring_t, then passes execution into ustring_t::index_of
		 *
		 * @param[in] str @c ASCII substring to be found
		 * @param[in] first How many chars from beginning have to be skipped
		 * @return Position of first @c str character in this ustring_t, or -1 if there is no
		 * @see index_of, last_index_of
		 */
		int_fast32_t index_of_ascii(const char * str, int_fast32_t first = 0) const
		{
			return index_of(from_ascii(str), first);
		}
		/**
		 * @brief Return first index of given substring in ustring_t
		 *
		 * Return first index of given substring in ustring_t, or -1 if is not found. Optional argument @c first tells how many chars from beginning have to be skipped
		 *
		 * This converts first argument into ustring_t, then passes execution into ustring_t::index_of
		 *
		 * @param[in] str @c UTF-8 substring to be found
		 * @param[in] first How many chars from beginning have to be skipped
		 * @return Position of first @c str character in this ustring_t, or -1 if there is no
		 * @see index_of, last_index_of
		 */
		int_fast32_t index_of_utf8(const char * str, int_fast32_t first = 0) const
		{
			return index_of(from_utf8(str), first);
		}
		/**
		 * @brief Return first index of given substring in ustring_t
		 *
		 * Return first index of given substring in ustring_t, or -1 if is not found. Optional argument @c first tells how many chars from beginning have to be skipped
		 *
		 * This converts first argument into ustring_t, then passes execution into ustring_t::index_of
		 *
		 * @param[in] str @c Wide substring to be found
		 * @param[in] first How many chars from beginning have to be skipped
		 * @return Position of first @c str character in this ustring_t, or -1 if there is no
		 * @see index_of, last_index_of
		 */
		int_fast32_t index_of_wide(const wchar_t * str, int_fast32_t first = 0) const
		{
			return index_of(from_wide(str), first);
		}

		//////////////////////////////////////////////////////////////////////////

		/**
		* @brief Return last index of given usymbol_t in ustring_t
		*
		* Return last index of given usymbol_t in ustring_t, or -1 if is not found. Optional argument @c first tells how many chars from end have to be skipped
		*
		* @param[in] symbol usymbol_t usymbol_t to be found
		* @param[in] last How many chars from end have to be skipped
		* @return Position of `symbol` character in this ustring_t, or -1 if there is no
		* @see last_index_of
		*/
		int_fast32_t last_index_of(usymbol_t symbol, int_fast32_t last = 0) const;

		/**
		* @brief Return first index of given usymbol_t in ustring_t
		*
		* Return first index of given usymbol_t in ustring_t, or -1 if is not found. Optional argument @c first tells how many chars from end have to be skipped
		*
		* @param[in] symbol usymbol_t usymbol_t to be found
		* @param[in] last How many chars from end have to be skipped
		* @return Position of `symbol` character in this ustring_t, or -1 if there is no
		* @see last_index_of
		*/
		int_fast32_t last_index_of_ascii(char symbol, int_fast32_t last = 0) const
		{
			return last_index_of(symbol, last);
		}

		/**
		* @brief Return first index of given usymbol_t in ustring_t
		*
		* Return first index of given usymbol_t in ustring_t, or -1 if is not found. Optional argument @c first tells how many chars from end have to be skipped
		*
		* @param[in] symbol usymbol_t usymbol_t to be found
		* @param[in] last How many chars from end have to be skipped
		* @return Position of `symbol` character in this ustring_t, or -1 if there is no
		* @see last_index_of
		*/
		int_fast32_t last_index_of_wide(wchar_t symbol, int_fast32_t last = 0) const
		{
			return last_index_of(symbol, last);
		}

		//////////////////////////////////////////////////////////////////////////

		/**
		* @brief Return last character ustring_t ONLY if it is ASCII (7-bit)
		*
		* Return last character of ustring_t ONLY If it is ASCII (7-bit). For use in some more advance algorithms.
		* Returns 0 if ustring_t is empty or last character is NOT ASCII.
		*
		* @return Last character as ASCII or 0 if last character is not ASCII
		*/
		char last_ascii() const
		{
			if (_str.length() == 0) return 0;
			if (_str.back() & 0x80) return 0;
			return _str.back();
		}

		//////////////////////////////////////////////////////////////////////////

		/**
		 * @brief Return last index of given substring in ustring_t
		 *
		 * Return last index of given substring in ustring_t, or -1 if is not found. Optional argument @c last tells how many chars from end have to be skipped
		 *
		 * @param[in] str ustring_t substring to be found
		 * @param[in] last How many chars from end have to be skipped
		 * @return Position of first @c str character in this ustring_t, or -1 if there is no
		 * @see index_of
		 */
		int_fast32_t last_index_of(const ustring_t & str, int_fast32_t last = 0) const;
		/**
		 * @brief Return last index of given substring in ustring_t
		 *
		 * Return last index of given substring in ustring_t, or -1 if is not found. Optional argument @c last tells how many chars from end have to be skipped
		 *
		 * This converts first argument into ustring_t, then passes execution into ustring_t::last_index_of
		 *
		 * @param[in] str @c ASCII substring to be found
		 * @param[in] last How many chars from end have to be skipped
		 * @return Position of first @c str character in this ustring_t, or -1 if there is no
		 * @see index_of, last_index_of
		 */
		int_fast32_t last_index_of_ascii(const char * str, int_fast32_t last = 0) const
		{
			return last_index_of(from_ascii(str), last);
		}
		/**
		 * @brief Return last index of given substring in ustring_t
		 *
		 * Return last index of given substring in ustring_t, or -1 if is not found. Optional argument @c last tells how many chars from end have to be skipped
		 *
		 * This converts first argument into ustring_t, then passes execution into ustring_t::last_index_of
		 *
		 * @param[in] str @c UTF-8 substring to be found
		 * @param[in] last How many chars from end have to be skipped
		 * @return Position of first @c str character in this ustring_t, or -1 if there is no
		 * @see index_of, last_index_of
		 */
		int_fast32_t last_index_of_utf8(const char * str, int_fast32_t last = 0) const
		{
			return last_index_of(from_utf8(str), last);
		}
		/**
		 * @brief Return last index of given substring in ustring_t
		 *
		 * Return last index of given substring in ustring_t, or -1 if is not found. Optional argument @c last tells how many chars from end have to be skipped
		 *
		 * This converts first argument into ustring_t, then passes execution into ustring_t::last_index_of
		 *
		 * @param[in] str @c Wide substring to be found
		 * @param[in] last How many chars from end have to be skipped
		 * @return Position of first @c str character in this ustring_t, or -1 if there is no
		 * @see index_of, last_index_of
		 */
		int_fast32_t last_index_of_wide(const wchar_t * str, int_fast32_t last = 0) const
		{
			return last_index_of(from_wide(str), last);
		}

		//////////////////////////////////////////////////////////////////////////

		/**
		 * @brief Converts this ustring_t into standard @c ASCII string
		 *
		 * @return Standard @c ASCII string
		 * @see get_cstring
		 */
		std::string to_ascii() const;
		/**
		 * @brief Converts this ustring_t into standard @c UTF-8 string
		 *
		 * @note Keep in mind that it will be stored in standard @c string container, so many operations on this object won't be reliable, ie @c string::len will give false results!
		 *
		 * @return Standard @c UTF-8 string
		 * @see get_cstring
		 */
		std::string to_utf8() const;
		/**
		 * @brief Converts this ustring_t into standard @c Wide string
		 *
		 * @note This is heavily platform depedent and generally shouldn't be use in production! Android for example defines @c wchar_t as 4 bytes...
		 *
		 * @return Standard @c Wide string
		 * @see get_cstring
		 */
		std::wstring to_wide() const;
		/**
		 * @brief Returns C-like string containing UTF-8 internal string
		 *
		 * @note THIS RETURNS NULL-TERMINATED UTF-8 STRING! Be advise that most (if not all) of standard library function won't be reliable. You canot use this for @c sprintf or @c strlen , or anything like this...
		 *
		 * @note To convert ustring_t into @c ASCII string you need to use ustring_t::to_ascii then call @c c_str() on that!
		 *
		 * @return C-like string contaning UTF-8 internal string
		 */
		const char * get_cstring() const;

		//////////////////////////////////////////////////////////////////////////

		friend bool operator==(const ustring_t & left, const ustring_t & right);
		friend bool operator< (const ustring_t & left, const ustring_t & right);
		friend bool operator< (const ustring_t & left, const ustring_t & right);
		
		friend class cereal::access;
		template <class T> friend ustring_t to_string(const T & item);
		template <class T> friend ustring_t to_string(const T & item, const ustring_t & format);
		template <class T> friend T from_string(const ustring_t & str);

	private:

		explicit ustring_t(const std::string & other) : _str(other)
		{

		}
		explicit ustring_t(std::string && other) : _str(std::move(other))
		{

		}

		std::string _str;

		void _encode(usymbol_t symbol);
		static usymbol_t _decode(const char *& pos);
		static uint_fast32_t _symbol_size(const char * pos);

		template <class archive_t> std::string save_minimal(archive_t const &) const
		{
			return _str;
		}

		template <class archive_t> void load_minimal(archive_t const &, std::string const & value)
		{
			_str = value;
		}
		
	};

	/**
	 * @brief Compares two ustring_t
	 *
	 * Compares two Ustring for equality
	 *
	 * @param[in] left First ustring_t
	 * @param[in] right Second ustring_t
	 * @return @c true if both arguments are equal, @c false otherwise
	 */
	inline bool operator==(const ustring_t & left, const ustring_t & right) { return (left._str == right._str); }
	/**
	 * @brief Compares two ustring_t
	 *
	 * Compares two Ustring for equality
	 *
	 * @param[in] left First ustring_t
	 * @param[in] right Second ustring_t
	 * @return @c true if both arguments are not equal, @c false otherwise
	 */
	inline bool operator!=(const ustring_t & left, const ustring_t & right) { return !operator==(left, right); }
	/**
	 * @brief Compares two ustring_t
	 *
	 * Compares two Ustring for sorting.
	 * @note This is NOT aware of UTF-8 content, and it can (and will!) return wrong answers for national characters!
	 *
	 * @param[in] left First ustring_t
	 * @param[in] right Second ustring_t
	 * @return @c true if left argument is earlier in dictionary then right, @c false otherwise
	 */
	inline bool operator< (const ustring_t & left, const ustring_t & right) { return (left._str < right._str); }
	/**
	 * @brief Compares two ustring_t
	 *
	 * Compares two Ustring for sorting.
	 * @note This is NOT aware of UTF-8 content, and it can (and will!) return wrong answers for national characters!
	 *
	 * @param[in] left First ustring_t
	 * @param[in] right Second ustring_t
	 * @return @c true if left argument is later in dictionary then right, @c false otherwise
	 */
	inline bool operator> (const ustring_t & left, const ustring_t & right) { return  operator< (right, left); }
	/**
	 * @brief Compares two ustring_t
	 *
	 * Compares two Ustring for sorting.
	 * @note This is NOT aware of UTF-8 content, and it can (and will!) return wrong answers for national characters!
	 *
	 * @param[in] left First ustring_t
	 * @param[in] right Second ustring_t
	 * @return @c true if left argument is earlier or equal in dictionary then right, @c false otherwise
	 */
	inline bool operator<=(const ustring_t & left, const ustring_t & right) { return !operator> (left, right); }
	/**
	 * @brief Compares two ustring_t
	 *
	 * Compares two Ustring for sorting.
	 * @note This is NOT aware of UTF-8 content, and it can (and will!) return wrong answers for national characters!
	 *
	 * @param[in] left First ustring_t
	 * @param[in] right Second ustring_t
	 * @return @c true if left argument is later or equal in dictionary then right, @c false otherwise
	 */
	inline bool operator>=(const ustring_t & left, const ustring_t & right) { return !operator< (left, right); }

	typedef std::vector<ustring_t> ustring_collection_t;

	/**
	* @brief Converts generic object into ustring_t
	*
	* @note This function is deliberary not defined,
	* @note so it won't compile unless generic object
	* @note has explicty declared to_string override
	*
	* @param[in] item Generic Object
	* @param[in] format Format
	* @return Converted `ustring_t`
	* @see from_string
	*/
	template <class T> inline ustring_t to_string(const T & item, const ustring_t & format);

	/**
	* @brief Converts generic object into ustring_t
	*
	* @param[in] item Generic Object
	* @return Converted `ustring_t`
	* @see from_string
	*/
	template <class T> inline ustring_t to_string(const T & item)
	{
		return to_string(item, ""_u);
	}

	/**
	* @brief Converts ustring_t into generic object
	*
	* @note This function is deliberary not defined,
	* @note so it won't compile unless generic object
	* @note has explicty declared from_string override
	*
	* @param[in] str `ustring_t` containing object value
	* @return Converted generic object
	* @see to_string
	*/
	template <class T> inline T from_string(const ustring_t & str);

	//////////////////////////////////////////////////////////////////////////

	template<> inline ustring_t to_string<std::filesystem::path>(const std::filesystem::path & item, const ustring_t & format)
	{
		std::string path = item.u8string();
		return ustring_t(path.c_str());
	}

	template<> inline ustring_t to_string<void*>(void* const& item, const ustring_t & format)
	{
		std::stringstream ss;
		ss << "0x" << std::hex << std::setfill('0') << std::setw(sizeof(intptr_t) * 2) << reinterpret_cast<intptr_t>(item);
		return ustring_t(ss.str());
	}
	
	template<> inline ustring_t to_string<int8_t>(const int8_t & item, const ustring_t & format)
	{
		return ustring_t(std::to_string(item));
	}

	template<> inline ustring_t to_string<uint8_t>(const uint8_t & item, const ustring_t & format)
	{
		return ustring_t(std::to_string(item));
	}

	template<> inline ustring_t to_string<int16_t>(const int16_t & item, const ustring_t & format)
	{
		return ustring_t(std::to_string(item));
	}

	template<> inline ustring_t to_string<uint16_t>(const uint16_t & item, const ustring_t & format)
	{
		return ustring_t(std::to_string(item));
	}

	template<> inline ustring_t to_string<int32_t>(const int32_t & item, const ustring_t & format)
	{
		return ustring_t(std::to_string(item));
	}

	template<> inline ustring_t to_string<uint32_t>(const uint32_t & item, const ustring_t & format)
	{
		return ustring_t(std::to_string(item));
	}

	template<> inline ustring_t to_string<int64_t>(const int64_t & item, const ustring_t & format)
	{
		return ustring_t(std::to_string(item));
	}

	template<> inline ustring_t to_string<uint64_t>(const uint64_t & item, const ustring_t & format)
	{
		return ustring_t(std::to_string(item));
	}

	template<> inline ustring_t to_string<float>(const float & item, const ustring_t & format)
	{
		return ustring_t(std::to_string(item));
	}

	template<> inline ustring_t to_string<double>(const double & item, const ustring_t & format)
	{
		return ustring_t(std::to_string(item));
	}
	
	template<> inline ustring_t to_string<bool>(const bool & item, const ustring_t & format)
	{
		if (item) return ustring_t::from_ascii("true");
		else return ustring_t::from_ascii("false");
	}

	template<> inline ustring_t to_string<ustring_collection_t>(const ustring_collection_t & item, const ustring_t & format)
	{
		ustring_t ret;

		for (auto & iter : item)
		{
			if (ret.len() > 0) ret.append_utf8(u8"|");
			ret.append(iter);
		}
		return ret;
	}

	template<> inline ustring_t to_string<color_t>(const color_t & item, const ustring_t & format)
	{
		std::stringstream ss;
		ss << "#" << std::hex << std::setfill('0') << std::setw(sizeof(uint32_t) * 2)
			<< static_cast<uint32_t>(item.get());

		return ustring_t(ss.str());
	}

	template <> inline ustring_t to_string<std::chrono::duration<double> >(const std::chrono::duration<double> & item, const ustring_t & format)
	{
		uint32_t int_hours = std::chrono::duration_cast<std::chrono::hours>(item).count();
		uint32_t int_minutes = std::chrono::duration_cast<std::chrono::minutes>(item).count() % 60;
		uint32_t int_seconds = std::chrono::duration_cast<std::chrono::seconds>(item).count() % 60;
		uint32_t int_miliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(item).count() % 1000;

		const ustring_t str_zero = "0"_u;
		const ustring_t str_none = ""_u;

		std::string ret = "";
		if (int_hours < 10) ret.push_back('0');
		ret += std::to_string(int_hours);

		ret.push_back(':');
		if (int_minutes < 10) ret.push_back('0');
		ret += std::to_string(int_minutes);

		ret.push_back(':');
		if (int_seconds < 10) ret.push_back('0');
		ret += std::to_string(int_seconds);

		ret.push_back(':');
		if (int_miliseconds < 10) ret.push_back('0');
		if (int_miliseconds < 100) ret.push_back('0');
		ret += std::to_string(int_miliseconds);

		return ustring_t(ret);
	}

	template <> inline ustring_t to_string<std::chrono::seconds>(const std::chrono::seconds & item, const ustring_t & format)
	{
		uint32_t int_hours = std::chrono::duration_cast<std::chrono::hours>(item).count();
		uint32_t int_minutes = std::chrono::duration_cast<std::chrono::minutes>(item).count() % 60;
		uint32_t int_seconds = std::chrono::duration_cast<std::chrono::seconds>(item).count() % 60;

		const ustring_t str_zero = "0"_u;
		const ustring_t str_none = ""_u;

		std::string ret = "";
		if (int_hours < 10) ret.push_back('0');
		ret += std::to_string(int_hours);

		ret.push_back(':');
		if (int_minutes < 10) ret.push_back('0');
		ret += std::to_string(int_minutes);

		ret.push_back(':');
		if (int_seconds < 10) ret.push_back('0');
		ret += std::to_string(int_seconds);

		return ustring_t(ret);
	}

	template <> inline ustring_t to_string<std::chrono::system_clock::time_point>(const std::chrono::system_clock::time_point & item, const ustring_t & format)
	{
		std::time_t time = std::chrono::system_clock::to_time_t(item);
		std::tm timetm = *std::localtime(&time);
		std::stringstream ss;
		ss << std::put_time(&timetm, "%c");
		std::string str = ss.str();
		return ustring_t::from_ascii(str.c_str());
	}


	template<> inline ustring_t to_string<ustring_t>(const ustring_t & item, const ustring_t & format)
	{
		return item;
	}

	template<> inline ustring_t to_string<std::thread::id>(const std::thread::id & id, const ustring_t & format)
	{
		std::stringstream ss;
		ss << id;
		return ustring_t(ss.str());
	}

	template<> inline ustring_t to_string<id_t>(const id_t & item, const ustring_t & format)
	{
		id_t::underlying_type val = static_cast<id_t::underlying_type>(item);
		
		char buffer[5];
		buffer[0] = ((val >> 24) & 0xff);
		buffer[1] = ((val >> 16) & 0xff);
		buffer[2] = ((val >> 8) & 0xff);
		buffer[3] = ((val >> 0) & 0xff);
		buffer[4] = '\0';

		return ustring_t::from_ascii(buffer);
	}

	//////////////////////////////////////////////////////////////////////////

	template<> inline std::filesystem::path from_string(const ustring_t & str)
	{
		return std::filesystem::path(str.get_cstring());
	}

	template<> inline void* from_string<void*>(const ustring_t & str)
	{
		if(sizeof(intptr_t) == 4)
			return reinterpret_cast<void*>(std::strtoul(str.get_cstring(), nullptr, 0));
		else
			return reinterpret_cast<void*>(std::strtoull(str.get_cstring(), nullptr, 0));
	}

	template<> inline int8_t from_string(const ustring_t & str)
	{
		return static_cast<int8_t>(std::stoll(str._str));
	}

	template<> inline uint8_t from_string(const ustring_t & str)
	{
		return static_cast<uint8_t>(std::stoull(str._str));
	}

	template<> inline int16_t from_string(const ustring_t & str)
	{
		return static_cast<int16_t>(std::stoll(str._str));
	}

	template<> inline uint16_t from_string(const ustring_t & str)
	{
		return static_cast<uint16_t>(std::stoull(str._str));
	}

	template<> inline int32_t from_string(const ustring_t & str)
	{
		return static_cast<int32_t>(std::stoll(str._str));
	}

	template<> inline uint32_t from_string(const ustring_t & str)
	{
		return static_cast<uint32_t>(std::stoull(str._str));
	}

	template<> inline int64_t from_string(const ustring_t & str)
	{
		return static_cast<int64_t>(std::stoll(str._str));
	}

	template<> inline uint64_t from_string(const ustring_t & str)
	{
		return static_cast<uint64_t>(std::stoull(str._str));
	}

	template<> inline float from_string(const ustring_t & str)
	{
		return static_cast<float>(std::stof(str._str));
	}

	template<> inline double from_string(const ustring_t & str)
	{
		return static_cast<double>(std::stod(str._str));
	}
	
	template<> inline bool from_string(const ustring_t & str)
	{
		ustring_t lower = str.to_lower();
		return !(!strcmp(lower.get_cstring(), "false") || !strcmp(lower.get_cstring(), "0") ||
				!strcmp(lower.get_cstring(), "") || !strcmp(lower.get_cstring(), "null") ||
				!strcmp(lower.get_cstring(), "nul"));
	}

	template<> inline ustring_collection_t from_string(const ustring_t & str)
	{
		ustring_collection_t ret;

		int start = 0;
		int index_of = str.index_of('|', 1);
		while (index_of != -1)
		{
			ret.push_back(str.substr(start, index_of - start));

			start = index_of + 1;
			index_of = str.index_of('|', start + 1);
		}
		if (start < str.len())
		{
			ret.push_back(str.substr(start, str.len() - start));
		}

		return ret;
	}

	template<> inline color_t from_string<color_t>(const ustring_t & str)
	{
		uint32_t color;
		if(*str.begin() == '#')
			color = static_cast<uint32_t>(std::strtoul(str.get_cstring() + 1, nullptr, 16));
		else
			color = static_cast<uint32_t>(std::strtoul(str.get_cstring(), nullptr, 16));

		return color_t(color);
	}

	template<> inline std::chrono::duration<double> from_string(const ustring_t & str)
	{
		std::chrono::duration<double> ret;
		
		int_fast32_t level = 0;
		size_t start = str.len();

		const std::string & str_internal = str.to_utf8();

		for (size_t i = str_internal.length(); i-- > 0;)
		{
			usymbol_t u = str_internal[i];
			if (u == ':')
			{
				int_fast64_t number = 0;
				for (size_t s = i + 1; s < start; s++)
				{
					u = str_internal[s];
					if (isdigit(u))
					{
						number *= 10;
						number += u - '0';
					}
				}
				start = i;
				++level;
				switch (level)
				{
				case 1: ret += std::chrono::milliseconds(number); break;
				case 2: ret += std::chrono::seconds(number); break;
				case 3: ret += std::chrono::minutes(number); break;
				case 4: ret += std::chrono::hours(number); break;
				}
			}
		}
		return ret;
	}

	template<> inline std::chrono::seconds from_string(const ustring_t & str)
	{
		std::chrono::seconds ret;
		
		int_fast32_t level = 0;
		size_t start = str.len();

		const std::string & str_internal = str.to_utf8();

		for (size_t i = str_internal.length(); i-- > 0;)
		{
			usymbol_t u = str_internal[i];
			if (u == ':')
			{
				int_fast64_t number = 0;
				for (size_t s = i + 1; s < start; s++)
				{
					u = str_internal[s];
					if (isdigit(u))
					{
						number *= 10;
						number += u - '0';
					}
				}
				start = i;
				++level;
				switch (level)
				{
				case 1: ret += std::chrono::seconds(number); break;
				case 2: ret += std::chrono::minutes(number); break;
				case 3: ret += std::chrono::hours(number); break;
				}
			}
		}
		return ret;
	}
	
	template<> inline std::chrono::system_clock::time_point from_string(const ustring_t & str)
	{
		return std::chrono::system_clock::now(); // ToDo: Fix me!
	}

	template<> inline id_t from_string(const ustring_t & str)
	{
		uint32_t ret = 0;
		const std::string & str_internal = str.to_utf8();
		auto iter = str_internal.begin();
		int pos = 0;
		while (iter != str_internal.end() && pos < 4)
		{
			ret = (ret << 8) | (*iter & 0xff);
			pos++;
			iter++;
		}
		while (pos < 4)
			ret <<= 8;

		return ret;
	}

	template<> inline ustring_t from_string(const ustring_t & str)
	{
		return str;
	}

	/**
	* @brief Serilizes ustring_collection_t
	*
	* This function is used by cereal serializer
	*
	* @note String will be separated by '|' mark
	* @todo Escape | if already part of the string in collection
	*
	* @param[in] obj Collection to serialize
	* @return Serialized string
	* @see to_string, load_minimal
	*/
	template <class archive_t> std::string save_minimal(archive_t const &, ustring_collection_t const & obj)
	{
		return to_string(obj).to_utf8();
	}

	/**
	* @brief Deserialize string into ustring_collection_t
	*
	* This function is used by cereal deserializer
	*
	* @note String will be separated by '|' mark
	*
	* @param[out] obj Collection to store the result
	* @param[in] value String containg serialized collection
	* @see to_string, save_minimal
	*/
	template <class archive_t> void load_minimal(archive_t const &, ustring_collection_t & obj, std::string const & value)
	{
		obj = from_string<ustring_collection_t>(engine::ustring_t::from_utf8(value.c_str()));
	}

	/**
	 * @brief Compare two ustring_t::iterator_t for equality
	 * 
	 * @param[in] lhs First iterator_t
	 * @param[in] rhs Second iterator_t
	 * @return @c true if both arguments are equal, @c false otherwise
	 */
	inline bool operator==(const ustring_t::iterator_t& lhs, const ustring_t::iterator_t& rhs)
	{
		return lhs.equals(rhs);
	}

	/**
	 * @brief Compare two ustring_t::iterator_t for equality
	 * 
	 * @param[in] lhs First iterator_t
	 * @param[in] rhs Second iterator_t
	 * @return @c true if both arguments are not equal, @c false otherwise
	 */
	inline bool operator!=(const ustring_t::iterator_t& lhs, const ustring_t::iterator_t& rhs)
	{
		return !lhs.equals(rhs);
	}
}

namespace std
{

	/**
	* @brief Hash functor to be used for std collections
	*
	* This is a helper object for std collection.
	* You should never use it directly
	*
	* @see engine::ustring_t
	*/
	template<> struct hash<engine::ustring_t>
	{
		typedef engine::ustring_t argument_type_t;
		typedef std::size_t result_t;
		result_t operator()(argument_type_t const& s) const
		{
			return std::hash<std::string>{}(s._str);
		}
	};


	/**
	 * @brief Swaps two string iterators
	 * 
	 * @param[in] lhs First iterator_t
	 * @param[in] rhs Second iterator_t
	 */
	inline void swap(engine::ustring_t::iterator_t& lhs, engine::ustring_t::iterator_t& rhs)
	{
		lhs.swap(rhs);
	}
}

#include "utility/text/format/string.hpp"
#include "utility/test/print_to.hpp"

#endif
