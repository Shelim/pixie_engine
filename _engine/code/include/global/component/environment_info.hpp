#ifndef ENGINE_COMPONENT_ENVIRONMENT_INFO_HPP
#define ENGINE_COMPONENT_ENVIRONMENT_INFO_HPP
#pragma once

#include "global/core/platform/cpu_features.hpp"
#include "utility/platform/version.hpp"
#include "utility/text/ustring.hpp"
#include "global/core/manifest/os.hpp"

namespace engine
{
	class environment_info_t
	{

		public:
			virtual ~environment_info_t() { }

#define ENGINE_ENVIRONMENT_INFO_DEF(...) DEFINE_ENUM_ONLY_1ST_TYPE(type_t, __VA_ARGS__)
#define ENGINE_ENVIRONMENT_INFO_STATUS_DEF(...) DEFINE_ENUM_ONLY_1ST_TYPE(status_t, __VA_ARGS__)
#include "def/environment_info.def"

		static ustring_t get_key_name(type_t key)
		{
			switch(key)
			{
#define ENGINE_ENVIRONMENT_INFO_IMPL(key, name, type) case type_t::key: return name##_u;
#define ENGINE_ENVIRONMENT_INFO_DEF(...) DEFINE_TYPE_PASS(ENGINE_ENVIRONMENT_INFO_IMPL, __VA_ARGS__)
#include "def/environment_info.def"
			}
			return "Unknown"_u;
		}

		virtual ustring_t get(type_t key) const = 0;

#define ENGINE_ENVIRONMENT_INFO_IMPL(key, name, type) virtual ::type get_##key() const = 0;
#define ENGINE_ENVIRONMENT_INFO_DEF(...) DEFINE_TYPE_PASS(ENGINE_ENVIRONMENT_INFO_IMPL, __VA_ARGS__)
#include "def/environment_info.def"
		  
		virtual status_t status(type_t key) const = 0;
	};
}

#define ENGINE_ENVIRONMENT_INFO_DEF(...) DEFINE_ENUM_ONLY_1ST_TO_STRING(engine::environment_info_t::type_t, __VA_ARGS__)
#define ENGINE_ENVIRONMENT_INFO_STATUS_DEF(...) DEFINE_ENUM_ONLY_1ST_TO_STRING(engine::environment_info_t::status_t, __VA_ARGS__)
#include "def/environment_info.def"

#include "global/component/environment_info/dummy.hpp"
#include "global/component/environment_info/real.hpp"

#endif