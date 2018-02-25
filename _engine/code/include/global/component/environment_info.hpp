#ifndef ENGINE_COMPONENT_ENVIRONMENT_INFO_HPP
#define ENGINE_COMPONENT_ENVIRONMENT_INFO_HPP
#pragma once


namespace engine
{
	class environment_info_t
	{

		public:
			virtual ~environment_info_t() { }

#include "def/enum/environment_info.def"

		virtual const ustring_t & get(type_t key) const = 0;
		  
		virtual status_t get_status(type_t key) const = 0;
	};
}

#define ENGINE_ENUM_HEADER_TO_USE "def/enum/environment_info.def"
#include "global/core/utility/enum_to_string.hpp"

#include "global/component/environment_info/dummy.hpp"
#include "global/component/environment_info/real.hpp"

#endif