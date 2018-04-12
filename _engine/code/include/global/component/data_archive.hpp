#ifndef ENGINE_COMPONENT_DATA_ARCHIVE_HPP
#define ENGINE_COMPONENT_DATA_ARCHIVE_HPP
#pragma once

namespace engine
{

	class data_archive_t
	{

	public:

		virtual ~data_archive_t()
		{

		}

		// ToDo: Add API for creating archives

	};

}

#include "global/component/data_archive/dummy.hpp"
#include "global/component/data_archive/real.hpp"

#endif