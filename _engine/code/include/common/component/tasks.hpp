#ifndef ENGINE_COMPONENT_TASKS_HPP
#define ENGINE_COMPONENT_TASKS_HPP
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
#include "component/tasks/task.hpp"
#include "utility/text/parser.hpp"
#include <cereal/cereal.hpp>
#include <cereal/access.hpp>
#include <vlc/vlc.h>
#include <bitset>

namespace engine
{

}

#include "component/tasks/dummy.hpp"
#include "component/tasks/real.hpp"

#endif