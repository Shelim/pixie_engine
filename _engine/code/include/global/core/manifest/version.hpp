#ifndef ENGINE_COMMON_UTILITY_MANIFEST_VERSION_HPP
#define ENGINE_COMMON_UTILITY_MANIFEST_VERSION_HPP

#include "utility/text/ustring.hpp"
#include "utility/container/version.hpp"

#pragma once

namespace engine
{

	class manifest_version_t
	{

	public:

		const version_t & get_version() const
		{
			return version;
		}

	private:

		static version_t version;

	};

}

#endif