#ifndef ENGINE_COMPONENT_DATA_CREATOR_REAL_HPP
#define ENGINE_COMPONENT_DATA_CREATOR_REAL_HPP
#pragma once

#include "global/component/data_creator.hpp"

namespace engine
{
	class data_creator_real_t : public data_creator_t
	{

	public:

		data_creator_real_t()
		{

		}

		std::unique_ptr<data::output_t> create_new(virtual_path_t virtual_path) final
		{
			return std::make_unique<data::output_void_t>(virtual_path);
		}

	private:


	};
}


#endif