#ifndef ENGINE_COMPONENT_ENVIRONMENT_INFO_HPP
#define ENGINE_COMPONENT_ENVIRONMENT_INFO_HPP
#pragma once

#include "utility/platform/cpu_features.hpp"
#include "utility/platform/version.hpp"
#include "utility/text/ustring.hpp"
#include "global/core/manifest/os.hpp"

namespace engine
{
	class environment_info_t
	{

		public:
			virtual ~environment_info_t() { }

#include "def/enum/environment_info.def"

		virtual ustring_t get(type_t key) const = 0;

#define ENGINE_ENVIRONMENT_INFO_DEF(key, name, type) virtual ::type get_##key() const = 0;
#include "def/environment_info.def"
		  
		virtual status_t status(type_t key) const = 0;
	};
}

#define ENGINE_ENUM_HEADER_TO_USE "def/enum/environment_info.def"
#include "global/core/utility/enum_to_string.hpp"

#include "global/component/environment_info/dummy.hpp"
#include "global/component/environment_info/real.hpp"

#endif