#ifndef ENGINE_COMPONENT_ENVIRONMENT_INFO_HPP
#define ENGINE_COMPONENT_ENVIRONMENT_INFO_HPP
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
#include <pugixml.hpp>
#include "utility/text/ustring.hpp"
#include "component/frame_notifier.hpp"
#include "utility/pattern/flags.hpp"
#include "component/logger/item.hpp"
#include "utility/text/parser.hpp"
#include <cereal/cereal.hpp>
#include <cereal/access.hpp>
#include <vlc/vlc.h>
#include <bitset>

namespace engine
{

	class environment_info_t
	{
	public:

		enum class key_t
		{
#define ENGINE_ENVIRONMENT_INFO_STD(key, name) key,
#include "std/environment_info_std.hpp"
			count
		};

		enum class status_t
		{
			not_yet_populated,
			normal,
			good,
			bad
		};

		environment_info_t()
		{

		}

		virtual ~environment_info_t()
		{

		}

		virtual const ustring_t & get(key_t key) const = 0;
		virtual status_t get_status(key_t key) const = 0;
		
		static const ustring_t key_to_name(key_t key)
		{
#define ENGINE_ENVIRONMENT_INFO_STD(key_name, name) if(key == key_t::key_name) return name;
#include "std/environment_info_std.hpp"

			return  ""_u;
		}

	};

}

#include "component/environment_info/dummy.hpp"
#include "component/environment_info/real.hpp"


#endif