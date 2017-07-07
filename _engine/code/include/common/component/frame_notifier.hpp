#ifndef ENGINE_COMPONENT_FRAME_NOTIFIER_HPP
#define ENGINE_COMPONENT_FRAME_NOTIFIER_HPP
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

	class frame_notifier_t
	{

	public:

		virtual uint64_t get_frame() const = 0;

		virtual ~frame_notifier_t() { }
	};
}

#include "component/frame_notifier/dummy.hpp"
#include "component/frame_notifier/real.hpp"

#endif