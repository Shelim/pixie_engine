#ifndef ENGINE_COMPONENT_DATA_ITEMS_HPP
#define ENGINE_COMPONENT_DATA_ITEMS_HPP
#pragma once

namespace engine
{

	class data_items_t
	{

	public:

		virtual ~data_items_t()
		{

		} 

		// ToDo: API to load and retrieve items

	};

}

#include "global/component/data_items/dummy.hpp"
#include "global/component/data_items/real.hpp"

#endif