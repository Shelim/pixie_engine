#ifndef ENGINE_COMMON_UTILITY_MANIFEST_VERSION_HPP
#define ENGINE_COMMON_UTILITY_MANIFEST_VERSION_HPP

#include "utility/text/ustring.hpp"
#include "utility/text/parser.hpp"

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

		ustring_t get_string() const
		{
			return format_string("#1#.#2#.#3#.#4#"_u, get_major(), get_minor(), get_revision(), get_build());
		}

	private:

	};

}

#endif