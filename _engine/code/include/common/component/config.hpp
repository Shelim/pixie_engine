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

namespace engine
{

	class config_t : public std::enable_shared_from_this<config_t>
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