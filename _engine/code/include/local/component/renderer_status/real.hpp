#ifndef ENGINE_COMPONENT_RENDERER_STATUS_REAL_HPP
#define ENGINE_COMPONENT_RENDERER_STATUS_REAL_HPP
#pragma once

#include "local/component/renderer_status.hpp"

namespace engine
{

	class renderer_status_real_t : public renderer_status_t
	{

	public:

		uint64_t get_frame() const final
        {
            return 0; // ToDo!
        }

	};
}

#endif