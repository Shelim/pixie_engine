#ifndef ENGINE_COMMON_UTILITY_MANIFEST_APP_HPP
#define ENGINE_COMMON_UTILITY_MANIFEST_APP_HPP

#include "utility/text/ustring.hpp"
#include "global/core/app/context.hpp"
#include "global/core/app.hpp"
#include "boost/di.hpp"

#pragma once

namespace engine
{

	class manifest_app_t
	{

	public:

		app_t::kind_t get_local_app() const;

		app_t::instance_id_t get_local_instance_id() const;

		BOOST_DI_INJECT(manifest_app_t, app_t::kind_t local_app, (named = engine::instance_id) app_t::instance_id_t local_instance_id);

	private:

		app_t::kind_t local_app;
		app_t::instance_id_t local_instance_id;
	};

}

#endif