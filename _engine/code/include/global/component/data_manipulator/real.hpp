#ifndef ENGINE_COMPONENT_DATA_MANIPULATOR_REAL_HPP
#define ENGINE_COMPONENT_DATA_MANIPULATOR_REAL_HPP
#pragma once

#include "global/component/data_manipulator.hpp"

namespace engine
{
	class data_manipulator_real_t : public data_manipulator_t
	{

	public:

		data_manipulator_real_t()
		{

		}
		
		void softlink_item(const virtual_path_t & item, const virtual_path_t & link) final
		{

		}
		void move_item(const virtual_path_t & src, const virtual_path_t & dst) final
		{

		}
		void copy_item(const virtual_path_t & src, const virtual_path_t & dst) final
		{

		}
		void delete_item(const virtual_path_t & filename) final
		{

		}
		void delete_item_newest(const virtual_path_t & filename) final
		{

		}

	private:


	};
}


#endif