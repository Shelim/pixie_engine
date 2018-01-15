#ifndef ENGINE_COMMON_UTILITY_MANIFEST_VERSION_HPP
#define ENGINE_COMMON_UTILITY_MANIFEST_VERSION_HPP

#include "utility/text/ustring.hpp"

#pragma once

namespace engine
{

	class manifest_version_t
	{

	public:

		uint32_t get_major() const;
		uint32_t get_minor() const;
		uint32_t get_revision() const;
		uint32_t get_build() const;

	private:

	};

	template<> inline ustring_t to_string<manifest_version_t>(const manifest_version_t & item)
	{
		return format_string("#1#.#2#.#3#.#4#"_u, item.get_major(), item.get_minor(), item.get_revision(), item.get_build());
	}

}

#endif