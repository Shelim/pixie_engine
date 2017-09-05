#ifndef ENGINE_COMPONENT_DATA_ARCHIVES_HPP
#define ENGINE_COMPONENT_DATA_ARCHIVES_HPP
#pragma once


namespace engine
{

	class data_archives_t
	{

	public:

		data_archives_t()
		{

		}

		virtual ~data_archives_t()
		{

		}

	private:

	};
}

#include "component/data_archives/dummy.hpp"
#include "component/data_archives/real.hpp"

#endif