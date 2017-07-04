#ifndef ENGINE_COMPONENT_FRAME_NOTIFIER_DUMMY_HPP
#define ENGINE_COMPONENT_FRAME_NOTIFIER_DUMMY_HPP
#pragma once

#include "component/frame_notifier.hpp"

namespace engine
{

	class frame_notifier_dummy_t : public frame_notifier_t
	{

	public:

		uint64_t get_frame() const final
		{
			return 0;
		}

	};
}

#endif