#ifndef ENGINE_COMPONENT_DATA_CREATOR_HPP
#define ENGINE_COMPONENT_DATA_CREATOR_HPP
#pragma once

#include "global/core/data/output.hpp"

namespace engine
{

	class data_creator_t
	{

	public:

		virtual ~data_creator_t()
		{

		}

		virtual std::unique_ptr<data::output_t> create_new(virtual_path_t virtual_path) = 0;

	};

}

#include "global/component/data_creator/dummy.hpp"
#include "global/component/data_creator/real.hpp"

#endif