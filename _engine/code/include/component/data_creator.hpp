/*

#ifndef ENGINE_COMPONENT_DATA_CREATOR_HPP
#define ENGINE_COMPONENT_DATA_CREATOR_HPP
#pragma once

#include "core/data/output.hpp"

namespace engine
{

	class data_creator_t
	{

	public:

		data_creator_t()
		{

		}

		virtual ~data_creator_t()
		{

		}

		virtual std::unique_ptr<data::output_t> create_new_file(const virtual_path_t & path) = 0;

	private:

	};
}

#include "component/data_archives/dummy.hpp"
#include "component/data_archives/real.hpp"

#endif

*/