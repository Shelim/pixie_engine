#ifndef ENGINE_COMMON_USTRING_HPP
#define ENGINE_COMMON_USTRING_HPP
#pragma once

#include <string>
#include <vector>
#include <clocale>
#include <cstdio>
#include <cstring>
#include <cstdarg>
#include <cinttypes>
#include <limits>
#include <array>
#include <chrono>
#include <functional>
#include <sstream>
#include <iomanip>
#include <cereal/access.hpp>
#include "common/filesystem.hpp"
#include "common/color.hpp"

/**
 * @page ustring_format UString formatting
 * @tableofcontents
 * @section ustring_format_intro Introduction
 *		Pixie uses custom way to format unicode string.
 *		This way is much more flexible than original formatting.
 *		It can be extended in next versions, but will be always kept
 *		backward compatible.
 *
 *		The most important change done to standard C-like `printf` is
 *		the ability to change order of arguments, as needed for translations.
 *		Other changes includes full unicode support and ability to repeat
 *		some arguments more then once.
 * @section ustring_format_tutorial How to use it? (Quick Start)
 *		@code{.cpp}
 *			engine::format_string(_U("Hello my #2# #1# world!"), _U("nice"), _U("really"));
 *			// Outputs: "Hello my really nice world!"
 *
 *			engine::format_string(_U("You can have #1:what?# in tags too!"), _U("comments"));
 *			// Outputs: "You can have comments in tags too!"
 *
 *			engine::format_string(_U("Argument type is detected on compile time: #1# (#3#)"), true, 1.0F, _U("As you can see you can skip the numbers!"));
 *			// Outputs: "Argument type is detected on compile time: True (As you can see you can skip the numbers!)"
 *		@endcode
 * @section ustring_format_reference Full Reference
 *		@subsection ustring_format_reference_structure Structure
 *			@code
 *				#order#
 *				#order:comment#
 *			@endcode
 *		@subsection ustring_format_reference_order Order
 *			Order is a positive number starting from `1`. Can span more than one character (ie `26`).
 *			Can be repeated (ie the same argument used more than once with the same or different key)
 *			@warning There must be no spaces between # and order!
 *		@subsection ustring_format_reference_comment Comment
 *			Comment starts with `:` then span till ending `#` is reached. It will be completely
 *			ignored by parser, but can give hint to the translators what will be substituted
 *			during formatting pass. Can contain any unicode characters
 *		@subsection ustring_format_reference_escape Escape hash
 *			You can use ## every time you wish to escape given hash (## will be replaced by single # and ignored by parser)
 *		@subsection ustring_format_reference_error Error handling
 *			Formatting is safe for arguments having incorect value (ie. 0 or higher then number of arguments)
 *			In such case the formatting code will be safely substituted for empty string
 * @see engine::ustring_t, engine::ustring_t::format_string
 */

namespace engine
{
	/**
	 * @brief Unicode Symbol (32-bit unsigned value)
	 */
	typedef int_least32_t usymbol_t;

	/**
	* @brief Converts usymbol_t into uppercase
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
	bool is_whitespace(usymbol_t item);

	/**
	* @brief Check if given usymbol_t contains path separator
	*
	* @remark This function MAY BE platform depedent!
	*
	* @param[in] item usymbol_t to check
	* @return true if usymbol_t contains path separator
	*/
	bool is_path_separator(usymbol_t ch);

	/**
	 * @brief Class for handling @c UTF-8 string
	 */
	class ustring_t final
	{

	public:

		friend class std::hash<engine::ustring_t>;

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
		 * @brief Returns Unicode Symbol at given position in ustring_t
		 *
		 * @param[in] pos Position in ustring_t
		 * @return Full Unicode glyph code
		 */
		usymbol_t at(uint_fast32_t pos) const;
		/**
		 * @brief Returns Unicode Symbol at given position in ustring_t
		 *
		 * @param[in] pos Position in ustring_t
		 * @return Full Unicode glyph code
		 */
		usymbol_t operator[](uint_fast32_t pos) const
		{
			return at(pos);
		}
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
		
		template<class... Args> friend ustring_t format_string(const ustring_t & str, Args... args);
		friend class cereal::access;
		template <class T> friend ustring_t to_string(const T & item);
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

		template<class... Args> static inline void _format(ustring_t & str, Args... args)
		{
			if (sizeof...(Args) > 0)
			{
				std::array<ustring_t, sizeof...(Args)> a;
				expand<0>(a, args...);
				str = _base_format(str, a);
			}
		}

		template<std::size_t iter, std::size_t size, class Arg, class... Args> static void expand(std::array<ustring_t, size> & array, const Arg & arg, Args... args);

		template<std::size_t iter, std::size_t size> static inline void expand(std::array<ustring_t, size> & array)
		{
			// Intentionally do nothing. The array is already populated!
		}
		
