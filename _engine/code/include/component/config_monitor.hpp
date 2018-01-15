#ifndef ENGINE_COMPONENT_CONFIG_MONITOR_HPP
#define ENGINE_COMPONENT_CONFIG_MONITOR_HPP
#pragma once

#include "core/monitor/base.hpp"
#include "component/config_common/enum.hpp"
#include "component/config_common/changed_provider.hpp"

namespace engine
{

	class config_monitor_t : public monitor_base_t
	{

	public:

		virtual ~config_monitor_t()
		{

		} 

	};

}

#include "component/config_monitor/dummy.hpp"
#include "component/config_monitor/real.hpp"

#endif