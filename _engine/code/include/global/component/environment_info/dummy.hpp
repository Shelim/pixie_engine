#ifndef ENGINE_COMPONENT_ENVIRONMENT_INFO_DUMMY_HPP
#define ENGINE_COMPONENT_ENVIRONMENT_INFO_DUMMY_HPP
#pragma once

#include "global/component/environment_info.hpp"

namespace engine
{

	class environment_info_dummy_t : public environment_info_t
	{

	public:
		
		const ustring_t & get(key_t key) const final
		{
			return ""_u;
		}
		  
		status_t get_status(key_t key) const final
		{
			return status_t::not_yet_populated;
		}

	private:

	};
}

#endif