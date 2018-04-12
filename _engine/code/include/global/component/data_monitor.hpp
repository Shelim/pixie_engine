#ifndef ENGINE_COMPONENT_DATA_MONITOR_HPP
#define ENGINE_COMPONENT_DATA_MONITOR_HPP
#pragma once

namespace engine
{

	class data_monitor_t
	{

	public:

		virtual ~data_monitor_t()
		{

		} 

	};

}

#include "global/component/data_monitor/dummy.hpp"
#include "global/component/data_monitor/real.hpp"

#endif