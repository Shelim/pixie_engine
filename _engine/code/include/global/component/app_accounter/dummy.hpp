#ifndef ENGINE_COMPONENT_APP_ACCOUNTER_DUMMY_HPP
#define ENGINE_COMPONENT_APP_ACCOUNTER_DUMMY_HPP
#pragma once

#include "global/component/app_accounter.hpp"

namespace engine
{

	class app_accounter_dummy_t : public app_accounter_t
	{

	public:

		app_accounter_dummy_t()
		{

		}

		apps_meta_t get_running_apps() final
		{
			apps_meta_t ret;
			return ret;	
		}

	private:

	};
}

#endif