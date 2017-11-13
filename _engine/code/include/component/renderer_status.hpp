#ifndef ENGINE_COMPONENT_RENDERER_STATUS_HPP
#define ENGINE_COMPONENT_RENDERER_STATUS_HPP
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
#include "utility/container/sync_queue.hpp"
#include <cereal/cereal.hpp>
#include <cereal/access.hpp>
#include <vlc/vlc.h>
#include <bitset>

namespace engine
{

	class renderer_status_t
	{

	public:

		virtual ~renderer_status_t();

		virtual uint64_t get_frame() const = 0;
	};
}

#include "component/renderer_status/dummy.hpp"
#include "component/renderer_status/real.hpp"

#endif