#ifndef ENGINE_COMPONENT_ENVIRONMENT_INFO_HPP
#define ENGINE_COMPONENT_ENVIRONMENT_INFO_HPP
#pragma once


namespace engine
{
	class environment_info_t
	{

		public:
			virtual ~environment_info_t() { }

	};
	class environment_info_dummy_t : public environment_info_t
	{

	};
	class environment_info_real_t : public environment_info_t
	{

	};
}

#endif

/*

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
#include "utility/pattern/flags.hpp"
#include "global/component/logger/item.hpp"
#include "utility/text/ustring.hpp"
#include "utility/platform/info.hpp"
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
#define ENGINE_ENVIRONMENT_INFO_DEF(key, name) key,
#include "def/environment_info.def"
			count
		};

		enum class status_t
		{
#define ENGINE_ENVIRONMENT_INFO_STATUS_DEF(status) status,
#include "def/environment_info.def"
			count
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
#define ENGINE_ENVIRONMENT_INFO_DEF(key_name, name) if(key == key_t::key_name) return name;
#include "def/environment_info.def"

			return  ""_u;
		}

	private:
	};

}

#include "global/component/environment_info/dummy.hpp"
#include "global/component/environment_info/real.hpp"


#endif
*/