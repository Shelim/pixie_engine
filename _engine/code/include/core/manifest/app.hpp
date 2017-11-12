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
#define ENGINE_APP_STD(app) app,
#include "std/app_std.hpp"
			count
		};

		static const ustring_t & get_app_name(app_t app)
		{
			switch (app)
			{
#define ENGINE_APP_STD(app) case app_t::app: return #app##_u;
#include "std/app_std.hpp"
			}
			return "unknown"_u;
		}

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

}

#endif