		template<std::size_t size> static inline ustring_t _base_format(ustring_t str, std::array<ustring_t, size> & args)
		{
			char * it = const_cast<char*>(str._str.c_str());
			ustring_t ret;
			ret._str.reserve(str._str.length());

			enum state_t
			{
				closed,
				just_opened,
				number,
				comment
			};

			char * tag_number_start;

			state_t state = state_t::closed;

			while (*it)
			{
				uint_fast32_t symbol_size = ustring_t::_symbol_size(it);
				usymbol_t symbol = ustring_t::_decode(const_cast<const char *&>(it));
				if (symbol == '#')
				{
					if (state == state_t::closed)
					{
						tag_number_start = it;
						state = state_t::just_opened;
					}
					else if (state == state_t::just_opened)		// This is doubled '##'
					{
						ret._encode(symbol);
						state = state_t::closed;
					}
					else if (state == state_t::number || state == state_t::comment)
					{
						if (state == state_t::number)
						{
							*(it - symbol_size) = 0;
						}

						int32_t id = atoi(tag_number_start);
						--id;

						if (id >= 0 || id < size)
							ret.append(args[id]);

						state = state_t::closed;
					}
				}
				else if (symbol == ':' && state == state_t::number)
				{
					*(it - symbol_size) = 0;
					state = state_t::comment;
				}
				else
				{
					if (state == state_t::just_opened)
						state = state_t::number;
					else if (state == state_t::closed)
						ret._encode(symbol);
				}
			}
			return ret;
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

	/**
	 * @brief Helper macro. Do not use!
	 *
	 * @note This is defined so you can do `_U(__FILE__)` or anything like that...
	 * @see _U
	 */
#define _U2(x) engine::ustring_t::from_utf8(u8##x)
	/**
	 * @brief Converts string literal into engine::ustring_t
	 * @note Do not use on variables!
	 */
#define _U(x) _U2(x)

	 /**
	 * @brief Helper macro. Do not use!
	 *
	 * @note This is defined so you can do `_U(__FILE__)` or anything like that...
	 * @see _U
	 */
#define _L2(x) L##x
	 /**
	 * @brief Converts string literal into engine::ustring_t
	 * @note Do not use on variables!
	 */
#define _L(x) _L2(x)

	typedef std::vector<ustring_t> ustring_collection_t;

	/**
	* @brief Converts generic object into ustring_t
	*
	* @note This function is deliberary not defined,
	* @note so it won't compile unless generic object
	* @note has explicty declared to_string override
	*
	* @param[in] item Generic Object
	* @return Converted `ustring_t`
	* @see from_string
	*/
	template <class T> inline ustring_t to_string(const T & item);

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

	template<> inline ustring_t to_string<std::filesystem::path>(const std::filesystem::path & item)
	{
		std::string path = item.u8string();
		return ustring_t(path.c_str());
	}

	template<> inline ustring_t to_string<void*>(void* const& item)
	{
		std::stringstream ss;
		ss << "0x" << std::hex << std::setfill('0') << std::setw(sizeof(intptr_t) * 2) << reinterpret_cast<intptr_t>(item);
		return ustring_t(ss.str());
	}
	
	template<> inline ustring_t to_string<int8_t>(const int8_t & item)
	{
		return ustring_t(std::to_string(item));
	}

	template<> inline ustring_t to_string<uint8_t>(const uint8_t & item)
	{
		return ustring_t(std::to_string(item));
	}

	template<> inline ustring_t to_string<int16_t>(const int16_t & item)
	{
		return ustring_t(std::to_string(item));
	}

	template<> inline ustring_t to_string<uint16_t>(const uint16_t & item)
	{
		return ustring_t(std::to_string(item));
	}

	template<> inline ustring_t to_string<int32_t>(const int32_t & item)
	{
		return ustring_t(std::to_string(item));
	}

	template<> inline ustring_t to_string<uint32_t>(const uint32_t & item)
	{
		return ustring_t(std::to_string(item));
	}

	template<> inline ustring_t to_string<int64_t>(const int64_t & item)
	{
		return ustring_t(std::to_string(item));
	}

	template<> inline ustring_t to_string<uint64_t>(const uint64_t & item)
	{
		return ustring_t(std::to_string(item));
	}

	template<> inline ustring_t to_string<float>(const float & item)
	{
		return ustring_t(std::to_string(item));
	}

	template<> inline ustring_t to_string<double>(const double & item)
	{
		return ustring_t(std::to_string(item));
	}
	
	template<> inline ustring_t to_string<bool>(const bool & item)
	{
		if (item) return ustring_t::from_ascii("true");
		else return ustring_t::from_ascii("false");
	}

	template<> inline ustring_t to_string<ustring_collection_t>(const ustring_collection_t & item)
	{
		ustring_t ret;

		for (auto & iter : item)
		{
			if (ret.len() > 0) ret.append_utf8(u8"|");
			ret.append(iter);
		}
		return ret;
	}

	template<> inline ustring_t to_string<color_t>(const color_t & item)
	{
		std::stringstream ss;
		ss << "#" << std::hex << std::setfill('0') << std::setw(sizeof(uint32_t) * 2)
			<< static_cast<uint32_t>(item.get());

		return ustring_t(ss.str());
	}

	template<class... Args> ustring_t format_utf8(const char * str, Args... args);

	template <> inline ustring_t to_string<std::chrono::seconds>(const std::chrono::seconds & item)
	{
		uint32_t int_hours = std::chrono::duration_cast<std::chrono::hours>(item).count();
		uint32_t int_minutes = std::chrono::duration_cast<std::chrono::minutes>(item).count() % 60;
		uint32_t int_seconds = std::chrono::duration_cast<std::chrono::seconds>(item).count() % 60;

		const ustring_t str_zero = _U("0");
		const ustring_t str_none = _U("");

		return format_utf8(u8"#1##2#:#3##4#:#5##6#", (int_hours < 10 ? str_zero : str_none), int_hours,
			(int_minutes < 10 ? str_zero : str_none), int_minutes,
			(int_seconds < 10 ? str_zero : str_none), int_seconds);
	}

	template<> inline ustring_t to_string<ustring_t>(const ustring_t & item)
	{
		return item;
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
		if(str[0] == '#')
			color = static_cast<uint32_t>(std::strtoul(str.get_cstring() + 1, nullptr, 16));
		else
			color = static_cast<uint32_t>(std::strtoul(str.get_cstring(), nullptr, 16));

		return color_t(color);
	}

	template<> inline std::chrono::seconds from_string(const ustring_t & str)
	{
		std::chrono::seconds ret;
		
		int_fast32_t level = 0;
		size_t start = str.len();

		for (size_t i = str.len(); i-- > 0;)
		{
			usymbol_t u = str[i];
			if (u == ':')
			{
				int_fast64_t number = 0;
				for (size_t s = i + 1; s < start; s++)
				{
					u = str[s];
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

	template<> inline ustring_t from_string(const ustring_t & str)
	{
		return str;
	}

    /**
     * @brief Formats string with arguments
     *
     * @note This function differs from standard C-like formatting.
     * @note See complete instruction at @ref ustring_format "Formatting Description"
     *
     * @param[in] str ustring_t containing format
     * @param[in] args Zero or more arguments to format
     * @return Formated ustring_t
     */
    template<class... Args> ustring_t format_string(const ustring_t & str, Args... args)
    {
        ustring_t ret = str;
		ustring_t::_format(ret, args...);
        return ret;
    }
    /**
     * @brief Formats string with arguments
     *
     * This converts first argument into ustring_t, then passes execution into ustring_t::format_string
     *
     * @note This function differs from standard C-like formatting.
     * @note See complete instruction at @ref ustring_format "Formatting Description"
     *
     * @param[in] str @c ASCII string containing format
     * @param[in] args Zero or more arguments to format
     * @return Formated ustring_t
     * @see format_string
     */
    template<class... Args> ustring_t format_ascii(const char * str, Args... args)
    {
        return format_string(ustring_t::from_ascii(str), args...);
    }
    /**
     * @brief Formats string with arguments
     *
     * This converts first argument into ustring_t, then passes execution into ustring_t::format_string
     *
     * @note This function differs from standard C-like formatting.
     * @note See complete instruction at @ref ustring_format "Formatting Description"
     *
     * @param[in] str @c UTF-8 string containing format
     * @param[in] args Zero or more arguments to format
     * @return Formated ustring_t
     * @see format_string
     */
    template<class... Args> ustring_t format_utf8(const char * str, Args... args)
    {
        return format_string(ustring_t::from_utf8(str), args...);
    }
    /**
     * @brief Formats string with arguments
     *
     * This converts first argument into ustring_t, then passes execution into ustring_t::format_string
     *
     * @note This function differs from standard C-like formatting.
     * @note See complete instruction at @ref ustring_format "Formatting Description"
     *
     * @param[in] str @c Wide string containing format
     * @param[in] args Zero or more arguments to format
     * @return Formated ustring_t
     * @see format_string
     */
    template<class... Args> ustring_t format_wide(const wchar_t * str, Args... args)
    {
        return format_string(ustring_t::from_wide(str), args...);
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

	template<std::size_t iter, std::size_t size, class Arg, class... Args> inline void ustring_t::expand(std::array<ustring_t, size> & array, const Arg & arg, Args... args)
	{
		array[iter] = to_string(arg);

		expand<iter + 1>(array, args...);
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
}

#endif
