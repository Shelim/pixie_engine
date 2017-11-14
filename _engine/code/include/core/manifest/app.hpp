#ifndef ENGINE_COMMON_UTILITY_MANIFEST_APP_HPP
#define ENGINE_COMMON_UTILITY_MANIFEST_APP_HPP

#include "utility/text/ustring.hpp"

#pragma once

namespace engine
{

	class manifest_app_t
	{

	public:

		enum class app_t
		{
#define ENGINE_APP_DEF(app) app,
#include "def/app.def"
			count
		};

		app_t get_local_app()
		{
			return local_app;
		}

		manifest_app_t(app_t local_app) : local_app(local_app)
		{

		}

	private:

		app_t local_app;

	};
	
#define STRINGIFY_ENUM_TYPE manifest_app_t::app_t
#define ENGINE_APP_DEF STRINGIFY_DEF_NAME
#define STRINGIFY_DEF_INCLUDE "def/app.def"
#include "core/utility/stringify_def.hpp"

}

#endif