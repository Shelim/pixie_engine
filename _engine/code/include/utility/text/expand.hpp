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

	class tuple_expand_t
	{

	public:

		template<class T> ustring_t operator()(T & tuple, int index)
		{
			return tuple_item_to_string(tuple, index, ""_u);
		}

		template<class T> ustring_t operator()(T & tuple, int index, const ustring_t & format)
		{
			return tuple_item_to_string(tuple, index, format);
		}

	private:

		template<int... Is> struct seq_t {};
		template<int N, int... Is> struct gen_seq_t : gen_seq_t<N - 1, N - 1, Is...> {};
		template<int... Is> struct gen_seq_t<0, Is...> : seq_t<Is...> {};

		template<int N, class T>
		static ustring_t tupple_to_string(const T& p, const ustring_t & format)
		{
			return to_string(std::get<N>(p), format);
		}

		template<class T, int... Is>
		static ustring_t tuple_item_to_string(const T& p, int index, const ustring_t & format, seq_t<Is...>)
		{
			using FT = ustring_t(const T&, const ustring_t &);
			static constexpr FT* arr[] = { &tupple_to_string<Is, T>... };
			return arr[index](p, format);
		}

		template<class T>
		static ustring_t tuple_item_to_string(const T& p, int index, const ustring_t & format)
		{
			return tuple_item_to_string(p, index, format, gen_seq_t<std::tuple_size<T>::value>{});
		}
	};

	template <class... args_t> inline ustring_t expand_into_string(int index, args_t... args)
	{
		return tuple_expand_t{}(std::forward_as_tuple<args_t...>(args), index);
	}

	template <class... args_t> inline ustring_t expand_into_string(int index, const ustring_t & formatter, args_t... args)
	{
		return tuple_expand_t{}(std::forward_as_tuple<args_t...>(args), index, formatter);
	}

}

#endif