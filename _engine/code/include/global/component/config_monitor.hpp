#ifndef ENGINE_COMPONENT_CONFIG_MONITOR_HPP
#define ENGINE_COMPONENT_CONFIG_MONITOR_HPP
#pragma once

#include "global/component/config_common/enum.hpp"
#include "global/component/config_common/changed_provider.hpp"

namespace engine
{

	class config_monitor_t
	{

	public:

		virtual ~config_monitor_t()
		{

		} 

	};

}

#include "global/component/config_monitor/dummy.hpp"
#include "global/component/config_monitor/real.hpp"

#endif