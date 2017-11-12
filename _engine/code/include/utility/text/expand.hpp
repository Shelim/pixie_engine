#ifndef ENGINE_COMMON_UTILITY_TEXT_EXPAND_HPP
#define ENGINE_COMMON_UTILITY_TEXT_EXPAND_HPP
#pragma once

#include "utility/text/ustring.hpp"

namespace engine
{


	template<std::size_t iter, std::size_t size> inline void expand_into_array(std::array<ustring_t, size> & array)
	{
		// Intentionally do nothing. The array is already populated!
	}

	template<std::size_t iter, std::size_t size, class arg_t, class... args_t> inline void expand_into_array(std::array<ustring_t, size> & array, const arg_t & arg, args_t... args)
	{
		array[iter] = to_string(arg);

		expand_into_array<iter + 1>(array, args...);
	}

	template<class... args_t> inline void expand_into_array(std::array<ustring_t, sizeof...(args_t)> & array, args_t... args)
	{
		expand_into_array<0>(array, args...);
	}


	template<std::size_t iter> inline void expand_into_vector(std::vector<ustring_t> & vec)
	{
		// Intentionally do nothing. The array is already populated!
	}

	template<std::size_t iter, class arg_t, class... args_t> inline void expand_into_vector(std::vector<ustring_t> & vec, const arg_t & arg, args_t... args)
	{
		vec.push_back(to_string(arg));

		expand_into_vector<iter + 1>(vec, args...);
	}

	template<class... args_t> inline void expand_into_vector(std::vector<ustring_t> & vec, args_t... args)
	{
		vec.reserve(sizeof...(args_t));
		expand_into_vector<0>(vec, args...);
	}

}

#endif