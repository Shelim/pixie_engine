#ifndef ENGINE_COMMON_UTILITY_CONTAINER_COMPILE_TIME_MAP_HPP
#define ENGINE_COMMON_UTILITY_CONTAINER_COMPILE_TIME_MAP_HPP
#pragma once

namespace engine
{

	template<class key_t, class value_t> class compile_time_map_t
	{

	public:

		template<key_t key_value, value_t value_value> class pair_t
		{

		public:

			static const key_t key = key_value;
			static const value_t value = value_value;
		};

		template<class ... values> class instance_from_pairs_t
		{

		public:

			template<key_t key> static constexpr value_t get()
			{
				return get_local<key, values...>();
			}

			static value_t get(key_t key)
			{
				return get_local<values...>(key);
			}

		private:

			template<key_t key> static constexpr value_t get_local()
			{
				return value_t{};
			}

			template<key_t key, class pair, class ... pairs> static constexpr value_t get_local()
			{
				return (pair::key == key) ? pair::value : get_local<key, pairs...>();
			}

			static value_t get_local(key_t key)
			{
				return value_t{};
			}

			template<class pair, class ... pairs> static value_t get_local(key_t key)
			{
				return (pair::key == key) ? pair::value : get_local<pairs...>(key);
			}
		};

	};
}
#endif