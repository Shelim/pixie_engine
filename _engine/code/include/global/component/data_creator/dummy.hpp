#ifndef ENGINE_COMPONENT_DATA_CREATOR_DUMMY_HPP
#define ENGINE_COMPONENT_DATA_CREATOR_DUMMY_HPP
#pragma once

#include "global/component/data_creator.hpp"

namespace engine
{

	class data_creator_dummy_t : public data_creator_t
	{

	public:

		data_creator_dummy_t()
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