#ifndef ENGINE_COMPONENT_CONFIG_HPP
#define ENGINE_COMPONENT_CONFIG_HPP
#pragma once

#include <pugixml.hpp>
#include <string>
#include <vector>
#include <bitset>
#include <array>
#include <mutex>
#include <map>
#include <thread>
#include <chrono>
#include <sstream>
#include <SDL.h>
#include <functional>
#include <pugixml.hpp>
#include "utility/text/ustring.hpp"
#include "component/frame_notifier.hpp"
#include "utility/pattern/flags.hpp"
#include "component/logger/item.hpp"
#include "utility/pattern/class_settings.hpp"
#include "manifest_app.hpp"
#include <cereal/cereal.hpp>
#include <cereal/access.hpp>
#include "utility/pattern/enum.hpp"
#include <vlc/vlc.h>
#include <bitset>

namespace engine
{

	class config_t
	{
	public:

		
		config_t()
		{

		}

		virtual ~config_t()
		{

		}

#define ENGINE_CONFIG_GLOBAL_STD(type, name) virtual type get_global_##name() const = 0;  virtual void set_global_##name(type val) = 0;
#define ENGINE_CONFIG_LOCAL_STD(type, app, name) virtual type get_app_##app##_##name() const = 0;  virtual void set_app_##app##_##name(type val) = 0;
#define ENGINE_CONFIG_STD(type, name) virtual type get_cfg_##name(manifest_app_t::app_t app) const = 0;  virtual void set_cfg_##name(manifest_app_t::app_t app, type val) = 0; type get_cfg_##name() const { return get_cfg_##name(manifest_app_t::get_local_app()); } void set_cfg_##name(type val) { set_cfg_##name(manifest_app_t::get_local_app(), val); }
#include "std/config_std.hpp"

		enum class item_t
		{
#define ENGINE_CONFIG_GLOBAL_STD(type, name) global_##name,
#define ENGINE_CONFIG_LOCAL_STD(type, app, name) app_##app##_##name,
#define ENGINE_CONFIG_STD(type, name) cfg_##name,
#include "std/config_std.hpp"
			count
		};

		static const ustring_t type_to_text(item_t item)
		{
#define ENGINE_CONFIG_GLOBAL_STD(type, name) if(item == item_t::global_##name) return "global_" #name##_u;
#define ENGINE_CONFIG_LOCAL_STD(type, app, name) if(item == item_t::app_##app##_##name) return "local_" #app "_" #name##_u;
#define ENGINE_CONFIG_STD(type, name) if(item == item_t::cfg_##name) return "cfg_" #name##_u;
#include "std/config_std.hpp"
			return "Unknown"_u;
		}

	private:

	};

	SETTINGS_TABLE_START(config_t)

#define ENGINE_CONFIG_GLOBAL_STD(type, name) SETTINGS_TABLE_ENTRY(type, global_##name)
#define ENGINE_CONFIG_LOCAL_STD(type, app, name) SETTINGS_TABLE_ENTRY(type, app_##app##_##name)
#define ENGINE_CONFIG_STD(type, name) SETTINGS_TABLE_ENTRY(type, cfg_##name)
#include "std/config_std.hpp"

	SETTINGS_TABLE_END()
}

#include "component/config/dummy.hpp"
#include "component/config/real.hpp"


#endif