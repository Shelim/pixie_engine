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
#include <cereal/cereal.hpp>
#include <cereal/access.hpp>
#include <vlc/vlc.h>
#include <bitset>

#define GAME_CONFIG_LOCAL__(name, app_unix_name) GAME_CONFIG_LOCAL_(name, app_unix_name)
#define GAME_CONFIG_LOCAL_(name, app_unix_name) app_unix_name##_##name
#define GAME_CONFIG_LOCAL(name) GAME_CONFIG_LOCAL__(name, PIXIE_OUTPUT_UNIX_NAME)

#define GAME_CONFIG_GET_LOCAL__(name, app_unix_name) GAME_CONFIG_GET_LOCAL_(name, app_unix_name)
#define GAME_CONFIG_GET_LOCAL_(name, app_unix_name) get_##app_unix_name##_##name
#define config_get_local(name) GAME_CONFIG_GET_LOCAL__(name, PIXIE_OUTPUT_UNIX_NAME) ()

#define GAME_CONFIG_SET_LOCAL__(name, app_unix_name) GAME_CONFIG_SET_LOCAL_(name, app_unix_name)
#define GAME_CONFIG_SET_LOCAL_(name, app_unix_name) set_##app_unix_name##_##name
#define config_set_local(name, value) GAME_CONFIG_SET_LOCAL__(name, PIXIE_OUTPUT_UNIX_NAME) (value)

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

#define GAME_CONFIG_STD(type, name) virtual const type & get_##name() const = 0;  virtual void set_##name(const type & val) = 0;
#include "std/config_std.hpp"

		enum class item_t
		{
#define GAME_CONFIG_STD(type, name) name,
#include "std/config_std.hpp"
			count
		};

		static const ustring_t type_to_text(item_t item)
		{
#define GAME_CONFIG_STD(type, name) if(item == item_t::name) return #name##_u;
#include "std/config_std.hpp"
			return "Unknown"_u;
		}

	private:

	};

	SETTINGS_TABLE_START(config_t)

#define GAME_CONFIG_STD(type, name) SETTINGS_TABLE_ENTRY(type, name)
#include "std/config_std.hpp"

	SETTINGS_TABLE_END()
}

#include "component/config/dummy.hpp"
#include "component/config/real.hpp"


#endif