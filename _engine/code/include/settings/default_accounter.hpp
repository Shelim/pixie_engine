#ifndef ENGINE_COMMON_DEFAULT_ACCOUNTER_HPP
#define ENGINE_COMMON_DEFAULT_ACCOUNTER_HPP
#pragma once

#include "bootstrapper.hpp"
#include "global/component/thread_accounter.hpp"

namespace engine
{

	SETTINGS_START(thread_accounter_t, normal)

		SETTINGS_SET(std::chrono::duration<double>, snapshot_refresh_interval, std::chrono::milliseconds(1000))

	SETTINGS_END()

}

#endif