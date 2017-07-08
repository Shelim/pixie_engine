#ifndef ENGINE_COMMON_DEFAULT_RICHTEXT_HPP
#define ENGINE_COMMON_DEFAULT_RICHTEXT_HPP
#pragma once

#include <SDL.h>
#include <cstdio>
#include <cstdlib>
#include "bootstrapper.hpp"
#include "component/logger.hpp"
#include "component/frame_notifier.hpp"
#include "utility/vfs/filesystem.hpp"
#include "utility/text/ustring.hpp"
#include "utility/vfs/virtual_path.hpp"
#include "component/terminal_output.hpp"
#include "utility/text/richtext.hpp"

namespace engine
{

	SETTINGS_START(richtext_colors_t, normal)

	SETTINGS_END()

}

#endif