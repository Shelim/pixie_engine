#ifndef ENGINE_COMPONENT_DATA_MANIPULATOR_HPP
#define ENGINE_COMPONENT_DATA_MANIPULATOR_HPP
#pragma once

namespace engine
{

	class data_manipulator_t
	{

	public:

		virtual ~data_manipulator_t()
		{

		} 
		
		// virtual void sign_item(const virtual_path_t & item, const signature_t & signature) = 0;
		virtual void softlink_item(const virtual_path_t & item, const virtual_path_t & link) = 0;
		virtual void move_item(const virtual_path_t & src, const virtual_path_t & dst) = 0;
		virtual void copy_item(const virtual_path_t & src, const virtual_path_t & dst) = 0;
		virtual void delete_item(const virtual_path_t & filename) = 0;
		virtual void delete_item_newest(const virtual_path_t & filename) = 0;


	};

}

#include "global/component/data_manipulator/dummy.hpp"
#include "global/component/data_manipulator/real.hpp"

#endif