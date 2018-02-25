#ifndef ENGINE_COMPONENT_ENVIRONMENT_INFO_DUMMY_HPP
#define ENGINE_COMPONENT_ENVIRONMENT_INFO_DUMMY_HPP
#pragma once

#include "global/component/environment_info.hpp"

namespace engine
{

	class environment_info_dummy_t : public environment_info_t
	{

	public:
		
		ustring_t get(type_t key) const final
		{
			return ""_u;
		}

#define ENGINE_ENVIRONMENT_INFO_DEF(key, name, type) ::type get_##key() const final { return from_string<::type>(""_u); }
#include "def/environment_info.def"
		  
		status_t status(type_t key) const final
		{
			return status_t::not_yet_populated;
		}

	private:

	};
}

#endif