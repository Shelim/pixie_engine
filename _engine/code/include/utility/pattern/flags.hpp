#ifndef ENGINE_COMMON_UTILITY_PATTERN_FLAGS_HPP
#define ENGINE_COMMON_UTILITY_PATTERN_FLAGS_HPP
#pragma once

#include <unordered_map>
#include <bitset>
#include <type_traits>
#include "utility/pattern/enum.hpp"
#include <mutex>

namespace engine
{
	/**
	 * \~English @brief Class for managing flags based on given strongly-typed enumerations
	 * 
	 * This class allows making dynamic flags based on strongly-typed, sequencial enums
	 * 
	 * @warning It is expected that last element of the given strongly-typed enum will be named @c count !
	 * 
	 * @code{.cpp}
	 * enum class flag_t
	 * {
	 *     foo,
	 *     bar,
	 *     baz,
	 *     count // This is important!
	 * };
	 * flags_t<flag_t> flags;
	 * 
	 * flags.set_flag(flag_t::foo, true);
	 * if(flags.is_flag(flag_t::bar)) return true;
	 * @endcode
	 * 
	 * @tparam flag_t strongly-typed enumeration (@c enum @c class) to be used as base of flags
	 */
	template<class flag_t> class flags_t final
	{

	public:

		/**
		 * \~English @brief Sets given flag to value val
		 * 
		 * @param[in] flag to set
		 * @param[in] val true to set, false to clear
		 */
		void set_flag(flag_t flag, bool val = true)
		{
			flags.set(value_of(flag), val);
		}

		/**
		 * \~English @brief Checks if given flag is set
		 * 
		 * @param[in] flag to check
		 * @return true if flag is set, false otherwise
		 */
		bool is_flag(flag_t flag) const
		{
			return flags.test(value_of(flag));
		}

		/**
		 * \~English @brief Resets all flags to the @c clear status
		 */
		void clear_all()
		{
			flags.reset();
		}

	private:

		std::bitset<value_of(flag_t::count)> flags;
	};

}

#endif
