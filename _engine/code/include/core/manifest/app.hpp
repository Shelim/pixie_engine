#ifndef ENGINE_COMMON_UTILITY_MANIFEST_APP_HPP
#define ENGINE_COMMON_UTILITY_MANIFEST_APP_HPP

#include "utility/text/ustring.hpp"

#pragma once

namespace engine
{

	class manifest_app_t
	{

	public:

#include "def/enum/app.def"

		app_t get_local_app();

		manifest_app_t(app_t local_app);

	private:

		app_t local_app;

	};

}

#define ENGINE_ENUM_HEADER_TO_USE "def/enum/app.def"
#include "core/utility/enum_to_string.hpp"

#endif