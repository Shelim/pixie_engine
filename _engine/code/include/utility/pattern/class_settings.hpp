#ifndef ENGINE_COMMON_UTILITY_PATTERN_CLASS_SETTINGS_HPP
#define ENGINE_COMMON_UTILITY_PATTERN_CLASS_SETTINGS_HPP
#pragma once

#include <tuple>
#include "utility/text/ustring.hpp"

namespace engine
{

	template<class owner_t> struct settings_manifest_t;

#define SETTINGS_TABLE_START(owner) template<> struct settings_manifest_t<owner> { class values_t { public: virtual ~values_t() { }
#define SETTINGS_TABLE_ENTRY(type, key) virtual type key() const = 0;
#define SETTINGS_TABLE_END() }; };

#define SETTINGS_START(owner, id) class settings_metadata_##owner##_##id##_t : public engine::settings_t<engine::owner>::values_t { public:
#define SETTINGS_SET(type, key, value) type key() const final { return value; }
#define SETTINGS_END() };

#define USE_SETTINGS(owner, id) engine::settings_for<engine::owner, engine::settings_metadata_##owner##_##id##_t>

	template<class owner_t> class settings_t
	{

	public:

		typedef typename settings_manifest_t<owner_t>::values_t values_t;

		virtual ~settings_t()
		{

		}

		virtual values_t * get() = 0;

	};

	template<class owner_t, class values_final_t> class settings_implementation_t : public settings_t<owner_t>
	{
		static_assert(std::is_base_of<typename settings_t<owner_t>::values_t, values_final_t>::value, "data_final_t must be descendet of data_t!");

	public:

		typename settings_t<owner_t>::values_t * get() final
		{
			return &data;
		}

	private:

		values_final_t data;
	};
}

#endif