#ifndef ENGINE_COMPONENT_RENDERER_STATUS_DUMMY_HPP
#define ENGINE_COMPONENT_RENDERER_STATUS_DUMMY_HPP
#pragma once

#include "local/component/renderer_status.hpp"

namespace engine
{

	class renderer_status_dummy_t : public renderer_status_t
	{

	public:

		uint64_t get_frame() const final;

	};
}

#endif